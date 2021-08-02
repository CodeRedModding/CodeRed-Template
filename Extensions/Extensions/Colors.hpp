#pragma once
#include "../pch.hpp"

struct Color
{
public:
	uint8_t R, G, B, A;

public:
	Color();
	Color(const uint8_t rgba);
	Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
	Color(const Color& other);
	Color(const struct FColor& other);
	~Color();

public:
	struct FColor UnrealColor() const;

public:
	Color operator=(const Color& other);
	Color operator=(const struct FColor& other);
	bool operator==(const Color& other);
	bool operator==(const struct FColor& other);
	bool operator!=(const Color& other);
	bool operator!=(const struct FColor& other);
};

struct LinearColor
{
public:
	float R, G, B, A;

public:
	LinearColor();
	LinearColor(const float rgba);
	LinearColor(const float r, const float g, const float b, const float a);
	LinearColor(const LinearColor& other);
	LinearColor(const struct FLinearColor& other);
	~LinearColor();

public:
	struct FLinearColor UnrealColor() const;

public:
	LinearColor operator=(const LinearColor& other);
	LinearColor operator=(const struct FLinearColor& other);
	bool operator==(const LinearColor& other);
	bool operator==(const struct FLinearColor& other);
	bool operator!=(const LinearColor& other);
	bool operator!=(const struct FLinearColor& other);
};

// This is a global rainbow color class, hook your own function to the "Tick" function for it to update.
// This means you can sync up multiple objects to cycle through RGB at the same rate.

class FRainbowColor
{
public:
	static inline Color ByteRainbow =					{ 0, 0, 255, 255 };
	static inline LinearColor LinearRainbow =			{ 0, 0, 255, 255 };

public:
	static Color GetByte();
	static LinearColor GetLinear();
	static void Tick();
};

// Taken directly from Unreal Engine 4 source code and converted to BGRA format instead of RGBA.
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Private/Math/ColorList.cpp

class FColorList
{
public:
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Magenta;
	static const Color Cyan;
	static const Color Yellow;
	static const Color Black;
	static const Color Aquamarine;
	static const Color BakerChocolate;
	static const Color BlueViolet;
	static const Color Brass;
	static const Color BrightGold;
	static const Color Brown;
	static const Color Bronze;
	static const Color BronzeII;
	static const Color CadetBlue;
	static const Color CoolCopper;
	static const Color Copper;
	static const Color Coral;
	static const Color CornFlowerBlue;
	static const Color DarkBrown;
	static const Color DarkGreen;
	static const Color DarkGreenCopper;
	static const Color DarkOliveGreen;
	static const Color DarkOrchid;
	static const Color DarkPurple;
	static const Color DarkSlateBlue;
	static const Color DarkSlateGrey;
	static const Color DarkTan;
	static const Color DarkTurquoise;
	static const Color DarkWood;
	static const Color DimGrey;
	static const Color DustyRose;
	static const Color Feldspar;
	static const Color Firebrick;
	static const Color ForestGreen;
	static const Color Gold;
	static const Color Goldenrod;
	static const Color Grey;
	static const Color GreenCopper;
	static const Color GreenYellow;
	static const Color HunterGreen;
	static const Color IndianRed;
	static const Color Khaki;
	static const Color LightBlue;
	static const Color LightGrey;
	static const Color LightSteelBlue;
	static const Color LightWood;
	static const Color LimeGreen;
	static const Color MandarianOrange;
	static const Color Maroon;
	static const Color MediumAquamarine;
	static const Color MediumBlue;
	static const Color MediumForestGreen;
	static const Color MediumGoldenrod ;
	static const Color MediumOrchid;
	static const Color MediumSeaGreen;
	static const Color MediumSlateBlue;
	static const Color MediumSpringGreen;
	static const Color MediumTurquoise;
	static const Color MediumVioletRed;
	static const Color MediumWood;
	static const Color MidnightBlue;
	static const Color NavyBlue;
	static const Color NeonBlue;
	static const Color NeonPink;
	static const Color NewMidnightBlue;
	static const Color NewTan;
	static const Color OldGold;
	static const Color Orange;
	static const Color OrangeRed;
	static const Color Orchid;
	static const Color PaleGreen;
	static const Color Pink;
	static const Color Plum;
	static const Color Quartz;
	static const Color RichBlue;
	static const Color Salmon;
	static const Color Scarlet;
	static const Color SeaGreen;
	static const Color SemiSweetChocolate ;
	static const Color Sienna;
	static const Color Silver;
	static const Color SkyBlue;
	static const Color SlateBlue;
	static const Color SpicyPink;
	static const Color SpringGreen;
	static const Color SteelBlue;
	static const Color SummerSky;
	static const Color Tan;
	static const Color Thistle;
	static const Color Turquoise;
	static const Color VeryDarkBrown;
	static const Color VeryLightGrey;
	static const Color Violet;
	static const Color VioletRed;
	static const Color Wheat;
	static const Color YellowGreen;
};

namespace Colors
{
	int32_t HexToDecimal(std::string hexString);
	std::string DecimalToHex(const int32_t decimal, const bool bInlcudeHead);
	Color DecimalToColor(const int32_t decimal);
	LinearColor DecimalToLinearColor(const int32_t decimal);
	Color LinearToColor(const LinearColor& linear);
	LinearColor ColorToLinear(const Color& color);
}