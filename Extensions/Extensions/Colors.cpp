#include "Colors.hpp"
#include "Formatting.hpp"

Color::Color() : R(255), G(255), B(255), A(255) {}

Color::Color(uint8_t rgba) : R(rgba), G(rgba), B(rgba), A(rgba) {}

Color::Color(int32_t rgba) : R(rgba), G(rgba), B(rgba), A(rgba) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) {}

Color::Color(int32_t r, int32_t g, int32_t b, int32_t a) : R(r), G(g), B(b), A(a) {}

Color::Color(float r, float g, float b, float a) : R(255), G(255), B(255), A(255) { FromLinear(LinearColor(r, g, b, a)); }

Color::Color(const std::string& hexColor) : R(255), G(255), B(255), A(255) { FromHex(hexColor); }

Color::Color(const FColor& color) : R(color.R), G(color.G), B(color.B), A(color.A) {}

Color::Color(const Color& color) : R(color.R), G(color.G), B(color.B), A(color.A) {}

Color::~Color() {}

FColor Color::UnrealColor() const
{
	return FColor{ B, G, R, A }; // Your game might be in a different format (RGBA), so be aware of that.
}

class LinearColor Color::ToLinear() const
{
	return LinearColor().FromColor(*this);
}

uint32_t Color::ToDecimal() const
{
	return CodeRed::Format::ToDecimal(ToHex(false)); // Standard method would be "uint32_t decimalValue = (R & 0xFF) << 16) | ((G & 0xFF) << 8) | (B & 0xFF);"
}

uint32_t Color::ToDecimalAlpha() const
{
	return CodeRed::Format::ToDecimal(ToHexAlpha(false));
}

std::string Color::ToHex(bool bNotation) const
{
	std::string hexStr = (bNotation ? "#" : "");
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(R), 2, false);
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(G), 2, false);
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(B), 2, false);
	return hexStr;
}

std::string Color::ToHexAlpha(bool bNotation) const
{
	std::string hexStr = ToHex(bNotation);
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(A), 2, false);
	return hexStr;
}

Color& Color::FromLinear(const LinearColor& linearColor)
{
	R = static_cast<uint8_t>(linearColor.R * 255.0f);
	G = static_cast<uint8_t>(linearColor.G * 255.0f);
	B = static_cast<uint8_t>(linearColor.B * 255.0f);
	A = static_cast<uint8_t>(linearColor.A * 255.0f);
	return *this;
}

Color& Color::FromDecimal(uint32_t decimalColor)
{
	return FromHex(CodeRed::Format::ToHex(decimalColor, ((decimalColor > 0xFFFFFF) ? 8 : 6), false));
}

Color& Color::FromHex(std::string hexColor)
{
	CodeRed::Format::RemoveAllCharsInline(hexColor, '#');

	if (CodeRed::Format::IsStringHexadecimal(hexColor))
	{
		if (hexColor.length() > 8)
		{
			hexColor = hexColor.substr(0, 8);
		}

		uint64_t alpha = 255;

		if (hexColor.length() == 8)
		{
			alpha = CodeRed::Format::ToDecimal(hexColor.substr(6, 2)); // Optional, if an alpha value is provided.
		}

		A = static_cast<uint8_t>(alpha);

		if (hexColor.length() > 6)
		{
			hexColor = hexColor.substr(0, 6); // Removes the alpha value, or invalid trailing characters.
		}

		if (hexColor.length() >= 2)
		{
			R = static_cast<uint8_t>(CodeRed::Format::ToDecimal(hexColor.substr(0, 2)));
		}

		if (hexColor.length() >= 4)
		{
			G = static_cast<uint8_t>(CodeRed::Format::ToDecimal(hexColor.substr(2, 2)));
		}

		if (hexColor.length() >= 6)
		{
			B = static_cast<uint8_t>(CodeRed::Format::ToDecimal(hexColor.substr(4, 2)));
		}
	}

	return *this;
}

