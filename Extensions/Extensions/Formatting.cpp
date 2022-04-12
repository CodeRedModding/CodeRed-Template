#include "Formatting.hpp"

namespace Format
{
    static char AlphabetCharacters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    static char DecimalCharacters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    static char HexCharacters[] = { 'a', 'b', 'c', 'd', 'e', 'f' };

    // String and character identifier  functions.

    bool IsCharAlphabet(char character)
    {
        char lowChar = std::tolower(character);

        for (char alpha : AlphabetCharacters)
        {
            if (lowChar == alpha)
            {
                return true;
            }
        }

        return false;
    }

    bool IsCharDecimal(char character)
    {
        char lowChar = std::tolower(character);

        for (char dec : DecimalCharacters)
        {
            if (lowChar == dec || lowChar == '-')
            {
                return true;
            }
        }

        return false;
    }

    bool IsCharHexadecimal(char character)
    {
        char lowChar = std::tolower(character);

        for (char hex : HexCharacters)
        {
            if (lowChar == hex || lowChar == '-')
            {
                return true;
            }
        }

        return IsCharDecimal(character);
    }

    bool IsStringDecimal(std::string_view str)
    {
        for (char c : str)
        {
            if (!IsCharDecimal(c))
            {
                return false;
            }
        }

        return true;
    }

    bool IsStringAlphabet(std::string_view str)
    {
        for (char c : str)
        {
            if (!IsCharAlphabet(c))
            {
                return false;
            }
        }

        return true;
    }

    bool IsStringHexadecimal(std::string_view str)
    {
        for (char c : str)
        {
            if (!IsCharHexadecimal(c))
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
            if (!std::isdigit(c) && c != '-' && c != '.' && c != 'f')
            {
                return false;
            }
        }

        return true;
    }

    // String addon functions.

    std::string ToLower(std::string str)
    {
        for (char& c : str)
        {
            c = std::tolower(c);
        }

        return str;
    }

    std::string ToUpper(std::string str)
    {
        for (char& c : str)
        {
            c = std::toupper(c);
        }

        return str;
    }

