#include "Colors.hpp"

Color::Color() :
	R(255),
	G(255),
	B(255),
	A(255)
{

}

Color::Color(uint8_t rgba) :
	R(rgba),
	G(rgba),
	B(rgba),
	A(rgba)
{

}

Color::Color(int32_t rgba) :
	R(rgba),
	G(rgba),
	B(rgba),
	A(rgba)
{

}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
	R(r),
	G(g),
	B(b),
	A(a)
{

}

Color::Color(int32_t r, int32_t g, int32_t b, int32_t a) :
	R(r),
	G(g),
	B(b),
	A(a)
{

}

Color::Color(float r, float g, float b, float a) :
	R(r * 255.0f),
	G(g * 255.0f),
	B(b * 255.0f),
	A(a * 255.0f)
{

}

Color::Color(const Color& other) :
	R(other.R),
	G(other.G),
	B(other.B),
	A(other.A)
{

}

Color::Color(const struct FColor& other) :
	R(other.R),
	G(other.G),
	B(other.B),
	A(other.A)
{

}

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

LinearColor::LinearColor() :
	R(1.0f),
	G(1.0f),
	B(1.0f),
	A(1.0f)
{

}

LinearColor::LinearColor(float rgba) :
	R(rgba),
	G(rgba),
	B(rgba),
	A(rgba)
{

}

LinearColor::LinearColor(float r, float g, float b, float a) :
	R(r),
	G(g),
	B(b),
	A(a)
{

}

LinearColor::LinearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
	R(r / 255.0f),
	G(g / 255.0f),
	B(b / 255.0f),
	A(a / 255.0f)
{

}

LinearColor::LinearColor(const LinearColor& other) :
	R(other.R),
	G(other.G),
	B(other.B),
	A(other.A)
{

}

LinearColor::LinearColor(const struct FLinearColor& other) :
	R(other.R),
	G(other.G),
	B(other.B),
	A(other.A)
{

}

LinearColor::~LinearColor() { }

