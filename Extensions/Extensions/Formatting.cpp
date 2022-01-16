#include "Formatting.hpp"

namespace Format
{
    bool IsStringNumber(std::string_view str)
    {
        for (const char& c : str)
        {
            if (!std::isdigit(c))
            {
                return false;
            }
        }

        return true;
    }

    bool IsStringFloat(std::string_view str)
    {
        for (const char& c : str)
        {
            if (!std::isdigit(c) && c != '.' && c != 'f')
            {
                return false;
            }
        }

        return true;
    }

    void EraseAllChars(std::string& str, char character)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
    }

    std::string RemoveAllChars(std::string str, char character)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
        return str;
    }

    void Empty(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void FillRight(std::ostringstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ostringstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::left;
    }

    void FillRight(std::ofstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ofstream& stream, char fill, uint64_t width)
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

    std::vector<std::string> SplitArguments(const std::string& arguments)
    {
        std::vector<std::string> returnVector;
        std::string currentWord;

        for (const char x : arguments) 
        {
            if (x == ' ')
            {
                returnVector.push_back(currentWord);
                currentWord.clear();
            }
            else
            {
                currentWord = currentWord + x;
            }
        }

        returnVector.push_back(currentWord);

        return returnVector;
    }

    std::string ParseArguments(std::string arguments)
    {
        size_t spacePos = arguments.find(' ');

        if (spacePos != std::string::npos)
        {
            arguments = arguments.substr(0, spacePos);
        }

        return arguments;
    }
}