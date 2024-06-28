#include "../pch.hpp"

namespace CodeRed::Memory
{
	uintptr_t GetEntryPoint();
	std::string GetProcessName();
	uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask); // Half byte mask find pattern.
}