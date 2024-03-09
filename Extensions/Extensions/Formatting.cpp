#include "Formatting.hpp"

namespace Format
{
    static char AlphabetCharacters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    static char DecimalCharacters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    static char HexCharacters[] = { 'a', 'b', 'c', 'd', 'e', 'f' };

    // String and character identifier functions.

    bool IsStringDecimal(std::string str)
    {
        if (str.empty())
        {
            return false;
        }

        ToLowerInline(str);

        bool first = true;
        bool negative = false;
        bool found = false;

        for (char c : str)
        {
            if (first)
            {
                first = false;
                negative = (c == '-');
            }

            if (std::isdigit(c))
            {
                found = true;
            }
            else if (!negative)
            {
                return false;
            }
        }

        return found;
    }

    bool IsStringAlphabet(std::string str)
    {
        if (str.empty())
        {
            return false;
        }

        ToLowerInline(str);
        bool found = false;

        for (char c : str)
        {
            if (std::isalpha(c))
            {
                found = true;
            }
            else
            {
                return false;
            }
        }

        return found;
    }

    bool IsStringHexadecimal(std::string str)
    {
        if (str.empty())
        {
            return false;
        }

        ToLowerInline(str);

        bool first = true;
        bool negative = false;
        bool found = false;

        for (char c : str)
        {
            if (first)
            {
                first = false;
                negative = (c == '-');
            }

            if (std::isxdigit(c))
            {
                found = true;
            }
            else if (!negative)
            {
                return false;
            }
        }

        return found;
    }

    bool IsStringFloat(std::string str)
    {
        if (str.empty())
        {
            return false;
        }

        ToLowerInline(str);

        bool first = true;
        bool negative = false;
        bool found = false;

        for (char c : str)
        {
            if (first)
            {
                first = false;
                negative = (c == '-');
            }

            if (std::isdigit(c))
            {
                found = true;
            }
            else if ((c != '.') && (c != 'f') && !negative)
            {
                return false;
            }
        }

        return found;
    }

    bool IsStringBase64(std::string str)
    {
        if (str.empty())
        {
            return false;
        }

        ToLowerInline(str);

        for (char c : str)
        {
            if (std::isalpha(c) || std::isdigit(c))
            {
                continue;
            }
            else if ((c != '+') && (c != '=') && (c != '/'))
            {
                return false;
            }
        }

        return true;
    }

    // String addon functions.

    bool Contains(std::string_view baseStr, std::string_view strToFind)
    {
        return (baseStr.find(strToFind) != std::string::npos);
    }

    std::string ToLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
        return str;
    }

    void ToLowerInline(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    std::string ToUpper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
        return str;
    }

    void ToUpperInline(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    std::string RemoveAllChars(std::string str, char character)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
        return str;
    }

    void RemoveAllCharsInline(std::string& str, char character)
    {
        str.erase(std::remove(str.begin(), str.end(), character), str.end());
    }

    std::string RemoveFileChars(std::string str)
    {
        RemoveAllCharsInline(str, '\\');
        RemoveAllCharsInline(str, '/');
        RemoveAllCharsInline(str, ':');
        RemoveAllCharsInline(str, '*');
        RemoveAllCharsInline(str, '?');
        RemoveAllCharsInline(str, '"');
        RemoveAllCharsInline(str, '<');
        RemoveAllCharsInline(str, '>');
        RemoveAllCharsInline(str, '|');
        return str;
    }

    std::string ReplaceAllChars(std::string str, char oldChar, char newChar)
    {
        for (char& c : str)
        {
            if (c == oldChar)
            {
                c = newChar;
            }
        }

        return str;
    }

    void ReplaceAllCharsInline(std::string& str, char oldChar, char newChar)
    {
        for (char& c : str)
        {
            if (c == oldChar)
            {
                c = newChar;
            }
        }
    }

    std::string ReplaceString(std::string baseStr, const std::string& strToReplace, const std::string& replaceWithStr)
    {
        if (!baseStr.empty() && !strToReplace.empty())
        {
            size_t replaceLength = strToReplace.length();
            size_t replacePos = baseStr.find(strToReplace);

            while (replacePos != std::string::npos)
            {
                baseStr.replace(replacePos, strToReplace.length(), replaceWithStr);
                replacePos = baseStr.find(strToReplace);
            }
        }

        return baseStr;
    }

    void ReplaceStringInline(std::string& baseStr, const std::string& strToReplace, const std::string& replaceWithStr)
    {
        baseStr = ReplaceString(baseStr, strToReplace, replaceWithStr);
    }

    std::string RemoveString(std::string baseStr, const std::string& strToRemove)
    {
        return ReplaceString(baseStr, strToRemove, "");
    }

    void RemoveStringInline(std::string& baseStr, const std::string& strToRemove)
    {
        baseStr = RemoveString(baseStr, strToRemove);
    }

    std::string ReverseString(std::string str)
    {
        if (!str.empty())
        {
            std::reverse(str.begin(), str.end());
        }

        return str;
    }

    void ReverseStringInline(std::string& str)
    {
        if (!str.empty())
        {
            std::reverse(str.begin(), str.end());
        }
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

    std::string ToHex(void* address, bool bNotation)
    {
        return ToHex(reinterpret_cast<uint64_t>(address), sizeof(uint64_t), bNotation);
    }

    std::string ToHex(uint64_t decimal, size_t width, bool bNotation)
    {
        std::ostringstream stream;
        if (bNotation) { stream << "0x"; }
        stream << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::hex << decimal;
        return stream.str();
    }

    uint64_t ToDecimal(const std::string& hexStr)
    {
        uint64_t decimal = 0;
        std::stringstream stream;
        stream << std::right << std::uppercase << std::hex << RemoveAllChars(hexStr, '#');
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

        if (precision > 0)
        {
            stream << std::fixed << std::showpoint << std::setprecision(precision) << value;
        }
        else
        {
            stream << static_cast<int32_t>(value);
        }

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

    // Helper Functions.

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
                if (!currentWord.empty())
                {
                    splitStrings.push_back(currentWord);
                }

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

        if (!currentWord.empty())
        {
            splitStrings.push_back(currentWord);
        }

        return splitStrings;
    }
}