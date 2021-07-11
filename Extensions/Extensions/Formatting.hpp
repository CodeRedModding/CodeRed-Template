#pragma once
#include "../pch.hpp"

namespace Format
{
	void Empty(std::ostringstream& stream);
	void FillRight(std::ostringstream& stream, const char& fill, uint64_t width);
	void FillLeft(std::ostringstream& stream, const char& fill, uint64_t width);
	void FillRight(std::ofstream& stream, const char& fill, uint64_t width);
	void FillLeft(std::ofstream& stream, const char& fill, uint64_t width);
	std::string Hex(uintptr_t address, uint64_t width);
	std::string Decimal(uintptr_t address, uint64_t width);
	std::string Precision(float value, uint64_t precision);
}