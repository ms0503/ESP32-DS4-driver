# -*- encoding: UTF-8 -*-
"""ESP32-DS4-driver
"""

from enum import IntEnum

HEADER: int = 0x80
LENGTH: int = 10


class Button(IntEnum):
    """Button bits.

    Attributes:
        Circle (int): Circle button.
        Triangle (int): Triangle button.
        Square (int): Square button.
        Cross (int): Cross button.
        Right (int): D-pad right button.
        Up (int): D-pad up button.
        Left (int): D-pad left button.
        Down (int): D-pad down button.
        R1 (int): Right bumper button.
        L1 (int): Left bumper button.
        R3 (int): Right stick button.
        L3 (int): Left stick button.
        Options (int): Center-right button.
        Share (int): Center-left button.
        PSLogo (int): Logo button.
        Touchpad (int): Touchpad button.
    """

    Circle = 0b0000_0000_0000_0001
    Triangle = 0b0000_0000_0000_0010
    Square = 0b0000_0000_0000_0100
    Cross = 0b0000_0000_0000_1000
    Right = 0b0000_0000_0001_0000
    Up = 0b0000_0000_0010_0000
    Left = 0b0000_0000_0100_0000
    Down = 0b0000_0000_1000_0000
    R1 = 0b0000_0001_0000_0000
    L1 = 0b0000_0010_0000_0000
    R3 = 0b0000_0100_0000_0000
    L3 = 0b0000_1000_0000_0000
    Options = 0b0001_0000_0000_0000
    Share = 0b0010_0000_0000_0000
    PSLogo = 0b0100_0000_0000_0000
    Touchpad = 0b1000_0000_0000_0000


# noinspection PyMissingOrEmptyDocstring
class Byte(IntEnum):
    Header = 0
    BtnHigh = 1
    BtnLow = 2
    StickRX = 3
    StickRY = 4
    StickLX = 5
    StickLY = 6
    R2 = 7
    L2 = 8
    Checksum = 9


class Sticks:
    """Analog sticks.

    Attributes:
        rx (int): X-coordinate of right stick.
        ry (int): Y-coordinate of right stick.
        lx (int): X-coordinate of left stick.
        ly (int): Y-coordinate of left stick.
    """

    def __init__(self, rx: int, ry: int, lx: int, ly: int):
        self.rx = rx
        self.ry = ry
        self.lx = lx
        self.ly = ly


class Triggers:
    """Trigger buttons.

    Attributes:
        r2 (int): Right trigger button.
        l2 (int): Left trigger button.
    """

    def __init__(self, r2: int, l2: int):
        self.r2 = r2
        self.l2 = l2


class ValidationError(Exception):
    """Packet validation error.
    """

    def __str__(self):
        return 'packet validation failed!'


class Packet:
    """Packet data.
    """

    def __init__(self, raw: list):
        if Packet._validate(raw):
            self._raw = raw
        else:
            raise ValidationError()

    @staticmethod
    def _get_checksum(raw: list) -> int:
        checksum = 0
        for i in range(LENGTH - 1):
            checksum = checksum + raw[i] & 0xff
        return checksum

    @staticmethod
    def _validate(raw: list) -> bool:
        if HEADER != raw[Byte.Header]:
            return False
        if Packet._get_checksum(raw) != raw[Byte.Checksum]:
            return False
        return True

    def get_bit(self, t: Button) -> bool:
        """Gets a bit of specified button.

        Args:
            t (Button): Button type.

        Returns:
            bool: True if a bit is high, otherwise False.
        """

        return self._raw[Byte.BtnHigh] & t >> 8 != 0 if 0xff < t else self._raw[Byte.BtnLow] & t != 0

    def get_sticks(self) -> Sticks:
        """Gets sticks data.

        Returns:
            Sticks: A sticks data.
        """

        return Sticks(self._raw[Byte.StickRX], self._raw[Byte.StickRY], self._raw[Byte.StickLX],
                      self._raw[Byte.StickLY])

    def get_triggers(self) -> Triggers:
        """Gets triggers data.

        Returns:
            Triggers: A triggers data.
        """

        return Triggers(self._raw[Byte.R2], self._raw[Byte.L2])
