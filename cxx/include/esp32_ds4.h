#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace ESP32DS4 {
    /// Header byte.
    constexpr std::uint8_t HEADER = 0x80;
    /// Data byte length.
    constexpr std::size_t LENGTH = 10;

    /// Button bits.
    enum class Button : std::uint16_t {
        /// Circle button.
        Circle = 0b0000'0000'0000'0001,
        /// Triangle button.
        Triangle = 0b0000'0000'0000'0010,
        /// Square button.
        Square = 0b0000'0000'0000'0100,
        /// Cross button.
        Cross = 0b0000'0000'0000'1000,
        /// D-pad right button.
        Right = 0b0000'0000'0001'0000,
        /// D-pad up button.
        Up = 0b0000'0000'0010'0000,
        /// D-pad left button.
        Left = 0b0000'0000'0100'0000,
        /// D-pad down button.
        Down = 0b0000'0000'1000'0000,
        /// Right bumper button.
        R1 = 0b0000'0001'0000'0000,
        /// Left bumper button.
        L1 = 0b0000'0010'0000'0000,
        /// Right stick button.
        R3 = 0b0000'0100'0000'0000,
        /// Left stick button.
        L3 = 0b0000'1000'0000'0000,
        /// Center-right button.
        Options = 0b0001'0000'0000'0000,
        /// Center-left button.
        Share = 0b0010'0000'0000'0000,
        /// Logo button.
        PSLogo = 0b0100'0000'0000'0000,
        /// Touchpad button.
        Touchpad = 0b1000'0000'0000'0000
    };

    enum class Byte : std::size_t {
        Header,
        BtnHigh,
        BtnLow,
        StickRX,
        StickRY,
        StickLX,
        StickLY,
        R2,
        L2,
        Checksum
    };

    /// Analog sticks.
    struct Sticks {
        /// X-coordinate of right stick.
        std::int8_t rx;
        /// Y-coordinate of right stick.
        std::int8_t ry;
        /// X-coordinate of left stick.
        std::int8_t lx;
        /// Y-coordinate of left stick.
        std::int8_t ly;
    };

    /// Trigger buttons.
    struct Triggers {
        /// Right trigger button.
        std::uint8_t r2;
        /// Left trigger button.
        std::uint8_t l2;
    };

    class Packet {
    public:
        explicit Packet(std::uint8_t (&raw)[LENGTH]);

        ~Packet();

        bool get_bit(Button t) const noexcept;

        Sticks get_sticks() const noexcept;

        Triggers get_triggers() const noexcept;

    private:
        std::uint8_t (&raw)[LENGTH];

        static bool validate(std::uint8_t (&raw)[LENGTH]) noexcept {
            return get_checksum(raw) == raw[static_cast<std::size_t>(Byte::Checksum)];
        }

        static std::uint8_t get_checksum(std::uint8_t (&raw)[LENGTH]) noexcept {
            std::uint8_t checksum = 0;
            std::for_each(raw, raw + LENGTH - 1, [&](auto d) {
                checksum += d;
            });
            return checksum;
        }
    };
}
