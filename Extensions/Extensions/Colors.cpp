#include "Colors.hpp"

CRColor::CRColor() : R(255), G(255), B(255), A(255) { }

CRColor::CRColor(const uint8_t rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

CRColor::CRColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : R(r), G(g), B(b), A(a) { }

CRColor::~CRColor() { }

struct FColor CRColor::UnrealColor()
{
	return FColor { B, G, R, A };
}

struct CRColor CRColor::operator=(const struct CRColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

struct CRColor CRColor::operator=(const struct FColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool CRColor::operator==(const struct FColor& other)
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

bool CRColor::operator!=(const struct FColor& other)
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

CRLinearColor::CRLinearColor() : R(255), G(255), B(255), A(255) { }

CRLinearColor::CRLinearColor(const float rgba) : R(rgba), G(rgba), B(rgba), A(rgba) { }

CRLinearColor::CRLinearColor(const float r, const float g, const float b, const float a) : R(r), G(g), B(b), A(a) { }

CRLinearColor::~CRLinearColor() { }

struct FLinearColor CRLinearColor::UnrealColor()
{
	return FLinearColor { R, G, B, A };
}

struct CRLinearColor CRLinearColor::operator=(const struct CRLinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

struct CRLinearColor CRLinearColor::operator=(const struct FLinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool CRLinearColor::operator==(const struct FLinearColor& other)
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

bool CRLinearColor::operator!=(const struct FLinearColor& other)
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