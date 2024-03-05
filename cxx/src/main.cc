#include <cstddef>
#include <cstdint>
#include <esp32_ds4.h>

namespace ESP32DS4 {
    Packet::Packet(std::uint8_t (&raw)[LENGTH]):
        raw(raw) {
    }

    Packet::~Packet() = default;

    bool Packet::get_bit(const Button t) const noexcept {
        const std::uint16_t _t = static_cast<std::uint16_t>(t);
        return 0xff < _t ? raw[static_cast<std::size_t>(Byte::BtnHigh)] & (_t >> 8) : raw[static_cast<std::size_t>(Byte::BtnLow)] & _t;
    }

    Sticks Packet::get_sticks() const noexcept {
        return Sticks {
            .rx = static_cast<std::int8_t>(raw[static_cast<std::size_t>(Byte::StickRX)]),
            .ry = static_cast<std::int8_t>(raw[static_cast<std::size_t>(Byte::StickRY)]),
            .lx = static_cast<std::int8_t>(raw[static_cast<std::size_t>(Byte::StickLX)]),
            .ly = static_cast<std::int8_t>(raw[static_cast<std::size_t>(Byte::StickLY)])
        };
    }

    Triggers Packet::get_triggers() const noexcept {
        return Triggers {
            .r2 = raw[static_cast<std::size_t>(Byte::R2)],
            .l2 = raw[static_cast<std::size_t>(Byte::L2)]
        };
    }
}
