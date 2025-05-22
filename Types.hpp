#pragma once
#include <cstdint>

namespace CodeRed
{
	static constexpr int32_t INSTANCES_INTERATE_OFFSET = 10;

	enum EventTypes : uint8_t
	{
		Unknown,
		ProcessEvent,
		ProcessInternal,
		ProcessDelegate,
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