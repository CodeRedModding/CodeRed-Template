#pragma once
#include "../pch.hpp"
#include "Math.hpp"

// Helper functions for formatting streams and strings.
namespace Format
{
	// String and character identifier  functions.

	bool IsCharAlphabet(char character);
	bool IsCharDecimal(char character);
	bool IsCharHexadecimal(char character);
	bool IsStringAlphabet(std::string_view str);
	bool IsStringDecimal(std::string_view str);
	bool IsStringHexadecimal(std::string_view str);
	bool IsStringFloat(std::string_view str);

	// String addon functions.

	std::string ToLower(std::string str);
	std::string ToUpper(std::string str);
	void RemoveAllChars(std::string& str, char character);
	std::string RemoveAllChars(const char character, std::string str);
	std::string RemoveFileChars(std::string str);
	std::string ReplaceString(std::string baseStr, const std::string& strToReplace, const std::string& replaceWithStr);

	// Stream addon functions.

	void Empty(std::ostringstream& stream);
	void FillRight(std::ostringstream& stream, char fill, size_t width);
	void FillLeft(std::ostringstream& stream, char fill, size_t width);
	void FillRight(std::ofstream& stream, char fill, size_t width);
	void FillLeft(std::ofstream& stream, char fill, size_t width);

	std::string ToHex(void* address, bool bHeader = true);
	std::string ToHex(uint64_t decimal, size_t width, bool bHeader = true);
	uint64_t ToDecimal(std::string hexStr);
	std::string ToDecimal(uint64_t hex, size_t width);
	std::string Precision(float value, size_t precision);
	Rotator ToRotator(std::string_view str);
	VectorF ToVectorF(std::string_view str);
	VectorI ToVectorI(std::string_view str);
	Vector2DF ToVector2DF(std::string_view str);
	Vector2DI ToVector2DI(std::string_view str);

	// Helper functions.

	bool StringSequenceMatches(std::string_view baseStr, std::string_view matchStr, size_t startPos);
	std::vector<std::string> Split(std::string_view str, char character);
	std::vector<std::string> SplitRange(std::string_view str, char from, char to, bool bIncludeChar);
}