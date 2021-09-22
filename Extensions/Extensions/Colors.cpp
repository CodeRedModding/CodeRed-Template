#include "Colors.hpp"

Color::Color() : R(255), G(255), B(255), A(255) { }

Color::Color(uint8_t rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) { }

Color::Color(const Color& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

Color::Color(const struct FColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

Color::~Color() { }

struct FColor Color::UnrealColor() const
{
	return FColor { B, G, R, A }; // Your game might be in a different format (RGBA), so be aware of that.
}

Color Color::operator=(const Color& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

Color Color::operator=(const struct FColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool Color::operator==(const Color& other) const
{
	return (R == other.R && G == other.G && B == other.B && A == other.A);
}

bool Color::operator==(const struct FColor& other) const
{
	return (R == other.R && G == other.G && B == other.B && A == other.A);
}

bool Color::operator!=(const Color& other) const
{
	return (R != other.R && G != other.G && B != other.B && A != other.A);
}

bool Color::operator!=(const struct FColor& other) const
{
	return (R != other.R && G != other.G && B != other.B && A != other.A);
}

LinearColor::LinearColor() : R(1.f), G(1.f), B(1.f), A(1.f) { }

LinearColor::LinearColor(float rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

LinearColor::LinearColor(float r, float g, float b, float a) : R(r), G(g), B(b), A(a)
{
	Recalculate();
}

LinearColor::LinearColor(const LinearColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

LinearColor::LinearColor(const struct FLinearColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

LinearColor::~LinearColor() { }

struct FLinearColor LinearColor::UnrealColor() const
{
	return FLinearColor { R, G, B, A };
}

void LinearColor::Recalculate()
{
	if (R > 1.f) { R /= 255.f; }
	if (G > 1.f) { G /= 255.f; }
	if (B > 1.f) { B /= 255.f; }
	if (A > 1.f) { A /= 255.f; }
}


LinearColor LinearColor::operator=(const LinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

LinearColor LinearColor::operator=(const struct FLinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool LinearColor::operator==(const LinearColor& other) const
{
	return (R == other.R && G == other.G && B == other.B && A == other.A);
}

bool LinearColor::operator==(const struct FLinearColor& other) const
{
	return (R == other.R && G == other.G && B == other.B && A == other.A);
}

bool LinearColor::operator!=(const LinearColor& other) const
{
	return (R != other.R && G != other.G && B != other.B && A != other.A);
}

bool LinearColor::operator!=(const struct FLinearColor& other) const
{
	return (R != other.R && G != other.G && B != other.B && A != other.A);
}

Color FRainbowColor::GetByte()
{
	return ByteRainbow;
}

LinearColor FRainbowColor::GetLinear()
{
	return LinearRainbow;
}

void FRainbowColor::Tick()
{
	if (ByteRainbow.R == 255 && ByteRainbow.G < 255 && ByteRainbow.B == 0) // Green goes up to 255.
	{
		ByteRainbow.G += 1;
	}
	else if (ByteRainbow.R > 0 && ByteRainbow.G == 255 && ByteRainbow.B == 0) // Red goes down to 0.
	{
		ByteRainbow.R -= 1;
	}
	else if (ByteRainbow.R == 0 && ByteRainbow.G == 255 && ByteRainbow.B < 255) // Blue goes up to 255.
	{
		ByteRainbow.B += 1;
	}
	else if (ByteRainbow.R == 0 && ByteRainbow.G > 0 && ByteRainbow.B == 255) // Green goes down to 0.
	{
		ByteRainbow.G -= 1;
	}
	else if (ByteRainbow.R < 255 && ByteRainbow.G == 0 && ByteRainbow.B == 255) // Red goes up to 255.
	{
		ByteRainbow.R += 1;
	}
	else if (ByteRainbow.R == 255 && ByteRainbow.G == 0 && ByteRainbow.B > 0) // Blue goes down to 0.
	{
		ByteRainbow.B -= 1;
	}

	LinearRainbow = Colors::ColorToLinear(ByteRainbow);
}

const Color FColorList::White =					{ 255, 255, 255, 255 };
const Color FColorList::Red =					{ 255, 0, 0, 255 };
const Color FColorList::Green =					{ 0, 255, 0, 255 };
const Color FColorList::Blue =					{ 0, 0, 255, 255 };
const Color FColorList::Magenta =				{ 255, 0, 255, 255 };
const Color FColorList::Cyan =					{ 0, 255, 255, 255 };
const Color FColorList::Yellow =				{ 255, 255, 0, 255 };
const Color FColorList::Black =					{ 0, 0, 0, 255 };
const Color FColorList::Aquamarine =			{ 112, 219, 147, 255 };
const Color FColorList::BakerChocolate =		{ 92, 51, 23, 255 };
const Color FColorList::BlueViolet =			{ 159, 95, 159, 255 };
const Color FColorList::Brass =					{ 181, 166, 66, 255 };
const Color FColorList::BrightGold =			{ 217, 217, 25, 255 };
const Color FColorList::Brown =					{ 166, 42, 42, 255 };
const Color FColorList::Bronze =				{ 140, 120, 83, 255 };
const Color FColorList::BronzeII =				{ 166, 125, 61, 255 };
const Color FColorList::CadetBlue =				{ 95, 159, 159, 255 };
const Color FColorList::CoolCopper =			{ 217, 135, 25, 255 };
const Color FColorList::Copper =				{ 184, 115, 51, 255 };
const Color FColorList::Coral =					{ 255, 127, 0, 255 };
const Color FColorList::CornFlowerBlue =		{ 66, 66, 111, 255 };
const Color FColorList::DarkBrown =				{ 92, 64, 51, 255 };
const Color FColorList::DarkGreen =				{ 47, 79, 47, 255 };
const Color FColorList::DarkGreenCopper =		{ 74, 118, 110, 255 };
const Color FColorList::DarkOliveGreen =		{ 79, 79, 47, 255 };
const Color FColorList::DarkOrchid =			{ 153, 50, 205, 255 };
const Color FColorList::DarkPurple =			{ 135, 31, 120, 255 };
const Color FColorList::DarkSlateBlue =			{ 107, 35, 142, 255 };
const Color FColorList::DarkSlateGrey =			{ 47, 79, 79, 255 };
const Color FColorList::DarkTan =				{ 151, 105, 79, 255 };
const Color FColorList::DarkTurquoise =			{ 112, 147, 219, 255 };
const Color FColorList::DarkWood =				{ 133, 94, 66, 255 };
const Color FColorList::DimGrey =				{ 84, 84, 84, 255 };
const Color FColorList::DustyRose =				{ 133, 99, 99, 255 };
const Color FColorList::Feldspar =				{ 209, 146, 117, 255 };
const Color FColorList::Firebrick =				{ 142, 35, 35, 255 };
const Color FColorList::ForestGreen =			{ 35, 142, 35, 255 };
const Color FColorList::Gold =					{ 205, 127, 50, 255 };
const Color FColorList::Goldenrod =				{ 219, 219, 112, 255 };
const Color FColorList::Grey =					{ 192, 192, 192, 255 };
const Color FColorList::GreenCopper =			{ 82, 127, 118, 255 };
const Color FColorList::GreenYellow =			{ 147, 219, 112, 255 };
const Color FColorList::HunterGreen =			{ 33, 94, 33, 255 };
const Color FColorList::IndianRed =				{ 78, 47, 47, 255 };
const Color FColorList::Khaki =					{ 159, 159, 95, 255 };
const Color FColorList::LightBlue =				{ 192, 217, 217, 255 };
const Color FColorList::LightGrey =				{ 168, 168, 168, 255 };
const Color FColorList::LightSteelBlue =		{ 143, 143, 189, 255 };
const Color FColorList::LightWood =				{ 233, 194, 166, 255 };
const Color FColorList::LimeGreen =				{ 50, 205, 50, 255 };
const Color FColorList::MandarianOrange =		{ 228, 120, 51, 255 };
const Color FColorList::Maroon =				{ 142, 35, 107, 255 };
const Color FColorList::MediumAquamarine =		{ 50, 205, 153, 255 };
const Color FColorList::MediumBlue =			{ 50, 50, 205, 255 };
const Color FColorList::MediumForestGreen =		{ 107, 142, 35, 255 };
const Color FColorList::MediumGoldenrod  =		{ 234, 234, 174, 255 };
const Color FColorList::MediumOrchid =			{ 147, 112, 219, 255 };
const Color FColorList::MediumSeaGreen =		{ 66, 111, 66, 255 };
const Color FColorList::MediumSlateBlue =		{ 127, 0, 255, 255 };
const Color FColorList::MediumSpringGreen =		{ 127, 255, 0, 255 };
const Color FColorList::MediumTurquoise =		{ 112, 219, 219, 255 };
const Color FColorList::MediumVioletRed =		{ 219, 112, 147, 255 };
const Color FColorList::MediumWood =			{ 166, 128, 100, 255 };
const Color FColorList::MidnightBlue =			{ 47, 47, 79, 255 };
const Color FColorList::NavyBlue =				{ 35, 35, 142, 255 };
const Color FColorList::NeonBlue =				{ 77, 77, 255, 255 };
const Color FColorList::NeonPink =				{ 255, 110, 199, 255 };
const Color FColorList::NewMidnightBlue =		{ 0, 0, 156, 255 };
const Color FColorList::NewTan =				{ 235, 199, 158, 255 };
const Color FColorList::OldGold =				{ 207, 181, 59, 255 };
const Color FColorList::Orange =				{ 255, 127, 0, 255 };
const Color FColorList::OrangeRed =				{ 255, 36, 0, 255 };
const Color FColorList::Orchid =				{ 219, 112, 219, 255 };
const Color FColorList::PaleGreen =				{ 143, 188, 143, 255 };
const Color FColorList::Pink =					{ 188, 143, 143, 255 };
const Color FColorList::Plum =					{ 234, 173, 234, 255 };
const Color FColorList::Quartz =				{ 217, 217, 243, 255 };
const Color FColorList::RichBlue =				{ 89, 89, 171, 255 };
const Color FColorList::Salmon =				{ 111, 66, 66, 255 };
const Color FColorList::Scarlet =				{ 140, 23, 23, 255 };
const Color FColorList::SeaGreen =				{ 35, 142, 104, 255 };
const Color FColorList::SemiSweetChocolate  =	{ 107, 66, 38, 255 };
const Color FColorList::Sienna =				{ 142, 107, 35, 255 };
const Color FColorList::Silver =				{ 230, 232, 250, 255 };
const Color FColorList::SkyBlue =				{ 50, 153, 204, 255 };
const Color FColorList::SlateBlue =				{ 0, 127, 255, 255 };
const Color FColorList::SpicyPink =				{ 255, 28, 174, 255 };
const Color FColorList::SpringGreen =			{ 0, 255, 127, 255 };
const Color FColorList::SteelBlue =				{ 35, 107, 142, 255 };
const Color FColorList::SummerSky =				{ 56, 176, 222, 255 };
const Color FColorList::Tan =					{ 219, 147, 112, 255 };
const Color FColorList::Thistle =				{ 216, 191, 216, 255 };
const Color FColorList::Turquoise =				{ 173, 234, 234, 255 };
const Color FColorList::VeryDarkBrown =			{ 92, 64, 51, 255 };
const Color FColorList::VeryLightGrey =			{ 205, 205, 205, 255 };
const Color FColorList::Violet =				{ 79, 47, 79, 255 };
const Color FColorList::VioletRed =				{ 204, 50, 153, 255 };
const Color FColorList::Wheat =					{ 216, 216, 191, 255 };
const Color FColorList::YellowGreen =			{ 153, 204, 50, 255 };

namespace Colors
{
	int32_t HexToDecimal(std::string hexString)
	{
		int32_t returnDecimal = 0;

		if (hexString.find("#") == 0)
		{
			hexString = hexString.erase(0, 1);
		}

		std::stringstream ss;
		ss << std::hex << hexString;
		ss >> returnDecimal;

		return returnDecimal;
	}

	std::string DecimalToHex(const int32_t decimal, const bool bInlcudeHead)
	{
		std::string hexDigits = "0123456789ABCDEF";
		std::string hexColor;

		for (int32_t i = (3 * 2) - 1; i >= 0; i--)
		{
			hexColor += hexDigits[((decimal >> i * 4) & 0xF)];
		}

		if (bInlcudeHead)
		{
			hexColor = "#" + hexColor;
		}

		return hexColor;
	}

	Color DecimalToColor(const int32_t decimal)
	{
		Color returnColor;

		returnColor.R = (decimal >> 16) & 0xFF;
		returnColor.G = (decimal >> 8) & 0xFF;
		returnColor.B = (decimal) & 0xFF;
		returnColor.A = 255;

		return returnColor;
	}

	LinearColor DecimalToLinearColor(const int32_t decimal)
	{
		LinearColor returnColor;

		returnColor.R = (decimal >> 16) & 0xFF;
		returnColor.G = (decimal >> 8) & 0xFF;
		returnColor.B = (decimal) & 0xFF;
		returnColor.A = 255.f;

		returnColor.R /= 255.f;
		returnColor.G /= 255.f;
		returnColor.B /= 255.f;
		returnColor.A /= 255.f;

		return returnColor;
	}

	Color LinearToColor(const LinearColor& linear)
	{
		Color returnValue;

		returnValue.R = linear.R * 255.f;
		returnValue.G = linear.G * 255.f;
		returnValue.B = linear.B * 255.f;
		returnValue.A = linear.A * 255.f;

		return returnValue;
	}

	LinearColor ColorToLinear(const Color& color)
	{
		LinearColor returnValue;

		returnValue.R = color.R / 255.f;
		returnValue.G = color.G / 255.f;
		returnValue.B = color.B / 255.f;
		returnValue.A = color.A / 255.f;

		return returnValue;
	}
}