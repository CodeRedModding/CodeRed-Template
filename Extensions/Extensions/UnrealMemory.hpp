#pragma once
#include "../pch.hpp"

namespace FMemory
{
	void* Memmove(void* Dest, const void* Src, size_t Count);
	int32_t Memcmp(const void* Buf1, const void* Buf2, size_t Count);
	void* Memset(void* Dest, uint8_t Char, size_t Count);
	template<typename T>  void Memset(T& Src, uint8_t ValueToSet)
	{
		Memset(&Src, ValueToSet, sizeof(T));
	}
	void* Memzero(void* Dest, size_t Count);
	template<typename T> void Memzero(T& Src)
	{
		Memzero(&Src, sizeof(T));
	}
	void* Memcpy(void* Dest, const void* Src, size_t Count);
	template<typename T> void Memcpy(T& Dest, const T& Src)
	{
		Memcpy(&Dest, &Src, sizeof(T));
	}
	void* BigBlockMemcpy(void* Dest, const void* Src, size_t Count);
	void* StreamingMemcpy(void* Dest, const void* Src, size_t Count);
	void* SystemMalloc(size_t Size);
	void SystemFree(void* Ptr);
	void* MallocZeroed(size_t Count, uint32_t Alignment = 0);
}