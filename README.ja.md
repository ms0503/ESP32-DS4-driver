# Driver for DualShock4 Input UART Transmitter on ESP32
- [English](README.md)

ESP32-DS4-driverは[ESP32-DS4](https://github.com/ms0503/ESP32-DS4)向けのドライバーです。

# 必要物
## C++
- C++14をサポートするC++コンパイラ
- CMake 3.5以降

## Python 3
- Python 3.6以降
- pip

## Rust
- Cargo
- `core`クレート

# 使用方法
## C++
1. クローンする。
2. `CMakeLists.txt`へサブディレクトリ`cxx`を追加する。
3. ビルドターゲットへ`esp32-ds4`をリンクさせる。

## Python 3
1. クローンする。
2. 開発環境で`pip install <リポジトリへのパス>/py`を実行する。

## Rust
1. クローンする。
2. `Cargo.toml`の`dependencies`セクションへ`rust`にある`esp32-ds4-driver`を追加する。
