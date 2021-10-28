#pragma once
#include "../pch.hpp"

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Public/GenericPlatform/GenericPlatformMemory.h

class FMemory
{
public:
	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return memmove(Dest,Src, Count);
	}

	static FORCEINLINE int32_t Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static FORCEINLINE void* Memset(void* Dest, uint8_t Char, SIZE_T Count)
	{
		return memset(Dest, Char, Count);
	}

	template<typename T> static FORCEINLINE void Memset(T& Src, uint8_t ValueToSet)
	{
		memset(&Src, ValueToSet, sizeof(T));
	}

	static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
	{
		return memset(Dest, 0, Count);
	}

	template<typename T> static FORCEINLINE void Memzero(T& Src)
	{
		memset(&Src, 0, sizeof(T));
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	template<typename T>  static FORCEINLINE void Memcpy(T& Dest, const T& Src)
	{
		memcpy(&Dest, &Src, sizeof(T));
	}

	static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* SystemMalloc(SIZE_T Size)
	{
		return ::malloc(Size);
	}

	static FORCEINLINE void SystemFree(void* Ptr)
	{
		::free(Ptr);
	}

private:
	template <typename T> static FORCEINLINE void Valswap(T& A, T& B)
	{
		T Tmp = A;
		A = B;
		B = Tmp;
	}
};