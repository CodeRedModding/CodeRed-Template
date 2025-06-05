#pragma once
#include <cstdint>

// Uncomment which hooking library you want to use, Detours or MinHook.
#define CR_DETOURS
//#define CR_MINHOOK

namespace CodeRed
{
	static constexpr int32_t INSTANCES_INTERATE_OFFSET = 10;
	static constexpr uint64_t GOLDEN_RATIO_MAGIC = 0x9E3779B9;

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
		Render,
		Other
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