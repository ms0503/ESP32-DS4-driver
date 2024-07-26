//! ESP32-DS4-driver

#![allow(dead_code)]
#![feature(error_in_core)]
#![no_std]

use core::error::Error;
use core::fmt::Display;

/// Header byte.
pub const HEADER: u8 = 0x80;
/// Data byte length.
pub const LENGTH: usize = 10;

/// Button bits.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
#[repr(u16)]
pub enum Button {
    /// Circle button.
    Circle = 0b0000_0000_0000_0001,
    /// Triangle button.
    Triangle = 0b0000_0000_0000_0010,
    /// Square button.
    Square = 0b0000_0000_0000_0100,
    /// Cross button.
    Cross = 0b0000_0000_0000_1000,
    /// D-pad right button.
    Right = 0b0000_0000_0001_0000,
    /// D-pad up button.
    Up = 0b0000_0000_0010_0000,
    /// D-pad left button.
    Left = 0b0000_0000_0100_0000,
    /// D-pad down button.
    Down = 0b0000_0000_1000_0000,
    /// Right bumper button.
    R1 = 0b0000_0001_0000_0000,
    /// Left bumper button.
    L1 = 0b0000_0010_0000_0000,
    /// Right stick button.
    R3 = 0b0000_0100_0000_0000,
    /// Left stick button.
    L3 = 0b0000_1000_0000_0000,
    /// Center-right button.
    Options = 0b0001_0000_0000_0000,
    /// Center-left button.
    Share = 0b0010_0000_0000_0000,
    /// Logo button.
    PSLogo = 0b0100_0000_0000_0000,
    /// Touchpad button.
    Touchpad = 0b1000_0000_0000_0000,
}

#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
#[repr(usize)]
enum Byte {
    Header,
    BtnHigh,
    BtnLow,
    StickRX,
    StickRY,
    StickLX,
    StickLY,
    R2,
    L2,
    Checksum,
}

/// Analog sticks.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct Sticks {
    /// X coordinate of right stick.
    pub rx: i8,
    /// Y coordinate of right stick.
    pub ry: i8,
    /// X coordinate of left stick.
    pub lx: i8,
    /// Y coordinate of left stick.
    pub ly: i8,
}

/// Trigger buttons.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct Triggers {
    /// Right trigger button.
    pub r2: u8,
    /// Left trigger button.
    pub l2: u8,
}

/// Packet validation error.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct ValidationError;

impl Display for ValidationError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "packet validation failed!")
    }
}

impl Error for ValidationError {
    fn source(&self) -> Option<&(dyn Error + 'static)> {
        None
    }
}

/// Packet data.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct Packet<'a> {
    raw: &'a [u8; LENGTH],
}

impl<'a> Packet<'a> {
    /// Creates an instance with raw data.
    ///
    /// # Arguments
    /// * `raw` - Raw packet data.
    ///
    /// # Returns
    /// An instance of Packet.
    ///
    /// # Errors
    /// This may return [ValidationError] whether raw data is invalid.
    pub fn from(raw: &'a [u8; LENGTH]) -> Result<Self, ValidationError> {
        if Self::validate(raw) {
            Ok(Self { raw })
        } else {
            Err(ValidationError)
        }
    }

    /// Gets a bit of specified button.
    ///
    /// # Arguments
    /// * `t` - Button type.
    ///
    /// # Returns
    /// true if a bit is high, otherwise false.
    pub fn get_bit(&self, t: Button) -> bool {
        if 0xff < t as u16 {
            (self.raw[Byte::BtnHigh as usize] & (t as u16 >> 8) as u8) != 0
        } else {
            (self.raw[Byte::BtnLow as usize] & t as u8) != 0
        }
    }

    /// Gets sticks data.
    ///
    /// # Returns
    /// A sticks data.
    pub fn get_sticks(&self) -> Sticks {
        Sticks {
            rx: self.raw[Byte::StickRX as usize] as i8,
            ry: self.raw[Byte::StickRY as usize] as i8,
            lx: self.raw[Byte::StickLX as usize] as i8,
            ly: self.raw[Byte::StickLY as usize] as i8,
        }
    }

    /// Gets triggers data.
    ///
    /// # Returns
    /// A triggers data.
    pub fn get_triggers(&self) -> Triggers {
        Triggers {
            r2: self.raw[Byte::R2 as usize],
            l2: self.raw[Byte::L2 as usize],
        }
    }

    fn validate(raw: &[u8; 10]) -> bool {
        if HEADER != raw[Byte::Header as usize] {
            return false;
        }
        if Self::get_checksum(raw) != raw[Byte::Checksum as usize] {
            return false;
        }
        true
    }

    fn get_checksum(raw: &[u8; 10]) -> u8 {
        let mut acc = 0u8;
        for n in raw[0..LENGTH - 1].iter() {
            acc = acc.wrapping_add(*n);
        }
        acc
    }
}
