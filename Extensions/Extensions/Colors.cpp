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
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(R), 2, "");
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(G), 2, "");
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(B), 2, "");
	return hexStr;
}

std::string Color::ToHexAlpha(bool bNotation) const
{
	std::string hexStr = ToHex(bNotation);
	hexStr += CodeRed::Format::ToHex(static_cast<uint64_t>(A), 2, "");
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
	return FromHex(CodeRed::Format::ToHex(decimalColor, ((decimalColor > 0xFFFFFF) ? 8 : 6), ""));
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

namespace CodeRed
{
	Color GRainbowColor::m_rainbow = Color(0, 0, 255, 255);

	void GRainbowColor::OnTick()
	{
		m_rainbow.Cycle();
	}

	void GRainbowColor::Reset()
	{
		m_rainbow = Color(0, 0, 255, 255);
	}

	Color GRainbowColor::GetColor()
	{
		return m_rainbow;
	}

	LinearColor GRainbowColor::GetLinear()
	{
		return m_rainbow.ToLinear();
	}

	const Color GColorList::White = Color("#FFFFFF");
	const Color GColorList::Red = Color("#FF0000");
	const Color GColorList::Green = Color("#00FF00");
	const Color GColorList::Blue = Color("#0000FF");
	const Color GColorList::Magenta = Color("#FF00FF");
	const Color GColorList::Cyan = Color("#00FFFF");
	const Color GColorList::Yellow = Color("#FFFF00");
	const Color GColorList::Black = Color("#000000");
	const Color GColorList::Aquamarine = Color("#70DB93");
	const Color GColorList::BakerChocolate = Color("#5C3317");
	const Color GColorList::BlueViolet = Color("#9F5F9F");
	const Color GColorList::Brass = Color("#B5A642");
	const Color GColorList::BrightGold = Color("#D9D919");
	const Color GColorList::Brown = Color("#2AA62A");
	const Color GColorList::Bronze = Color("#8C7853");
	const Color GColorList::BronzeII = Color("#A67D3D");
	const Color GColorList::CadetBlue = Color("#5F9F9F");
	const Color GColorList::CoolCopper = Color("#D98719");
	const Color GColorList::Copper = Color("#B87333");
	const Color GColorList::Coral = Color("#FF7F00");
	const Color GColorList::CornFlowerBlue = Color("#42426F");
	const Color GColorList::DarkBrown = Color("#5C4033");
	const Color GColorList::DarkGreen = Color("#2F4F2F");
	const Color GColorList::DarkGreenCopper = Color("#4A766E");
	const Color GColorList::DarkOliveGreen = Color("#4F4F2F");
	const Color GColorList::DarkOrchid = Color("#9932CD");
	const Color GColorList::DarkPurple = Color("#871F78");
	const Color GColorList::DarkSlateBlue = Color("#6B238E");
	const Color GColorList::DarkSlateGrey = Color("#2F4F4F");
	const Color GColorList::DarkTan = Color("#97694F");
	const Color GColorList::DarkTurquoise = Color("#7093DB");
	const Color GColorList::DarkWood = Color("#855E42");
	const Color GColorList::DimGrey = Color("#545454");
	const Color GColorList::DustyRose = Color("#856363");
	const Color GColorList::Feldspar = Color("#D19275");
	const Color GColorList::Firebrick = Color("#8E2323");
	const Color GColorList::ForestGreen = Color("#238E23");
	const Color GColorList::Gold = Color("#CD7F32");
	const Color GColorList::Goldenrod = Color("#DBDB70");
	const Color GColorList::Grey = Color("#C0C0C0");
	const Color GColorList::GreenCopper = Color("#527F76");
	const Color GColorList::GreenYellow = Color("#93DB70");
	const Color GColorList::HunterGreen = Color("#215E21");
	const Color GColorList::IndianRed = Color("#4E2F2F");
	const Color GColorList::Khaki = Color("#9F9F5F");
	const Color GColorList::LightBlue = Color("#C0D9D9");
	const Color GColorList::LightGrey = Color("#A8A8A8");
	const Color GColorList::LightSteelBlue = Color("#8F8FBD");
	const Color GColorList::LightWood = Color("#E9C2A6");
	const Color GColorList::LimeGreen = Color("#32CD32");
	const Color GColorList::MandarinOrange = Color("#E47833");
	const Color GColorList::Maroon = Color("#8E236B");
	const Color GColorList::MediumAquamarine = Color("#32CD99");
	const Color GColorList::MediumBlue = Color("#3232CD");
	const Color GColorList::MediumForestGreen = Color("#6B8E23");
	const Color GColorList::MediumGoldenrod = Color("#EAEAAE");
	const Color GColorList::MediumOrchid = Color("#9370DB");
	const Color GColorList::MediumSeaGreen = Color("#426F42");
	const Color GColorList::MediumSlateBlue = Color("#7F00FF");
	const Color GColorList::MediumSpringGreen = Color("#7FFF00");
	const Color GColorList::MediumTurquoise = Color("#70DBDB");
	const Color GColorList::MediumVioletRed = Color("#DB7093");
	const Color GColorList::MediumWood = Color("#A68064");
	const Color GColorList::MidnightBlue = Color("#2F2F4F");
	const Color GColorList::NavyBlue = Color("#23238E");
	const Color GColorList::NeonBlue = Color("#4D4DFF");
	const Color GColorList::NeonPink = Color("#FF6EC7");
	const Color GColorList::NewMidnightBlue = Color("#00009C");
	const Color GColorList::NewTan = Color("#EBC79E");
	const Color GColorList::OldGold = Color("#CFB53B");
	const Color GColorList::Orange = Color("#FF7F00");
	const Color GColorList::OrangeRed = Color("#FF2400");
	const Color GColorList::Orchid = Color("#DB70DB");
	const Color GColorList::PaleGreen = Color("#8FBC8F");
	const Color GColorList::Pink = Color("#BC8F8F");
	const Color GColorList::Plum = Color("#EAADEA");
	const Color GColorList::Quartz = Color("#D9D9F3");
	const Color GColorList::RichBlue = Color("#5959AB");
	const Color GColorList::Salmon = Color("#6F4242");
	const Color GColorList::Scarlet = Color("#8C1717");
	const Color GColorList::SeaGreen = Color("#238E68");
	const Color GColorList::SemiSweetChocolate = Color("#6B4226");
	const Color GColorList::Sienna = Color("#8E6B23");
	const Color GColorList::Silver = Color("#E6E8FA");
	const Color GColorList::SkyBlue = Color("#3299CC");
	const Color GColorList::SlateBlue = Color("#007FFF");
	const Color GColorList::SpicyPink = Color("#FF1CAE");
	const Color GColorList::SpringGreen = Color("#00FF7F");
	const Color GColorList::SteelBlue = Color("#236B8E");
	const Color GColorList::SummerSky = Color("#38B0DE");
	const Color GColorList::Tan = Color("#DB9370");
	const Color GColorList::Thistle = Color("#D8BFD8");
	const Color GColorList::Turquoise = Color("#ADEAEA");
	const Color GColorList::VeryDarkBrown = Color("#5C4033");
	const Color GColorList::VeryLightGrey = Color("#CDCDCD");
	const Color GColorList::Violet = Color("#4F2F4F");
	const Color GColorList::VioletRed = Color("#CC3299");
	const Color GColorList::Wheat = Color("#D8D8BF");
	const Color GColorList::YellowGreen = Color("#99CC32");
}