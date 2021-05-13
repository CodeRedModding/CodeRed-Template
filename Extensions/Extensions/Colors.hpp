#pragma once
#include "../pch.hpp"

struct CRColor
{
public:
	uint8_t R, G, B, A;
	struct FColor UnrealColor;

public:
	CRColor();
	CRColor(const uint8_t rgba);
	CRColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
	~CRColor();

public:
	struct FColor GetUnrealColor();

public:
	struct CRColor operator=(const struct FColor& other);
	bool operator==(const struct FColor& other);
	bool operator!=(const struct FColor& other);
};

struct CRLinearColor
{
public:
	float R, G, B, A;
	struct FLinearColor UnrealColor;

public:
	CRLinearColor();
	CRLinearColor(const float rgba);
	CRLinearColor(const float r, const float g, const float b, const float a);
	~CRLinearColor();

public:
	struct FLinearColor GetUnrealColor();

public:
	struct CRLinearColor operator=(const struct FLinearColor& other);
	bool operator==(const struct FLinearColor& other);
	bool operator!=(const struct FLinearColor& other);
};

// This is a global rainbow color class, hook your own function to the "Tick" function for it to update.
// This means you can sync up multiple objects to cycle through RGB at the same rate by reading from either "ByteRainbow" or "LinearRainbow".

class FRainbowColor
{
public:
	static inline struct CRColor ByteRainbow =			{ 0, 0, 255, 255 };
	static inline struct CRLinearColor LinearRainbow =	{ 0, 0, 255, 255 };

public:
	static void Tick();
};

// Taken directly from Unreal Engine 4 source code and converted to BGRA format instead of RGBA.
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Private/Math/ColorList.cpp

