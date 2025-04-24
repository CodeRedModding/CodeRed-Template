#include "../pch.hpp"

namespace CodeRed::Memory
{
	std::string GetProcessName();
	uintptr_t GetEntryPoint();
	uintptr_t GetOffsetFromEntry(uintptr_t address);

	uintptr_t FindPattern(HMODULE hModule, const uint8_t* bytePattern, const std::string& maskStr, size_t offset = 0);
	uintptr_t FindPattern(const uint8_t* bytePattern, const std::string& maskStr, size_t offset = 0);
}