Color& Color::Cycle(int32_t steps)
{
	uint8_t constant = R;
	if (G > constant) { constant = G; }
	if (B > constant) { constant = B; }

	if ((R == constant) && (G < constant) && (B == 0)) { G += steps; } 			// Green goes up.
	else if ((R > 0) && (G == constant) && (B == 0)) { R -= steps; }			// Red goes down.
	else if ((R == 0) && (G == constant) && (B < constant)) { B += steps; }		// Blue goes up.
	else if ((R == 0) && (G > 0) && (B == constant)) { G -= steps; }			// Green goes down.
	else if ((R < constant) && (G == 0) && (B == constant)) { R += steps; }		// Red goes up.
	else if ((R == constant) && (G == 0) && (B > 0)) { B -= steps; }			// Blue goes down.
	return *this;
}

Color& Color::operator=(const Color& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

Color& Color::operator=(const FColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool Color::operator==(const Color& other) const
{
	return ((R == other.R) && (G == other.G) && (B == other.B) && (A == other.A));
}

bool Color::operator==(const FColor& other) const
{
	return ((R == other.R) && (G == other.G) && (B == other.B) && (A == other.A));
}

bool Color::operator!=(const Color& other) const
{
	return !(*this == other);
}

bool Color::operator!=(const FColor& other) const
{
	return !(*this == other);
}

bool Color::operator<(const Color& other) const
{
	return (ToHexAlpha(false) < other.ToHexAlpha(false));
}

bool Color::operator>(const Color& other) const
{
	return (ToHexAlpha(false) > other.ToHexAlpha(false));
}

LinearColor::LinearColor() : R(1.0f), G(1.0f), B(1.0f), A(1.0f) {}

LinearColor::LinearColor(float rgba) : R(rgba), G(rgba), B(rgba), A(rgba) {}

LinearColor::LinearColor(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

LinearColor::LinearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(1.0f), G(1.0f), B(1.0f), A(1.0f) { FromColor(Color(a, g, b, a)); }

LinearColor::LinearColor(const std::string& hexColor) : R(1.0f), G(1.0f), B(1.0f), A(1.0f) { FromHex(hexColor); }

LinearColor::LinearColor(const FLinearColor& linearColor) : R(linearColor.R), G(linearColor.G), B(linearColor.B), A(linearColor.A) {}

LinearColor::LinearColor(const LinearColor& linearColor) : R(linearColor.R), G(linearColor.G), B(linearColor.B), A(linearColor.A) {}

LinearColor::~LinearColor() {}

FLinearColor LinearColor::UnrealColor() const
{
	return FLinearColor{ R, G, B, A };
}

Color LinearColor::ToColor() const
{
	return Color(R, G, B, A);
}

uint32_t LinearColor::ToDecimal() const
{
	return ToColor().ToDecimal();
}

uint32_t LinearColor::ToDecimalAlpha() const
{
	return ToColor().ToDecimalAlpha();
}

std::string LinearColor::ToHex(bool bNotation) const
{
	return ToColor().ToHex(bNotation);
}

std::string LinearColor::ToHexAlpha(bool bNotation) const
{
	return ToColor().ToHexAlpha(bNotation);
}

LinearColor& LinearColor::FromColor(const Color& color)
{
	R = (color.R > 0 ? (static_cast<float>(color.R) / 255.0f) : 0);
	G = (color.G > 0 ? (static_cast<float>(color.G) / 255.0f) : 0);
	B = (color.B > 0 ? (static_cast<float>(color.B) / 255.0f) : 0);
	A = (color.A > 0 ? (static_cast<float>(color.A) / 255.0f) : 0);
	return *this;
}

LinearColor& LinearColor::FromDecimal(uint32_t decimalColor)
{
	return FromColor(Color().FromDecimal(decimalColor));
}

LinearColor& LinearColor::FromHex(std::string hexColor)
{
	return FromColor(Color(hexColor));
}

LinearColor& LinearColor::Cycle(int32_t steps)
{
	return FromColor(ToColor().Cycle(steps));
}

LinearColor& LinearColor::operator=(const LinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

LinearColor& LinearColor::operator=(const FLinearColor& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

bool LinearColor::operator==(const LinearColor& other) const
{
	return ((R == other.R) && (G == other.G) && (B == other.B) && (A == other.A));
}

bool LinearColor::operator==(const FLinearColor& other) const
{
	return ((R == other.R) && (G == other.G) && (B == other.B) && (A == other.A));
}

bool LinearColor::operator!=(const LinearColor& other) const
{
	return !(*this == other);
}

bool LinearColor::operator!=(const FLinearColor& other) const
{
	return !(*this == other);
}

bool LinearColor::operator<(const LinearColor& other) const
{
	return (ToHexAlpha(false) < other.ToHexAlpha(false));
}

bool LinearColor::operator>(const LinearColor& other) const
{
	return (ToHexAlpha(false) > other.ToHexAlpha(false));
}

Color GRainbowColor::GetByte()
{
	return ByteRainbow;
}

LinearColor GRainbowColor::GetLinear()
{
	return LinearRainbow;
}

void GRainbowColor::Reset()
{
	ByteRainbow = Color(0, 0, 255, 255);
	LinearRainbow = LinearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void GRainbowColor::OnTick()
{
	ByteRainbow.Cycle();
	LinearRainbow = ByteRainbow.ToLinear();
}

const Color GColorList::White = Color(255, 255, 255, 255);
const Color GColorList::Red = Color(255, 0, 0, 255);
const Color GColorList::Green = Color(0, 255, 0, 255);
const Color GColorList::Blue = Color(0, 0, 255, 255);
const Color GColorList::Magenta = Color(255, 0, 255, 255);
const Color GColorList::Cyan = Color(0, 255, 255, 255);
const Color GColorList::Yellow = Color(255, 255, 0, 255);
const Color GColorList::Black = Color(0, 0, 0, 255);
const Color GColorList::Aquamarine = Color(112, 219, 147, 255);
const Color GColorList::BakerChocolate = Color(92, 51, 23, 255);
const Color GColorList::BlueViolet = Color(159, 95, 159, 255);
const Color GColorList::Brass = Color(181, 166, 66, 255);
const Color GColorList::BrightGold = Color(217, 217, 25, 255);
const Color GColorList::Brown = Color(166, 42, 42, 255);
const Color GColorList::Bronze = Color(140, 120, 83, 255);
const Color GColorList::BronzeII = Color(166, 125, 61, 255);
const Color GColorList::CadetBlue = Color(95, 159, 159, 255);
const Color GColorList::CoolCopper = Color(217, 135, 25, 255);
const Color GColorList::Copper = Color(184, 115, 51, 255);
const Color GColorList::Coral = Color(255, 127, 0, 255);
const Color GColorList::CornFlowerBlue = Color(66, 66, 111, 255);
const Color GColorList::DarkBrown = Color(92, 64, 51, 255);
const Color GColorList::DarkGreen = Color(47, 79, 47, 255);
const Color GColorList::DarkGreenCopper = Color(74, 118, 110, 255);
const Color GColorList::DarkOliveGreen = Color(79, 79, 47, 255);
const Color GColorList::DarkOrchid = Color(153, 50, 205, 255);
const Color GColorList::DarkPurple = Color(135, 31, 120, 255);
const Color GColorList::DarkSlateBlue = Color(107, 35, 142, 255);
const Color GColorList::DarkSlateGrey = Color(47, 79, 79, 255);
const Color GColorList::DarkTan = Color(151, 105, 79, 255);
const Color GColorList::DarkTurquoise = Color(112, 147, 219, 255);
const Color GColorList::DarkWood = Color(133, 94, 66, 255);
const Color GColorList::DimGrey = Color(84, 84, 84, 255);
const Color GColorList::DustyRose = Color(133, 99, 99, 255);
const Color GColorList::Feldspar = Color(209, 146, 117, 255);
const Color GColorList::Firebrick = Color(142, 35, 35, 255);
const Color GColorList::ForestGreen = Color(35, 142, 35, 255);
const Color GColorList::Gold = Color(205, 127, 50, 255);
const Color GColorList::Goldenrod = Color(219, 219, 112, 255);
const Color GColorList::Grey = Color(192, 192, 192, 255);
const Color GColorList::GreenCopper = Color(82, 127, 118, 255);
const Color GColorList::GreenYellow = Color(147, 219, 112, 255);
const Color GColorList::HunterGreen = Color(33, 94, 33, 255);
const Color GColorList::IndianRed = Color(78, 47, 47, 255);
const Color GColorList::Khaki = Color(159, 159, 95, 255);
const Color GColorList::LightBlue = Color(192, 217, 217, 255);
const Color GColorList::LightGrey = Color(168, 168, 168, 255);
const Color GColorList::LightSteelBlue = Color(143, 143, 189, 255);
const Color GColorList::LightWood = Color(233, 194, 166, 255);
const Color GColorList::LimeGreen = Color(50, 205, 50, 255);
const Color GColorList::MandarianOrange = Color(228, 120, 51, 255);
const Color GColorList::Maroon = Color(142, 35, 107, 255);
const Color GColorList::MediumAquamarine = Color(50, 205, 153, 255);
const Color GColorList::MediumBlue = Color(50, 50, 205, 255);
const Color GColorList::MediumForestGreen = Color(107, 142, 35, 255);
const Color GColorList::MediumGoldenrod = Color(234, 234, 174, 255);
const Color GColorList::MediumOrchid = Color(147, 112, 219, 255);
const Color GColorList::MediumSeaGreen = Color(66, 111, 66, 255);
const Color GColorList::MediumSlateBlue = Color(127, 0, 255, 255);
const Color GColorList::MediumSpringGreen = Color(127, 255, 0, 255);
const Color GColorList::MediumTurquoise = Color(112, 219, 219, 255);
const Color GColorList::MediumVioletRed = Color(219, 112, 147, 255);
const Color GColorList::MediumWood = Color(166, 128, 100, 255);
const Color GColorList::MidnightBlue = Color(47, 47, 79, 255);
const Color GColorList::NavyBlue = Color(35, 35, 142, 255);
const Color GColorList::NeonBlue = Color(77, 77, 255, 255);
const Color GColorList::NeonPink = Color(255, 110, 199, 255);
const Color GColorList::NewMidnightBlue = Color(0, 0, 156, 255);
const Color GColorList::NewTan = Color(235, 199, 158, 255);
const Color GColorList::OldGold = Color(207, 181, 59, 255);
const Color GColorList::Orange = Color(255, 127, 0, 255);
const Color GColorList::OrangeRed = Color(255, 36, 0, 255);
const Color GColorList::Orchid = Color(219, 112, 219, 255);
const Color GColorList::PaleGreen = Color(143, 188, 143, 255);
const Color GColorList::Pink = Color(188, 143, 143, 255);
const Color GColorList::Plum = Color(234, 173, 234, 255);
const Color GColorList::Quartz = Color(217, 217, 243, 255);
const Color GColorList::RichBlue = Color(89, 89, 171, 255);
const Color GColorList::Salmon = Color(111, 66, 66, 255);
const Color GColorList::Scarlet = Color(140, 23, 23, 255);
const Color GColorList::SeaGreen = Color(35, 142, 104, 255);
const Color GColorList::SemiSweetChocolate = Color(107, 66, 38, 255);
const Color GColorList::Sienna = Color(142, 107, 35, 255);
const Color GColorList::Silver = Color(230, 232, 250, 255);
const Color GColorList::SkyBlue = Color(50, 153, 204, 255);
const Color GColorList::SlateBlue = Color(0, 127, 255, 255);
const Color GColorList::SpicyPink = Color(255, 28, 174, 255);
const Color GColorList::SpringGreen = Color(0, 255, 127, 255);
const Color GColorList::SteelBlue = Color(35, 107, 142, 255);
const Color GColorList::SummerSky = Color(56, 176, 222, 255);
const Color GColorList::Tan = Color(219, 147, 112, 255);
const Color GColorList::Thistle = Color(216, 191, 216, 255);
const Color GColorList::Turquoise = Color(173, 234, 234, 255);
const Color GColorList::VeryDarkBrown = Color(92, 64, 51, 255);
const Color GColorList::VeryLightGrey = Color(205, 205, 205, 255);
const Color GColorList::Violet = Color(79, 47, 79, 255);
const Color GColorList::VioletRed = Color(204, 50, 153, 255);
const Color GColorList::Wheat = Color(216, 216, 191, 255);
const Color GColorList::YellowGreen = Color(153, 204, 50, 255);