struct FLinearColor LinearColor::UnrealColor() const
{
	return FLinearColor { R, G, B, A };
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

void FRainbowColor::OnTick()
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

const Color FColorList::White =					Color( 255, 255, 255, 255);
const Color FColorList::Red =					Color( 255, 0, 0, 255 );
const Color FColorList::Green =					Color( 0, 255, 0, 255 );
const Color FColorList::Blue =					Color( 0, 0, 255, 255 );
const Color FColorList::Magenta =				Color( 255, 0, 255, 255 );
const Color FColorList::Cyan =					Color( 0, 255, 255, 255 );
const Color FColorList::Yellow =				Color( 255, 255, 0, 255 );
const Color FColorList::Black =					Color( 0, 0, 0, 255 );
const Color FColorList::Aquamarine =			Color( 112, 219, 147, 255 );
const Color FColorList::BakerChocolate =		Color( 92, 51, 23, 255 );
const Color FColorList::BlueViolet =			Color( 159, 95, 159, 255 );
const Color FColorList::Brass =					Color( 181, 166, 66, 255 );
const Color FColorList::BrightGold =			Color( 217, 217, 25, 255 );
const Color FColorList::Brown =					Color( 166, 42, 42, 255 );
const Color FColorList::Bronze =				Color( 140, 120, 83, 255 );
const Color FColorList::BronzeII =				Color( 166, 125, 61, 255 );
const Color FColorList::CadetBlue =				Color( 95, 159, 159, 255 );
const Color FColorList::CoolCopper =			Color( 217, 135, 25, 255 );
const Color FColorList::Copper =				Color( 184, 115, 51, 255 );
const Color FColorList::Coral =					Color( 255, 127, 0, 255 );
const Color FColorList::CornFlowerBlue =		Color( 66, 66, 111, 255 );
const Color FColorList::DarkBrown =				Color( 92, 64, 51, 255 );
const Color FColorList::DarkGreen =				Color( 47, 79, 47, 255 );
const Color FColorList::DarkGreenCopper =		Color( 74, 118, 110, 255 );
const Color FColorList::DarkOliveGreen =		Color( 79, 79, 47, 255 );
const Color FColorList::DarkOrchid =			Color( 153, 50, 205, 255 );
const Color FColorList::DarkPurple =			Color( 135, 31, 120, 255 );
const Color FColorList::DarkSlateBlue =			Color( 107, 35, 142, 255 );
const Color FColorList::DarkSlateGrey =			Color( 47, 79, 79, 255 );
const Color FColorList::DarkTan =				Color( 151, 105, 79, 255 );
const Color FColorList::DarkTurquoise =			Color( 112, 147, 219, 255 );
const Color FColorList::DarkWood =				Color( 133, 94, 66, 255 );
const Color FColorList::DimGrey =				Color( 84, 84, 84, 255 );
const Color FColorList::DustyRose =				Color( 133, 99, 99, 255 );
const Color FColorList::Feldspar =				Color( 209, 146, 117, 255 );
const Color FColorList::Firebrick =				Color( 142, 35, 35, 255 );
const Color FColorList::ForestGreen =			Color( 35, 142, 35, 255 );
const Color FColorList::Gold =					Color( 205, 127, 50, 255 );
const Color FColorList::Goldenrod =				Color( 219, 219, 112, 255 );
const Color FColorList::Grey =					Color( 192, 192, 192, 255 );
const Color FColorList::GreenCopper =			Color( 82, 127, 118, 255 );
const Color FColorList::GreenYellow =			Color( 147, 219, 112, 255 );
const Color FColorList::HunterGreen =			Color( 33, 94, 33, 255 );
const Color FColorList::IndianRed =				Color( 78, 47, 47, 255 );
const Color FColorList::Khaki =					Color( 159, 159, 95, 255 );
const Color FColorList::LightBlue =				Color( 192, 217, 217, 255 );
const Color FColorList::LightGrey =				Color( 168, 168, 168, 255 );
const Color FColorList::LightSteelBlue =		Color( 143, 143, 189, 255 );
const Color FColorList::LightWood =				Color( 233, 194, 166, 255 );
const Color FColorList::LimeGreen =				Color( 50, 205, 50, 255 );
const Color FColorList::MandarianOrange =		Color( 228, 120, 51, 255 );
const Color FColorList::Maroon =				Color( 142, 35, 107, 255 );
const Color FColorList::MediumAquamarine =		Color( 50, 205, 153, 255 );
const Color FColorList::MediumBlue =			Color( 50, 50, 205, 255 );
const Color FColorList::MediumForestGreen =		Color( 107, 142, 35, 255 );
const Color FColorList::MediumGoldenrod  =		Color( 234, 234, 174, 255 );
const Color FColorList::MediumOrchid =			Color( 147, 112, 219, 255 );
const Color FColorList::MediumSeaGreen =		Color( 66, 111, 66, 255 );
const Color FColorList::MediumSlateBlue =		Color( 127, 0, 255, 255 );
const Color FColorList::MediumSpringGreen =		Color( 127, 255, 0, 255 );
const Color FColorList::MediumTurquoise =		Color( 112, 219, 219, 255 );
const Color FColorList::MediumVioletRed =		Color( 219, 112, 147, 255 );
const Color FColorList::MediumWood =			Color( 166, 128, 100, 255 );
const Color FColorList::MidnightBlue =			Color( 47, 47, 79, 255 );
const Color FColorList::NavyBlue =				Color( 35, 35, 142, 255 );
const Color FColorList::NeonBlue =				Color( 77, 77, 255, 255 );
const Color FColorList::NeonPink =				Color( 255, 110, 199, 255 );
const Color FColorList::NewMidnightBlue =		Color( 0, 0, 156, 255 );
const Color FColorList::NewTan =				Color( 235, 199, 158, 255 );
const Color FColorList::OldGold =				Color( 207, 181, 59, 255 );
const Color FColorList::Orange =				Color( 255, 127, 0, 255 );
const Color FColorList::OrangeRed =				Color( 255, 36, 0, 255 );
const Color FColorList::Orchid =				Color( 219, 112, 219, 255 );
const Color FColorList::PaleGreen =				Color( 143, 188, 143, 255 );
const Color FColorList::Pink =					Color( 188, 143, 143, 255 );
const Color FColorList::Plum =					Color( 234, 173, 234, 255 );
const Color FColorList::Quartz =				Color( 217, 217, 243, 255 );
const Color FColorList::RichBlue =				Color( 89, 89, 171, 255 );
const Color FColorList::Salmon =				Color( 111, 66, 66, 255 );
const Color FColorList::Scarlet =				Color( 140, 23, 23, 255 );
const Color FColorList::SeaGreen =				Color( 35, 142, 104, 255 );
const Color FColorList::SemiSweetChocolate  =	Color( 107, 66, 38, 255 );
const Color FColorList::Sienna =				Color( 142, 107, 35, 255 );
const Color FColorList::Silver =				Color( 230, 232, 250, 255 );
const Color FColorList::SkyBlue =				Color( 50, 153, 204, 255 );
const Color FColorList::SlateBlue =				Color( 0, 127, 255, 255 );
const Color FColorList::SpicyPink =				Color( 255, 28, 174, 255 );
const Color FColorList::SpringGreen =			Color( 0, 255, 127, 255 );
const Color FColorList::SteelBlue =				Color( 35, 107, 142, 255 );
const Color FColorList::SummerSky =				Color( 56, 176, 222, 255 );
const Color FColorList::Tan =					Color( 219, 147, 112, 255 );
const Color FColorList::Thistle =				Color( 216, 191, 216, 255 );
const Color FColorList::Turquoise =				Color( 173, 234, 234, 255 );
const Color FColorList::VeryDarkBrown =			Color( 92, 64, 51, 255 );
const Color FColorList::VeryLightGrey =			Color( 205, 205, 205, 255 );
const Color FColorList::Violet =				Color( 79, 47, 79, 255 );
const Color FColorList::VioletRed =				Color( 204, 50, 153, 255 );
const Color FColorList::Wheat =					Color( 216, 216, 191, 255 );
const Color FColorList::YellowGreen =			Color( 153, 204, 50, 255 );

namespace Colors
{
	// Color to Decimal/Base10

