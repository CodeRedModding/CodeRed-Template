#pragma once
#include "../pch.hpp"
#include "Math.hpp"

// Helper functions for formatting streams and strings.
namespace Format
{
	// String and Character Functions.

	bool IsStringDecimal(std::string str);
	bool IsStringAlphabet(std::string str);
	bool IsStringHexadecimal(std::string str);
	bool IsStringFloat(std::string str);
	bool IsStringBase64(std::string str);

	// String Utils.

	bool Contains(std::string_view baseStr, std::string_view strToFind);
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

	std::string ToHex(void* address, bool bNotation = true);
	std::string ToHex(uint64_t decimal, size_t width, bool bNotation = true);
	uint64_t ToDecimal(const std::string& hexStr);
	std::string ToDecimal(uint64_t hex, size_t width);
	std::string Precision(float value, size_t precision);
	Rotator ToRotator(std::string_view str);
	VectorF ToVectorF(std::string_view str);
	VectorI ToVectorI(std::string_view str);
	Vector2DF ToVector2DF(std::string_view str);
	Vector2DI ToVector2DI(std::string_view str);

	// Helper Functions.

	bool StringSequenceMatches(std::string_view baseStr, std::string_view matchStr, size_t startPos);
	std::vector<std::string> Split(std::string_view str, char character);
	std::vector<std::string> SplitRange(std::string_view str, char from, char to, bool bIncludeChar);
}