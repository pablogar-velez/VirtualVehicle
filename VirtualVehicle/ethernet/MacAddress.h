#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

class MacAddress
{
public:
    static constexpr std::size_t Size =
        6;

    using Storage =
        std::array<std::uint8_t, Size>;

    MacAddress() = default;

    explicit MacAddress(
        const Storage& bytes)
        : bytes(
            bytes
        )
    {
    }

    MacAddress(
        std::uint8_t byte0,
        std::uint8_t byte1,
        std::uint8_t byte2,
        std::uint8_t byte3,
        std::uint8_t byte4,
        std::uint8_t byte5)
        : bytes{
            byte0,
            byte1,
            byte2,
            byte3,
            byte4,
            byte5
        }
    {
    }

    const Storage&
        getBytes() const
    {
        return bytes;
    }

    bool operator==(
        const MacAddress& other) const
    {
        return
            bytes ==
            other.bytes;
    }

    bool operator!=(
        const MacAddress& other) const
    {
        return
            !(
                *this ==
                other
                );
    }

    bool operator<(
        const MacAddress& other) const
    {
        return
            bytes <
            other.bytes;
    }

    std::string toString() const
    {
        std::ostringstream stream;

        stream
            << std::hex
            << std::uppercase
            << std::setfill('0');

        for (
            std::size_t index = 0;
            index < bytes.size();
            ++index
            )
        {
            if (
                index >
                0
                )
            {
                stream << ":";
            }

            stream
                << std::setw(2)
                << static_cast<unsigned int>(
                    bytes[index]
                    );
        }

        return stream.str();
    }

private:
    Storage bytes{};
};