class FColorList
{
public:
	static inline struct FColor White =					{ 255, 255, 255, 255 };
	static inline struct FColor Red =					{ 0, 0, 255, 255 };
	static inline struct FColor Green =					{ 0, 255, 0, 255 };
	static inline struct FColor Blue =					{ 255, 0, 0, 255 };
	static inline struct FColor Magenta =				{ 255, 0, 255, 255 };
	static inline struct FColor Cyan =					{ 255, 255, 0, 255 };
	static inline struct FColor Yellow =				{ 0, 255, 255, 255 };
	static inline struct FColor Black =					{ 0, 0, 0, 255 };
	static inline struct FColor Aquamarine =			{ 147, 219, 112, 255 };
	static inline struct FColor BakerChocolate =		{ 23, 51, 92, 255 };
	static inline struct FColor BlueViolet =			{ 159, 95, 159, 255 };
	static inline struct FColor Brass =					{ 66, 166, 181, 255 };
	static inline struct FColor BrightGold =			{ 25, 217, 217, 255 };
	static inline struct FColor Brown =					{ 42, 42, 166, 255 };
	static inline struct FColor Bronze =				{ 83, 120, 140, 255 };
	static inline struct FColor BronzeII =				{ 61, 125, 166, 255 };
	static inline struct FColor CadetBlue =				{ 159, 159, 95, 255 };
	static inline struct FColor CoolCopper =			{ 25, 135, 217, 255 };
	static inline struct FColor Copper =				{ 51, 115, 184, 255 };
	static inline struct FColor Coral =					{ 0, 127, 255, 255 };
	static inline struct FColor CornFlowerBlue =		{ 111, 66, 66, 255 };
	static inline struct FColor DarkBrown =				{ 51, 64, 92, 255 };
	static inline struct FColor DarkGreen =				{ 47, 79, 47, 255 };
	static inline struct FColor DarkGreenCopper =		{ 110, 118, 74, 255 };
	static inline struct FColor DarkOliveGreen =		{ 47, 79, 79, 255 };
	static inline struct FColor DarkOrchid =			{ 205, 50, 153, 255 };
	static inline struct FColor DarkPurple =			{ 120, 31, 135, 255 };
	static inline struct FColor DarkSlateBlue =			{ 142, 35, 107, 255 };
	static inline struct FColor DarkSlateGrey =			{ 79, 79, 47, 255 };
	static inline struct FColor DarkTan =				{ 79, 105, 151, 255 };
	static inline struct FColor DarkTurquoise =			{ 219, 147, 112, 255 };
	static inline struct FColor DarkWood =				{ 66, 94, 133, 255 };
	static inline struct FColor DimGrey =				{ 84, 84, 84, 255 };
	static inline struct FColor DustyRose =				{ 99, 99, 133, 255 };
	static inline struct FColor Feldspar =				{ 117, 146, 209, 255 };
	static inline struct FColor Firebrick =				{ 35, 35, 142, 255 };
	static inline struct FColor ForestGreen =			{ 35, 142, 35, 255 };
	static inline struct FColor Gold =					{ 50, 127, 205, 255 };
	static inline struct FColor Goldenrod =				{ 112, 219, 219, 255 };
	static inline struct FColor Grey =					{ 192, 192, 192, 255 };
	static inline struct FColor GreenCopper =			{ 118, 127, 82, 255 };
	static inline struct FColor GreenYellow =			{ 112, 219, 147, 255 };
	static inline struct FColor HunterGreen =			{ 33, 94, 33, 255 };
	static inline struct FColor IndianRed =				{ 47, 47, 78, 255 };
	static inline struct FColor Khaki =					{ 95, 159, 159, 255 };
	static inline struct FColor LightBlue =				{ 217, 217, 192, 255 };
	static inline struct FColor LightGrey =				{ 168, 168, 168, 255 };
	static inline struct FColor LightSteelBlue =		{ 189, 143, 143, 255 };
	static inline struct FColor LightWood =				{ 166, 194, 233, 255 };
	static inline struct FColor LimeGreen =				{ 50, 205, 50, 255 };
	static inline struct FColor MandarianOrange =		{ 51, 120, 228, 255 };
	static inline struct FColor Maroon =				{ 107, 35, 142, 255 };
	static inline struct FColor MediumAquamarine =		{ 153, 205, 50, 255 };
	static inline struct FColor MediumBlue =			{ 205, 50, 50, 255 };
	static inline struct FColor MediumForestGreen =		{ 35, 142, 107, 255 };
	static inline struct FColor MediumGoldenrod  =		{ 174, 234, 234, 255 };
	static inline struct FColor MediumOrchid =			{ 219, 112, 147, 255 };
	static inline struct FColor MediumSeaGreen =		{ 66, 111, 66, 255 };
	static inline struct FColor MediumSlateBlue =		{ 255, 0, 127, 255 };
	static inline struct FColor MediumSpringGreen =		{ 0, 255, 127, 255 };
	static inline struct FColor MediumTurquoise =		{ 219, 219, 112, 255 };
	static inline struct FColor MediumVioletRed =		{ 147, 112, 219, 255 };
	static inline struct FColor MediumWood =			{ 100, 128, 166, 255 };
	static inline struct FColor MidnightBlue =			{ 79, 47, 47, 255 };
	static inline struct FColor NavyBlue =				{ 142, 35, 35, 255 };
	static inline struct FColor NeonBlue =				{ 255, 77, 77, 255 };
	static inline struct FColor NeonPink =				{ 199, 110, 255, 255 };
	static inline struct FColor NewMidnightBlue =		{ 156, 0, 0, 255 };
	static inline struct FColor NewTan =				{ 158, 199, 235, 255 };
	static inline struct FColor OldGold =				{ 59, 181, 207, 255 };
	static inline struct FColor Orange =				{ 0, 127, 255, 255 };
	static inline struct FColor OrangeRed =				{ 0, 36, 255, 255 };
	static inline struct FColor Orchid =				{ 219, 112, 219, 255 };
	static inline struct FColor PaleGreen =				{ 143, 188, 143, 255 };
	static inline struct FColor Pink =					{ 143, 143, 188, 255 };
	static inline struct FColor Plum =					{ 234, 173, 234, 255 };
	static inline struct FColor Quartz =				{ 243, 217, 217, 255 };
	static inline struct FColor RichBlue =				{ 171, 89, 89, 255 };
	static inline struct FColor Salmon =				{ 66, 66, 111, 255 };
	static inline struct FColor Scarlet =				{ 23, 23, 140, 255 };
	static inline struct FColor SeaGreen =				{ 104, 142, 35, 255 };
	static inline struct FColor SemiSweetChocolate  =	{ 38, 66, 107, 255 };
	static inline struct FColor Sienna =				{ 35, 107, 142, 255 };
	static inline struct FColor Silver =				{ 250, 232, 230, 255 };
	static inline struct FColor SkyBlue =				{ 204, 153, 50, 255 };
	static inline struct FColor SlateBlue =				{ 255, 127, 0, 255 };
	static inline struct FColor SpicyPink =				{ 174, 28, 255, 255 };
	static inline struct FColor SpringGreen =			{ 127, 255, 0, 255 };
	static inline struct FColor SteelBlue =				{ 142, 107, 35, 255 };
	static inline struct FColor SummerSky =				{ 222, 176, 56, 255 };
	static inline struct FColor Tan =					{ 112, 147, 219, 255 };
	static inline struct FColor Thistle =				{ 216, 191, 216, 255 };
	static inline struct FColor Turquoise =				{ 234, 234, 173, 255 };
	static inline struct FColor VeryDarkBrown =			{ 51, 64, 92, 255 };
	static inline struct FColor VeryLightGrey =			{ 205, 205, 205, 255 };
	static inline struct FColor Violet =				{ 79, 47, 79, 255 };
	static inline struct FColor VioletRed =				{ 153, 50, 204, 255 };
	static inline struct FColor Wheat =					{ 191, 216, 216, 255 };
	static inline struct FColor YellowGreen =			{ 50, 204, 153, 255 };
};