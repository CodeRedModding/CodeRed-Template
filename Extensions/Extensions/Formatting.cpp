#include "Formatting.hpp"

namespace Format
{
    void Empty(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void FillRight(std::ostringstream& stream, const char& fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ostringstream& stream, const char& fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::left;
    }

    void FillRight(std::ofstream& stream, const char& fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ofstream& stream, const char& fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::left;
    }

    std::string Hex(uintptr_t address, uint64_t width)
    {
        std::ostringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::hex << address;
        return stream.str();
    }

    std::string Decimal(uintptr_t address, uint64_t width)
    {
        std::ostringstream stream;
        stream << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::dec << address;
        return stream.str();
    }

    std::string Precision(float value, uint64_t precision)
    {
        std::ostringstream stream;
        stream << std::setprecision(precision) << value;
        return stream.str();
    }
}