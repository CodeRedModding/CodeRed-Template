#pragma once
#include "../../pch.hpp"

class Color
{
public:
	uint8_t R, G, B, A;

public:
	Color();
	explicit Color(uint8_t rgba);
	explicit Color(int32_t rgba);
	explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	explicit Color(int32_t r, int32_t g, int32_t b, int32_t a);
	explicit Color(float r, float g, float b, float a); // Auto converts linear color values to bytes.
	Color(const std::string& hexColor);
	Color(const FColor& color);
	Color(const Color& color);
	~Color();

public:
	FColor UnrealColor() const;
	class LinearColor ToLinear() const;
	uint32_t ToDecimal() const;
	uint32_t ToDecimalAlpha() const; // Same as "ToDecimal" but includes the alpha channel, supported here but may not be standard elsewhere.
	std::string ToHex(bool bNotation = true) const;
	std::string ToHexAlpha(bool bNotation = true) const; // Same as "ToHex" but includes the alpha channel, supported here but may not be standard elsewhere.
	Color& FromLinear(const LinearColor& linearColor);
	Color& FromDecimal(uint32_t decimalColor); // Supports both alpha and non alpha channels.
	Color& FromHex(std::string hexColor); // Supports both alpha and non alpha channels.
	Color& Cycle(int32_t steps = 1);

public:
	Color& operator=(const Color& other);
	Color& operator=(const FColor& other);
	bool operator==(const Color& other) const;
	bool operator==(const FColor& other) const;
	bool operator!=(const Color& other) const;
	bool operator!=(const FColor& other) const;
	bool operator<(const Color& other) const;
	bool operator>(const Color& other) const;
};

class LinearColor
{
public:
	float R, G, B, A;

public:
	LinearColor();
	explicit LinearColor(float rgba);
	explicit LinearColor(float r, float g, float b, float a);
	explicit LinearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a); // Auto converts byte color values to floats.
	LinearColor(const std::string& hexColor);
	LinearColor(const FLinearColor& linearColor);
	LinearColor(const LinearColor& linearColor);
	~LinearColor();

public:
	FLinearColor UnrealColor() const;
	Color ToColor() const;
	uint32_t ToDecimal() const;
	uint32_t ToDecimalAlpha() const; // Same as "ToDecimal" but includes the alpha channel, supported here but may not be standard elsewhere.
	std::string ToHex(bool bNotation = true) const;
	std::string ToHexAlpha(bool bNotation = true) const; // Same as "ToHex" but includes the alpha channel, supported here but may not be standard elsewhere.
	LinearColor& FromColor(const Color& color);
	LinearColor& FromDecimal(uint32_t decimalColor); // Supports both alpha and non alpha channels.
	LinearColor& FromHex(std::string hexColor); // Supports both alpha and non alpha channels.
	LinearColor& Cycle(int32_t steps = 1);

public:
	LinearColor& operator=(const LinearColor& other);
	LinearColor& operator=(const FLinearColor& other);
	bool operator==(const LinearColor& other) const;
	bool operator==(const FLinearColor& other) const;
	bool operator!=(const LinearColor& other) const;
	bool operator!=(const FLinearColor& other) const;
	bool operator<(const LinearColor& other) const;
	bool operator>(const LinearColor& other) const;
};

// This is a global rainbow color class, hook your own function to the "Tick" function for it to update.
// This means you can sync up multiple objects to cycle through RGB at the same rate.
class GRainbowColor
{
private:
	static inline Color ByteRainbow = Color(0, 0, 255, 255);
	static inline LinearColor LinearRainbow = LinearColor(0.0f, 0.0f, 1.0f, 1.0f);

public:
	static Color GetByte();
	static LinearColor GetLinear();
	static void Reset();
	static void OnTick(); // Managed by the "ManagerComponent::OnTick" function.
};

// Taken directly from Unreal Engine source code.
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
	static const Color MandarinOrange;
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

// Inline helper functions for different color type conversions.
namespace CodeRed::Colors
{
	// Color to Decimal/Base10

	inline uint32_t HexToDecimal(std::string hexStr) { return Color(hexStr).ToDecimal(); }
	inline uint32_t ColorToDecimal(const Color& color) { return color.ToDecimal(); }
	inline uint32_t LinearToDecimal(const LinearColor& linearColor) { return linearColor.ToDecimal(); }

	// Decimal/Base10 to Color

	inline Color DecimalToColor(uint32_t decimal) { return Color().FromDecimal(decimal); }
	inline LinearColor DecimalToLinear(uint32_t decimal) { return LinearColor().FromDecimal(decimal); }

	// Color to Hexidecimal/Base16

	inline std::string DecimalToHex(uint32_t decimal, bool bNotation = true) { return Color().FromDecimal(decimal).ToHex(bNotation); }
	inline std::string ColorToHex(const Color& color, bool bNotation = true) { return color.ToHex(bNotation); }
	inline std::string LinearToHex(const LinearColor& linearColor, bool bNotation = true) { return linearColor.ToHex(bNotation); }

	// Hexidecimal/Base16 to Color

	inline Color HexToColor(std::string hexStr) { return Color(hexStr); }
	inline LinearColor HexToLinear(std::string hexStr) { return LinearColor(hexStr); }

	// Direct Color Conversions

	inline Color LinearToColor(const LinearColor& linearColor) { return linearColor.ToColor(); }
	inline LinearColor ColorToLinear(const Color& color) { return color.ToLinear(); }
}

namespace std
{
	template<>
	struct hash<Color>
	{
		size_t operator()(const Color& other) const
		{
			return hash<string>()(other.ToHexAlpha(false));
		}
	};

	template<>
	struct hash<LinearColor>
	{
		size_t operator()(const LinearColor& other) const
		{
			return hash<string>()(other.ToHexAlpha(false));
		}
	};
}