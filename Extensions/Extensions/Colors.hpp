#pragma once
#include "../pch.hpp"

struct Color
{
public:
	uint8_t R, G, B, A;

public:
	Color();
	explicit Color(uint8_t rgba);
	explicit Color(int32_t rgba);
	explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	explicit Color(int32_t r, int32_t g, int32_t b, int32_t a);
	explicit Color(float r, float g, float b, float a); // Auto converts linear values to byte values.
	Color(const std::string& hexColor);
	Color(const Color& other);
	Color(const struct FColor& other);
	~Color();

public:
	struct FColor UnrealColor() const;
	struct LinearColor ToLinear() const;
	uint32_t ToDecimal() const;
	std::string ToHex(bool bNotation = true) const;
	Color& FromDecimal(uint32_t decimalColor);
	Color& FromHex(std::string hexColor);

public:
	Color& operator=(const Color& other);
	Color& operator=(const struct FColor& other);
	bool operator==(const Color& other) const;
	bool operator==(const struct FColor& other) const;
	bool operator!=(const Color& other) const;
	bool operator!=(const struct FColor& other) const;
};

struct LinearColor
{
public:
	float R, G, B, A;

public:
	LinearColor();
	explicit LinearColor(float rgba);
	explicit LinearColor(float r, float g, float b, float a);
	explicit LinearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a); // Auto converts non-linear/bye values to linear.
	LinearColor(const std::string& hexColor);
	LinearColor(const LinearColor& other);
	LinearColor(const struct FLinearColor& other);
	~LinearColor();

public:
	struct FLinearColor UnrealColor() const;
	struct Color ToColor() const;
	uint32_t ToDecimal() const;
	std::string ToHex(bool bNotation = true) const;
	LinearColor& FromDecimal(uint32_t decimalColor);
	LinearColor& FromHex(std::string hexColor);

public:
	LinearColor& operator=(const LinearColor& other);
	LinearColor& operator=(const struct FLinearColor& other);
	bool operator==(const LinearColor& other) const;
	bool operator==(const struct FLinearColor& other) const;
	bool operator!=(const LinearColor& other) const;
	bool operator!=(const struct FLinearColor& other) const;
};

// This is a global rainbow color class, hook your own function to the "Tick" function for it to update.
// This means you can sync up multiple objects to cycle through RGB at the same rate.

class GRainbowColor
{
public:
	static inline Color ByteRainbow = Color(0, 0, 255, 255);
	static inline LinearColor LinearRainbow = LinearColor(0.0f, 0.0f, 1.0f, 1.0f);

public:
	static Color GetByte();
	static LinearColor GetLinear();
	static void Reset();
	static void OnTick();
};

// Helper functions for different color type conversions.
namespace Colors
{
	// Color to Decimal/Base10
	uint32_t HexToDecimal(std::string hexString);
	uint32_t ColorToDecimal(const Color& color);
	uint32_t LinearToDecimal(const LinearColor& linearColor);

	// Decimal/Base10 to Color
	Color DecimalToColor(uint32_t decimal);
	LinearColor DecimalToLinear(uint32_t decimal);

	// Color to Hexidecimal/Base16
	std::string DecimalToHex(uint32_t decimal, bool bNotation = true);
	std::string ColorToHex(const Color& color, bool bNotation = true);
	std::string LinearToHex(const LinearColor& linearColor, bool bNotation = true);

	// Hexidecimal/Base16 to Color
	Color HexToColor(std::string hexString);
	LinearColor HexToLinear(std::string hexString);

	// Direct Color Conversions
	Color LinearToColor(const LinearColor& linearColor);
	LinearColor ColorToLinear(const Color& color);
}

// Taken directly from Unreal Engine 4 source code and converted to BGRA format instead of RGBA.
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Private/Math/ColorList.cpp

class GColorList
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
	static const Color MediumGoldenrod;
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
	static const Color SemiSweetChocolate;
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