	int32_t HexToDecimal(std::string hexString)
	{
		int32_t returnDecimal = 0;
		size_t headPos = hexString.find("#");

		if (headPos != std::string::npos)
		{
			hexString = hexString.erase(headPos, 1);
		}

		std::stringstream ss;
		ss << std::hex << hexString;
		ss >> returnDecimal;

		return returnDecimal;
	}

	int32_t ColorToDecimal(const Color& color)
	{
		return ((color.R & 0xFF) << 16) | ((color.G & 0xFF) << 8) | (color.B & 0xFF);
	}

	int32_t LinearToDecimal(const LinearColor& linearColor)
	{
		return ColorToDecimal(LinearToColor(linearColor));
	}

	// Decimal/Base10 to Color

	Color DecimalToColor(int32_t decimal)
	{
		Color returnColor;

		returnColor.R = (decimal >> 16) & 0xFF;
		returnColor.G = (decimal >> 8) & 0xFF;
		returnColor.B = (decimal) & 0xFF;
		returnColor.A = 255;

		return returnColor;
	}

	LinearColor DecimalToLinear(int32_t decimal)
	{
		return ColorToLinear(DecimalToColor(decimal));
	}

	// Color to Hexidecimal/Base16

	std::string DecimalToHex(int32_t decimal, bool bInlcudeHead)
	{
		static const std::string hexDigits = "0123456789ABCDEF";
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

	std::string ColorToHex(const Color& color, bool bInlcudeHead)
	{
		return DecimalToHex(ColorToDecimal(color), bInlcudeHead);
	}

	std::string LinearToHex(const LinearColor& linearColor, bool bInlcudeHead)
	{
		return DecimalToHex(LinearToDecimal(linearColor), bInlcudeHead);
	}

	// Hexidecimal/Base16 to Color

	Color HexToColor(std::string hexString)
	{
		return DecimalToColor(HexToDecimal(hexString));
	}

	LinearColor HexToLinear(std::string hexString)
	{
		return DecimalToLinear(HexToDecimal(hexString));
	}

	// Direct Color Conversions

	Color LinearToColor(const LinearColor& linearColor)
	{
		Color convertedColor;

		convertedColor.R = linearColor.R * 255.0f;
		convertedColor.G = linearColor.G * 255.0f;
		convertedColor.B = linearColor.B * 255.0f;
		convertedColor.A = linearColor.A * 255.0f;

		return convertedColor;
	}

	LinearColor ColorToLinear(const Color& color)
	{
		LinearColor convertedColor;

		convertedColor.R = color.R / 255.0f;
		convertedColor.G = color.G / 255.0f;
		convertedColor.B = color.B / 255.0f;
		convertedColor.A = color.A / 255.0f;

		return convertedColor;
	}
}