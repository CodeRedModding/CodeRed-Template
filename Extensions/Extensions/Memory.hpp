#include "../pch.hpp"
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace Memory
{
	uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask); // Half byte mask find pattern.
	void fstringcpy(struct FString& destination, struct FString& source); // For directly swapping out FStrings.
	void fnamecpy(struct FName& destination, const wchar_t* source); // For swapping out FNames that aren't in the GNames array, only use for debugging purposes.
}