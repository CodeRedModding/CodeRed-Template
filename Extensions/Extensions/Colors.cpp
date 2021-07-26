#include "Colors.hpp"

Color::Color() : R(255), G(255), B(255), A(255) { }

Color::Color(const uint8_t rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : R(r), G(g), B(b), A(a) { }

Color::Color(const Color& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

Color::Color(const struct FColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

Color::~Color() { }

struct FColor Color::UnrealColor()
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

bool Color::operator==(const Color& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return false;
	}

	return true;
}

bool Color::operator==(const struct FColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return false;
	}

	return true;
}

bool Color::operator!=(const Color& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return true;
	}

	return false;
}

bool Color::operator!=(const struct FColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return true;
	}

	return false;
}

LinearColor::LinearColor() : R(255), G(255), B(255), A(255) { }

LinearColor::LinearColor(const float rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

LinearColor::LinearColor(const float r, const float g, const float b, const float a) : R(r), G(g), B(b), A(a) { }

LinearColor::LinearColor(const LinearColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

LinearColor::LinearColor(const struct FLinearColor& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }

LinearColor::~LinearColor() { }

struct FLinearColor LinearColor::UnrealColor()
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

bool LinearColor::operator==(const LinearColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return false;
	}

	return true;
}

bool LinearColor::operator==(const struct FLinearColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return false;
	}

	return true;
}

bool LinearColor::operator!=(const LinearColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return true;
	}

	return false;
}

bool LinearColor::operator!=(const struct FLinearColor& other)
{
	if (R != other.R
		|| G != other.G
		|| B != other.B
		|| A != other.A)
	{
		return true;
	}

	return false;
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

	LinearRainbow = FLinearColor{ static_cast<float>(ByteRainbow.R), static_cast<float>(ByteRainbow.G), static_cast<float>(ByteRainbow.B), static_cast<float>(ByteRainbow.A) };
}

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

		return returnColor;
	}

	struct LinearColor DecimalToLinearColor(const int32_t decimal)
	{
		LinearColor returnColor;

		returnColor.R = (decimal >> 16) & 0xFF;
		returnColor.G = (decimal >> 8) & 0xFF;
		returnColor.B = (decimal) & 0xFF;

		return returnColor;
	}
}