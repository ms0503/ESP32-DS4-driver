# DualShock4 Input UART Transmitter on ESP32
- [日本語](README.ja.md)

ESP32-DS4 is a program, which receive DualShock4 signal and send UART signal on 32 and 33 pin.

# UART Packet Specification
| Baud rate | Data length | Parity | Stop bits |
| :-------: | :---------: | :----: | :-------: |
| 115200bps |    8-bit    |   None |   1-bit   |

| byte |    0   |     1     |     2    |    3   |    4   |    5   |    6   |  7  |  8  |     9    |
| :--: | :----: | :-------: | :------: | :----: | :----: | :----: | :----: | :-: | :-: | :------: |
|      | HEADER | BTN\_HIGH | BTN\_LOW | ST\_RX | ST\_RY | ST\_LX | ST\_LY |  R2 |  L2 | CHECKSUM |

## HEADER
HEADER is for checking data start.
It is 0x80 (128).

## BTN\_HIGH
BTN\_HIGH is button state high byte.
When a button is pressed, its bit is high; otherwise, it is low.
It contains below (LSB to MSB):
- R1
- L1
- R3
- L3
- Options
- Share
- PS Logo
- Touchpad (Press)

## BTN\_LOW
BTN\_LOW is button state low byte.
When a button is pressed, its bit is high; otherwise, it is low.
It contains below (LSB to MSB):
- Circle
- Triangle
- Square
- Cross
- POV Right
- POV Up
- POV Left
- POV Down

## ST\_RX
ST\_RX is x-coordinate of right stick.
Direction is left to right.

## ST\_RY
ST\_RY is y-coordinate of right stick.
Direction is down to up.

## ST\_LX
ST\_LX is x-coordinate of left stick.
Direction is left to right.

## ST\_LY
ST\_LY is y-coordinate of left stick.
Direction is down to top.

## R2
R2 is an analog value of R2 trigger.
When pushed to the end, it is 0xff (255).

## L2
L2 is an analog value of L2 trigger.
When pushed to the end, it is 0xff (255).

## CHECKSUM
CHECKSUM is data checksum.
1. Add all from HEADER to L2.
2. Truncate bits that overflow from 8-bit.

# Driver
[ms0503/ESP32-DS4-driver](https://github.com/ms0503/ESP32-DS4-driver) is a simple driver on C++ and Rust.
