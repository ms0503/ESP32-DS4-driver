/**
 * @file esp32_ds4.h
 * @brief ESP32-DS4-driver
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace ESP32DS4 {
    //! @brief Header byte.
    constexpr std::uint8_t HEADER = 0x80;
    //! @brief Data byte length.
    constexpr std::size_t LENGTH = 10;

    /**
     * @brief Button bits.
     */
    enum class Button : std::uint16_t {
        //! @brief Circle button.
        Circle = 0b0000'0000'0000'0001,
        //! @brief Triangle button.
        Triangle = 0b0000'0000'0000'0010,
        //! @brief Square button.
        Square = 0b0000'0000'0000'0100,
        //! @brief Cross button.
        Cross = 0b0000'0000'0000'1000,
        //! @brief D-pad right button.
        Right = 0b0000'0000'0001'0000,
        //! @brief D-pad up button.
        Up = 0b0000'0000'0010'0000,
        //! @brief D-pad left button.
        Left = 0b0000'0000'0100'0000,
        //! @brief D-pad down button.
        Down = 0b0000'0000'1000'0000,
        //! @brief Right bumper button.
        R1 = 0b0000'0001'0000'0000,
        //! @brief Left bumper button.
        L1 = 0b0000'0010'0000'0000,
        //! @brief Right stick button.
        R3 = 0b0000'0100'0000'0000,
        //! @brief Left stick button.
        L3 = 0b0000'1000'0000'0000,
        //! @brief Center-right button.
        Options = 0b0001'0000'0000'0000,
        //! @brief Center-left button.
        Share = 0b0010'0000'0000'0000,
        //! @brief Logo button.
        PSLogo = 0b0100'0000'0000'0000,
        //! @brief Touchpad button.
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

    /**
     * @brief Analog sticks.
     */
    struct Sticks {
        //! @brief X-coordinate of right stick.
        std::int8_t rx;
        //! @brief Y-coordinate of right stick.
        std::int8_t ry;
        //! @brief X-coordinate of left stick.
        std::int8_t lx;
        //! @brief Y-coordinate of left stick.
        std::int8_t ly;
    };

    /**
     * @brief Trigger buttons.
     */
    struct Triggers {
        //! @brief Right trigger button.
        std::uint8_t r2;
        //! @brief Left trigger button.
        std::uint8_t l2;
    };

    /**
     * @brief Packet data.
     */
    class Packet {
    public:
        const bool isValid;

        explicit Packet(std::uint8_t (&raw)[LENGTH]);

        ~Packet();

        /**
         * @brief Gets a bit of specified button.
         * @param [in] t Button type.
         * @retval true if a bit is high.
         * @retval false otherwise.
         */
        bool get_bit(Button t) const noexcept;

        /**
         * @brief Gets sticks data.
         * @returns A sticks data.
         */
        Sticks get_sticks() const noexcept;

        /**
         * @brief Gets triggers data.
         * @returns A triggers data.
         */
        Triggers get_triggers() const noexcept;

    private:
        std::uint8_t (&raw)[LENGTH];

        static bool validate(std::uint8_t (&raw)[LENGTH]) noexcept {
            if(HEADER != raw[static_cast<std::size_t>(Byte::Header)]) {
                return false;
            }
            if(get_checksum(raw) != raw[static_cast<std::size_t>(Byte::Checksum)]) {
                return false;
            }
            return true;
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
