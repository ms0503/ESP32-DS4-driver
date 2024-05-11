# Driver for DualShock4 Input UART Transmitter on ESP32
- [日本語](README.ja.md)

ESP32-DS4-driver is a driver for [ESP32-DS4](https://github.com/ms0503/ESP32-DS4).

# Dependencies
## On C++
- Any C++ compiler with C++14 support
- CMake 3.1 or later

## On Rust
- Cargo
- `core` crate

# Usage
## On C++
1. Clone this.
2. Add subdirectory `cxx` to your `CMakeLists.txt`.
3. Link `esp32-ds4` to your build target.

## On Rust
1. Clone this.
2. Add `esp32-ds4-driver` with path `rust` to `dependencies` section on your `Cargo.toml`.