    void RemoveAllChars(std::string& str, char character)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
    }

    std::string RemoveAllChars(const char character, std::string str)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
        return str;
    }

    std::string RemoveFileChars(std::string str)
    {
        RemoveAllChars(str, '\\');
        RemoveAllChars(str, '/');
        RemoveAllChars(str, ':');
        RemoveAllChars(str, '*');
        RemoveAllChars(str, '?');
        RemoveAllChars(str, '"');
        RemoveAllChars(str, '>');
        RemoveAllChars(str, '>');
        RemoveAllChars(str, '|');
        return str;
    }

    std::string ReplaceString(std::string baseStr, const std::string& strToReplace, const std::string& replaceWithStr)
    {
        size_t replaceLength = (strToReplace.length() - 1);

        while (baseStr.find(strToReplace) != std::string::npos)
        {
            size_t replacPos = baseStr.find(strToReplace);
            std::string rebuiltStr;

            size_t skipPos = 0;
            bool beingSkip = false;

            for (size_t i = 0; i < baseStr.length(); i++)
            {
                if (beingSkip && skipPos != replaceLength)
                {
                    skipPos++;
                    continue;
                }
                else if (beingSkip && skipPos == replaceLength)
                {
                    rebuiltStr += replaceWithStr;
                    skipPos = 0;
                    beingSkip = false;
                }
                else if (!beingSkip)
                {
                    if (i == replacPos)
                    {
                        skipPos++;
                        beingSkip = true;
                    }
                    else
                    {
                        rebuiltStr += baseStr[i];
                    }
                }
            }

            baseStr = rebuiltStr;
        }

        return baseStr;
    }

    // Stream addon functions.

    void Empty(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void FillRight(std::ostringstream& stream, char fill, size_t width)
    {
        stream << std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ostringstream& stream, char fill, size_t width)
    {
        stream << std::setfill(fill) << std::setw(width) << std::left;
    }

    void FillRight(std::ofstream& stream, char fill, size_t width)
    {
        stream << std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ofstream& stream, char fill, size_t width)
    {
        stream << std::setfill(fill) << std::setw(width) << std::left;
    }

    std::string ToHex(void* address, bool bHeader)
    {
        return ToHex(reinterpret_cast<uint64_t>(address), sizeof(address), bHeader);
    }

    std::string ToHex(uint64_t decimal, size_t width, bool bHeader)
    {
        std::ostringstream stream;
        if (bHeader) { stream << "0x"; }
        stream << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::hex << decimal;
        return stream.str();
    }

    uint64_t ToDecimal(std::string hexStr)
    {
        uint64_t decimal = 0;
        std::stringstream stream;
        stream << std::right << std::uppercase << std::hex << hexStr;
        stream >> decimal;
        return decimal;
    }

    std::string ToDecimal(uint64_t hex, size_t width)
    {
        std::ostringstream stream;
        stream << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::dec << hex;
        return stream.str();
    }

    std::string Precision(float value, size_t precision)
    {
        std::ostringstream stream;
        stream << std::setprecision(precision) << value;
        return stream.str();
    }

    Rotator ToRotator(std::string_view str)
    {
        Rotator returnRotator;
        std::vector<std::string> values = Split(str, ' ');

        if (values.size() >= 3)
        {
            if (IsStringDecimal(values[0]) && IsStringDecimal(values[1]) && IsStringDecimal(values[2]))
            {
                returnRotator.Pitch = std::stoi(values[0]);
                returnRotator.Yaw = std::stoi(values[1]);
                returnRotator.Roll = std::stoi(values[2]);
            }
        }

        return returnRotator;
    }

    VectorF ToVectorF(std::string_view str)
    {
        VectorF returnVector;
        std::vector<std::string> values = Split(str, ' ');

        if (values.size() == 2)
        {
            if (IsStringFloat(values[0]) && IsStringFloat(values[1]))
            {
                returnVector.X = std::stof(values[0]);
                returnVector.Y = std::stof(values[1]);
                returnVector.Z = 0.0f;
            }
        }
        else if (values.size() >= 3)
        {
            if (IsStringFloat(values[0]) && IsStringFloat(values[1]) && IsStringFloat(values[2]))
            {
                returnVector.X = std::stof(values[0]);
                returnVector.Y = std::stof(values[1]);
                returnVector.Z = std::stof(values[2]);
            }
        }

        return returnVector;
    }

    VectorI ToVectorI(std::string_view str)
    {
        VectorF floatVector = ToVectorF(str);
        return VectorI(static_cast<int32_t>(floatVector.X), static_cast<int32_t>(floatVector.Y), static_cast<int32_t>(floatVector.Z));
    }

    Vector2DF ToVector2DF(std::string_view str)
    {
        VectorF floatVector = ToVectorF(str);
        return Vector2DF(floatVector.X, floatVector.Y);
    }

    Vector2DI ToVector2DI(std::string_view str)
    {
        VectorI intVector = ToVectorI(str);
        return Vector2DI(intVector.X, intVector.Y);
    }

    bool StringSequenceMatches(std::string_view baseStr, std::string_view matchStr, size_t startPos)
    {
        size_t matches = 0;

        if ((baseStr.length() - startPos) + matchStr.length() <= baseStr.length())
        {
            for (size_t i = 0; i < matchStr.length(); i++)
            {
                if (baseStr[startPos + i] == matchStr[i])
                {
                    matches++;
                }
                else
                {
                    break;
                }
            }
        }

        return (matches == matchStr.length());
    }

    std::vector<std::string> Split(std::string_view str, char character)
    {
        std::vector<std::string> splitStrings;
        std::string currentWord;

        for (char c : str)
        {
            if (c == character)
            {
                splitStrings.push_back(currentWord);
                currentWord.clear();
            }
            else
            {
                currentWord += c;
            }
        }

        if (!currentWord.empty())
        {
            splitStrings.push_back(currentWord);
        }

        return splitStrings;
    }

    std::vector<std::string> SplitRange(std::string_view str, char from, char to, bool bIncludeChar)
    {
        std::vector<std::string> splitStrings;
        std::string currentWord;
        bool startWord = false;

        for (char c : str)
        {
            if (!startWord)
            {
                if (c == from)
                {
                    startWord = true;

                    if (bIncludeChar)
                    {
                        currentWord += c;
                    }
                }
            }
            else if (startWord)
            {
                if (c == to)
                {
                    startWord = false;

                    if (bIncludeChar)
                    {
                        currentWord += c;
                    }

                    splitStrings.push_back(currentWord);
                    currentWord.clear();
                }
                else
                {
                    currentWord += c;
                }
            }
        }

        return splitStrings;
    }
}