#pragma once
#include "../pch.hpp"
#include "Math.hpp"

// Helper functions for formatting streams and strings.
namespace CodeRed::Format
{
	// String and Character Functions.

	bool IsStringDecimal(const std::string& str);
	bool IsStringAlphabet(const std::string& str);
	bool IsStringHexadecimal(const std::string& str);
	bool IsStringFloat(const std::string& str);
	bool IsStringBase64(const std::string& str);

	// String Utils.

	bool Contains(const std::string& baseStr, const std::string& strToFind);
	std::string ToLower(std::string str);
	void ToLowerInline(std::string& str);
	std::string ToUpper(std::string str);
	void ToUpperInline(std::string& str);
	std::string RemoveAllChars(std::string str, char character);
	void RemoveAllCharsInline(std::string& str, char character);
	std::string RemoveFileChars(std::string str);
	std::string ReplaceAllChars(std::string str, char oldChar, char newChar);
	void ReplaceAllCharsInline(std::string& str, char oldChar, char newChar);
	std::string ReplaceString(std::string baseStr, const std::string& strToReplace, const std::string& replaceWithStr);
	void ReplaceStringInline(std::string& baseStr, const std::string& strToReplace, const std::string& replaceWithStr);
	std::string RemoveString(std::string baseStr, const std::string& strToRemove);
	void RemoveStringInline(std::string& baseStr, const std::string& strToRemove);
	std::string ReverseString(std::string str);
	void ReverseStringInline(std::string& str);

	// Stream Utils.

	void Empty(std::ostringstream& stream);
	void FillRight(std::ostringstream& stream, char fill, size_t width);
	void FillLeft(std::ostringstream& stream, char fill, size_t width);
	void FillRight(std::ofstream& stream, char fill, size_t width);
	void FillLeft(std::ofstream& stream, char fill, size_t width);

	std::string ToHex(void* address, const std::string notation = "0x");
	std::string ToHex(uint64_t decimal, size_t width, const std::string notation = "0x");
	uint64_t ToDecimal(std::string hexStr);
	std::string ToDecimal(uint64_t hex, size_t width);
	std::string Precision(float value, size_t precision);
	Rotator ToRotator(const std::string& str);
	VectorF ToVectorF(const std::string& str);
	VectorI ToVectorI(const std::string& str);
	Vector2DF ToVector2DF(const std::string& str);
	Vector2DI ToVector2DI(const std::string& str);

	// Helper Functions.

	bool StringSequenceMatches(const std::string& baseStr, const std::string& matchStr, size_t startPos);
	std::vector<std::string> Split(const std::string& str, char character);
	std::vector<std::string> SplitRange(const std::string& str, char from, char to, bool bIncludeChar);
	template <typename T> void Shuffle(T& input)
	{
		if (!input.empty())
		{
			std::mt19937 random(std::random_device{}());
			std::shuffle(input.begin(), input.end(), random);
		}
	}

	// Random Generators.

	enum RandomFlags : uint32_t
	{
		RNG_None =				0UL,
		RNG_Numbers =			1UL << 0UL,
		RNG_LowerLetters =		1UL << 1UL,
		RNG_CapitalLetters =	1UL << 2UL,
		RNG_SpecialSymbols =	1UL << 3UL,
		RNG_All =				1UL << 4UL,
	};

	std::string RandomString(uint32_t flags, size_t length);
	char RandomCharacter(uint32_t flags);
}