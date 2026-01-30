#pragma once
#include <cstdint>

// Uncomment which hooking library you want to use, Detours or MinHook.
#define CR_DETOURS
//#define CR_MINHOOK

namespace CodeRed
{
	static constexpr uint8_t GLOBAL_OBJECT_SUB = 10; // Objects at the back of GObjects are highly volatile, may crash if you read the very last entry when iterating backwards, good to subtract a few as a buffer.
	static constexpr uint64_t GOLDEN_RATIO_MAGIC = 0x9E3779B9; // 0x9E3779B9;

	enum class EventTypes : uint8_t
	{
		Unknown,
		ProcessEvent,
		ProcessInternal,
		CallFunction
	};

	enum class ThreadTypes : uint8_t
	{
		Main,
		Render
	};

	enum class TextColors : uint32_t
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Aqua = 3,
		Red = 4,
		Purple = 5,
		Yellow = 6,
		White = 7,
		Grey = 8,
		LightBlue = 9,
		LightGreen = 10,
		LightAqua = 11,
		LightRed = 12,
		LightPurple = 13,
		LightYellow = 14,
		BrightWhite = 15,
		END
	};
}