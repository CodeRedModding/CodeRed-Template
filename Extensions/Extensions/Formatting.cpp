#include "Formatting.hpp"

namespace CodeRed::Format
{
    // String and character identifier functions.

    bool IsStringDecimal(const std::string& str)
    {
        bool foundAny = false;

        if (!str.empty())
        {
            bool first = true;
            bool negative = false;

            for (char c : str)
            {
                if (first)
                {
                    first = false;
                    negative = (c == '-');
                }

                if (std::isdigit(c))
                {
                    foundAny = true;
                }
                else if (!negative)
                {
                    return false;
                }
            }
        }

        return foundAny;
    }

    bool IsStringAlphabet(const std::string& str)
    {
        bool foundAny = false;

        if (!str.empty())
        {
            for (char c : str)
            {
                if (std::isalpha(c))
                {
                    foundAny = true;
                }
                else
                {
                    return false;
                }
            }
        }

        return foundAny;
    }

    bool IsStringHexadecimal(const std::string& str)
    {
        bool foundAny = false;

        if (!str.empty())
        {
            bool first = true;
            bool negative = false;

            for (char c : str)
            {
                if (first)
                {
                    first = false;
                    negative = (c == '-');
                }

                if (std::isxdigit(c))
                {
                    foundAny = true;
                }
                else if (!negative)
                {
                    return false;
                }
            }
        }

        return foundAny;
    }

    bool IsStringFloat(const std::string& str)
    {
        bool foundAny = false;

        if (!str.empty())
        {
            bool first = true;
            bool negative = false;

            for (char c : str)
            {
                if (first)
                {
                    first = false;
                    negative = (c == '-');
                }

                if (std::isdigit(c))
                {
                    foundAny = true;
                }
                else if ((c != '.') && (c != 'f') && !negative)
                {
                    return false;
                }
            }
        }

        return foundAny;
    }

    bool IsStringBase64(const std::string& str)
    {
        if (!str.empty())
        {
            for (char c : str)
            {
                if ((!std::isalpha(c) && !std::isdigit(c)) && ((c != '+') && (c != '=') && (c != '/') && (c != '.') && (c != '_') && (c != '-')))
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    // String Utils

    bool Contains(const std::string& baseStr, const std::string& strToFind)
    {
        return (baseStr.find(strToFind) != std::string::npos);
    }

    std::string ToLower(std::string str)
    {
        ToLowerInline(str);
        return str;
    }

    void ToLowerInline(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    std::string ToUpper(std::string str)
    {
        ToUpperInline(str);
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
        if (!str.empty())
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
        }

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
            std::string result;
            result.reserve(baseStr.size());

            size_t pos = 0;
            size_t start = 0;

            while ((pos = baseStr.find(strToReplace, start)) != std::string::npos)
            {
                result.append(baseStr, start, (pos - start));
                result += replaceWithStr;
                start = (pos + strToReplace.length());
            }

            return result.append(baseStr, start);
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

    // Stream Utils.

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

    std::string ToHex(void* address, const std::string notation)
    {
        return ToHex(reinterpret_cast<uint64_t>(address), sizeof(uint64_t), notation);
    }

    std::string ToHex(uint64_t decimal, size_t width, const std::string notation)
    {
        std::ostringstream stream;
        stream << notation << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::hex << decimal;
        return stream.str();
    }

    uint64_t ToDecimal(std::string hexStr)
    {
        RemoveStringInline(hexStr, "0x");
        RemoveAllCharsInline(hexStr, '#');

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

    Rotator ToRotator(const std::string& str)
    {
        Rotator returnRotator;

        if (!str.empty())
        {
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
        }

        return returnRotator;
    }

    VectorF ToVectorF(const std::string& str)
    {
        VectorF returnVector;

        if (!str.empty())
        {
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
        }

        return returnVector;
    }

    VectorI ToVectorI(const std::string& str)
    {
        VectorF floatVector = ToVectorF(str);
        return VectorI(static_cast<int32_t>(floatVector.X), static_cast<int32_t>(floatVector.Y), static_cast<int32_t>(floatVector.Z));
    }

    Vector2DF ToVector2DF(const std::string& str)
    {
        VectorF floatVector = ToVectorF(str);
        return Vector2DF(floatVector.X, floatVector.Y);
    }

    Vector2DI ToVector2DI(const std::string& str)
    {
        VectorI intVector = ToVectorI(str);
        return Vector2DI(intVector.X, intVector.Y);
    }

    // Helper Functions.

    bool StringSequenceMatches(const std::string& baseStr, const std::string& matchStr, size_t startPos)
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

    std::vector<std::string> Split(const std::string& str, char character)
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

    std::vector<std::string> SplitRange(const std::string& str, char from, char to, bool bIncludeChar)
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

    // Random Generators.

    std::string GetCharacterSet(uint32_t flags)
    {
        static std::string RNG_NUMBERS_CHARS = "0123456789";
        static std::string RNG_LETTERS_CHARS = "abcdefghijklmnopqrstuvwxyz";
        static std::string RNG_SYMBOLS_CHARS = "!#$%&'()*+,-./:;<=>?@[\]^_`{|}~";
        std::string characterSet;

        if ((flags & RandomFlags::RNG_Numbers) || (flags & RandomFlags::RNG_All))
        {
            characterSet += RNG_NUMBERS_CHARS;
        }

        if ((flags & RandomFlags::RNG_LowerLetters) || (flags & RandomFlags::RNG_All))
        {
            characterSet += RNG_LETTERS_CHARS;
        }

        if ((flags & RandomFlags::RNG_CapitalLetters) || (flags & RandomFlags::RNG_All))
        {
            characterSet += Format::ToUpper(RNG_LETTERS_CHARS);
        }

        if ((flags & RandomFlags::RNG_SpecialSymbols) || (flags & RandomFlags::RNG_All))
        {
            characterSet += RNG_SYMBOLS_CHARS;
        }

        Shuffle<std::string>(characterSet);
        return characterSet;
    }

    std::string RandomString(uint32_t flags, size_t length)
    {
        std::string randomStr;

        if (length > 0)
        {
            std::string characterSet = GetCharacterSet(flags);

            if (!characterSet.empty())
            {
                for (size_t i = 0; i < length; i++)
                {
                    randomStr += characterSet[Math::RandomRange64<size_t>(0, (characterSet.length() - 1))];
                }
            }
        }

        return randomStr;
    }

    char RandomCharacter(uint32_t flags)
    {
        std::string characterSet = GetCharacterSet(flags);

        if (!characterSet.empty())
        {
            if (characterSet.size() > 1)
            {
                return characterSet[Math::RandomRange64<size_t>(0, (characterSet.length() - 1))];
            }
            
            return characterSet[0];
        }

        return NULL;
    }
}