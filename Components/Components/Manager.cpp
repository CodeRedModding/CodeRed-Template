#include "Manager.hpp"
#include "../Includes.hpp"
#include "../Extensions/Includes.hpp"

/*
# ================================================== #
# Setting Class
# ================================================== #
*/

Setting::Setting(const std::string& name, const std::string& description, const bool& bModifiable, const SettingTypes type)
{
	Name = name;
	Description = description;
	Modifiable = bModifiable;
	Hidden = false;
	ShouldCallback = false;
	Type = type;
}

Setting::~Setting() { }

std::string Setting::GetName() const
{
	return Name;
}

std::string Setting::GetDescription() const
{
	return Description;
}

SettingTypes Setting::GetType() const
{
	return Type;
}

bool Setting::IsModifiable() const
{
	return Modifiable;
}

bool Setting::IsHidden() const
{
	return Hidden;
}

bool Setting::HasCallback() const
{
	return ShouldCallback;
}

void Setting::SetShouldCallback(const bool& bShouldCallback)
{
	ShouldCallback = bShouldCallback;
}

/*
# ================================================== #
# Int Setting Sub-Class
# ================================================== #
*/

IntSetting::IntSetting(const std::string& name, const std::string& description, bool bModifiable, const int& value, std::function<void(class IntSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_INT)
{
	Value = value;
	Callback = callback;
	SetShouldCallback(true);
}

IntSetting::IntSetting(const std::string& name, const std::string& description, bool bModifiable, const int& value) : Setting(name, description, bModifiable, SettingTypes::TYPE_INT)
{
	Value = value;
}

IntSetting::IntSetting(const std::string& name, const std::string& description, bool bModifiable) : Setting(name, description, bModifiable, SettingTypes::TYPE_INT)
{
	Value = 0;
}

IntSetting::~IntSetting() { }

void IntSetting::SetValue(const int& value)
{
	if (IsModifiable())
	{
		Value = value;
	}
}

int IntSetting::GetValue() const
{
	return Value;
}

void IntSetting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback(*this);
	}
}

/*
# ================================================== #
# Float Setting Sub-Class
# ================================================== #
*/

FloatSetting::FloatSetting(const std::string& name, const std::string& description, bool bModifiable, const float& value, std::function<void(class FloatSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_FLOAT)
{
	Value = value;
	Callback = callback;
	SetShouldCallback(true);
}

FloatSetting::FloatSetting(const std::string& name, const std::string& description, bool bModifiable, const float& value) : Setting(name, description, bModifiable, SettingTypes::TYPE_FLOAT)
{
	Value = value;
}

FloatSetting::FloatSetting(const std::string& name, const std::string& description, bool bModifiable) : Setting(name, description, bModifiable, SettingTypes::TYPE_FLOAT)
{
	Value = 0.f;
}

FloatSetting::~FloatSetting() { }

void FloatSetting::SetValue(const float& value)
{
	if (IsModifiable())
	{
		Value = value;
	}
}

float FloatSetting::GetValue() const
{
	return Value;
}

void FloatSetting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback(*this);
	}
}

/*
# ================================================== #
# Bool Setting Sub-Class
# ================================================== #
*/

BoolSetting::BoolSetting(const std::string& name, const std::string& description, bool bModifiable, const bool& value, std::function<void(class BoolSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_BOOL)
{
	Value = value;
	Callback = callback;
	SetShouldCallback(true);
}

BoolSetting::BoolSetting(const std::string& name, const std::string& description, bool bModifiable, const bool& value) : Setting(name, description, bModifiable, SettingTypes::TYPE_BOOL)
{
	Value = value;
}

BoolSetting::BoolSetting(const std::string& name, const std::string& description, bool bModifiable) : Setting(name, description, bModifiable, SettingTypes::TYPE_BOOL)
{
	Value = false;
}

BoolSetting::~BoolSetting() { }

void BoolSetting::SetValue(const bool& value)
{
	if (IsModifiable())
	{
		Value = value;
	}
}

bool BoolSetting::GetValue() const
{
	return Value;
}

void BoolSetting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback(*this);
	}
}

/*
# ================================================== #
# String Setting Sub-Class
# ================================================== #
*/

StringSetting::StringSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& value, std::function<void(class StringSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_STRING)
{
	Value = value;
	Callback = callback;
	SetShouldCallback(true);
}

StringSetting::StringSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& value) : Setting(name, description, bModifiable, SettingTypes::TYPE_STRING)
{
	Value = value;
}

StringSetting::StringSetting(const std::string& name, const std::string& description, bool bModifiable) : Setting(name, description, bModifiable, SettingTypes::TYPE_STRING)
{
	Value = "null";
}

StringSetting::~StringSetting() { }

void StringSetting::SetValue(const std::string& value)
{
	if (IsModifiable())
	{
		Value = value;
	}
}

std::string StringSetting::GetValue() const
{
	return Value;
}

std::vector<std::string> StringSetting::GetSplitValue(const char split)
{
	std::vector<std::string> returnValue;
	std::string splitValue;

	for (const char& c : Value)
	{
		if (c == split)
		{
			returnValue.push_back(splitValue);
			splitValue.clear();
		}
		else
		{
			splitValue += c;
		}
	}

	return returnValue;
}

void StringSetting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback(*this);
	}
}

/*
# ================================================== #
# Color Setting Sub-Class
# ================================================== #
*/

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a, std::function<void(class ColorSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	R = r;
	G = g;
	B = b;
	A = a;

	Callback = callback;
	SetShouldCallback(true);
	InitializeValues(ColorTypes::TYPE_RGBA);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	R = r;
	G = g;
	B = b;
	A = a;

	InitializeValues(ColorTypes::TYPE_RGBA);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, int32_t decimalValue, std::function<void(class ColorSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	DecimalValue = decimalValue;

	Callback = callback;
	SetShouldCallback(true);
	InitializeValues(ColorTypes::TYPE_DECIMAL);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, int32_t decimalValue) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	DecimalValue = decimalValue;

	InitializeValues(ColorTypes::TYPE_DECIMAL);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FColor& colorValue, std::function<void(class ColorSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	ColorValue = colorValue;

	Callback = callback;
	SetShouldCallback(true);
	InitializeValues(ColorTypes::TYPE_COLOR);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FColor& colorValue) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	ColorValue = colorValue;

	InitializeValues(ColorTypes::TYPE_COLOR);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FLinearColor& linearValue, std::function<void(class ColorSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	LinearValue = linearValue;

	Callback = callback;
	SetShouldCallback(true);
	InitializeValues(ColorTypes::TYPE_LINEAR);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FLinearColor& linearValue) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	LinearValue = linearValue;

	InitializeValues(ColorTypes::TYPE_LINEAR);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& hexValue, std::function<void(class ColorSetting)> callback) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	HexValue = hexValue;

	Callback = callback;
	SetShouldCallback(true);
	InitializeValues(ColorTypes::TYPE_HEX);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& hexValue) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	HexValue = hexValue;

	InitializeValues(ColorTypes::TYPE_HEX);
}

ColorSetting::ColorSetting(const std::string& name, const std::string& description, bool bModifiable) : Setting(name, description, bModifiable, SettingTypes::TYPE_COLOR)
{
	R = G = B = A = 0;
	DecimalValue = 0;
	ColorValue = { 0, 0, 0, 0 };
	LinearValue = { 0, 0, 0, 0 };
	HexValue = "null";
}

ColorSetting::~ColorSetting() { }

void ColorSetting::SetValue(const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a)
{
	if (IsModifiable())
	{
		R = r;
		G = g;
		B = b;
		A = a;

		InitializeValues(ColorTypes::TYPE_RGBA);
	}
}
void ColorSetting::SetValue(const int32_t& decimalValue)
{
	if (IsModifiable())
	{
		DecimalValue = decimalValue;

		InitializeValues(ColorTypes::TYPE_DECIMAL);
	}
}

void ColorSetting::SetValue(const struct FColor& colorValue)
{
	if (IsModifiable())
	{
		ColorValue = colorValue;

		InitializeValues(ColorTypes::TYPE_COLOR);
	}
}

void ColorSetting::SetValue(const struct FLinearColor& linearValue)
{
	if (IsModifiable())
	{
		LinearValue = linearValue;

		InitializeValues(ColorTypes::TYPE_LINEAR);
	}
}
void ColorSetting::SetValue(const std::string& hexValue)
{
	if (IsModifiable())
	{
		HexValue = hexValue;

		InitializeValues(ColorTypes::TYPE_HEX);
	}
}

void ColorSetting::GetRGBAValue(int32_t& r, int32_t& g, int32_t& b, int32_t& a) const
{
	r = R;
	g = G;
	b = B;
	a = A;
}

int32_t ColorSetting::GetDecimalValue() const
{
	return DecimalValue;
}

struct FColor ColorSetting::GetColorValue() const
{
	return ColorValue;
}

struct FLinearColor ColorSetting::GetLinearValue() const
{
	return LinearValue;
}

std::string ColorSetting::GetHexValue() const
{
	return HexValue;
}

std::string ColorSetting::ToHex()
{
	std::string hexDigits = "0123456789ABCDEF";
	std::string hexColor = "#";

	for (int i = (3 * 2) - 1; i >= 0; i--)
	{
		hexColor += hexDigits[((DecimalValue >> i * 4) & 0xF)];
	}

	return hexColor;
}

void ColorSetting::InitializeValues(const ColorTypes& type)
{
	if (type == ColorTypes::TYPE_RGBA)
	{
		ColorValue = UObject::StaticClass()->MakeColor(R, G, B, A);
		DecimalValue = UObject::StaticClass()->ColorToInt(ColorValue);
		LinearValue = UObject::StaticClass()->ColorToLinearColor(ColorValue);
		HexValue = ToHex();
	}
	else if (type == ColorTypes::TYPE_DECIMAL)
	{
		ColorValue = UObject::StaticClass()->IntToColor(DecimalValue);
		R = ColorValue.R;
		G = ColorValue.G;
		B = ColorValue.B;
		A = ColorValue.A;
		LinearValue = UObject::StaticClass()->ColorToLinearColor(ColorValue);
		HexValue = ToHex();
	}
	else if (type == ColorTypes::TYPE_COLOR)
	{
		DecimalValue = UObject::StaticClass()->ColorToInt(ColorValue);
		R = ColorValue.R;
		G = ColorValue.G;
		B = ColorValue.B;
		A = ColorValue.A;
		LinearValue = UObject::StaticClass()->ColorToLinearColor(ColorValue);
		HexValue = ToHex();
	}
	else if (type == ColorTypes::TYPE_LINEAR)
	{
		DecimalValue = UObject::StaticClass()->LinearColorToInt(LinearValue);
		ColorValue = UObject::StaticClass()->LinearColorToColor(LinearValue);
		R = ColorValue.R;
		G = ColorValue.G;
		B = ColorValue.B;
		A = ColorValue.A;
		HexValue = ToHex();
	}
	else if (type == ColorTypes::TYPE_HEX)
	{
		//ColorValue = UObject::StaticClass()->FromHexColor(StringWrapper(HexValue).ToUnrealString()); // I have my own custom wrappers, they are too game-specific to implement directly here.
		R = ColorValue.R;
		G = ColorValue.G;
		B = ColorValue.B;
		A = ColorValue.A;
		DecimalValue = UObject::StaticClass()->ColorToInt(ColorValue);
		LinearValue = UObject::StaticClass()->ColorToLinearColor(ColorValue);
	}
}

void ColorSetting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback(*this);
	}
}

/*
# ================================================== #
# Command Base Class
# ================================================== #
*/

Command::Command(const std::string& name, const std::string& description, const bool& bHidden, const CommandTypes type)
{
	Name = name;
	Description = description;
	Hidden = bHidden;
	Type = type;
}

Command::~Command() { }

std::string Command::GetName() const
{
	return Name;
}

std::string Command::GetDescription() const
{
	return Description;
}

CommandTypes Command::GetType() const
{
	return Type;
}

bool Command::IsHidden() const
{
	return Hidden;
}

/*
# ================================================== #
# Callback Command Sub-Class
# ================================================== #
*/

CallbackCommand::CallbackCommand(const std::string& name, const std::string& description, const bool& bHidden, std::function<void()> callback) : Command(name, description, bHidden, CommandTypes::TYPE_CALLBACK)
{
	Callback = callback;
}

CallbackCommand::~CallbackCommand() { }

void CallbackCommand::TriggerCallback() const
{
	Callback();
}

/*
# ================================================== #
# Parameter Command Sub-Class
# ================================================== #
*/

ArgumentCommand::ArgumentCommand(const std::string& name, const std::string& description, const bool& bHidden, std::function<void(std::string)> callback) : Command(name, description, bHidden, CommandTypes::TYPE_ARGUMENT)
{
	Callback = callback;
}

ArgumentCommand::~ArgumentCommand() { }

void ArgumentCommand::TriggerCallback(const std::string& arguments) const
{
	Callback(arguments);
}

/*
# ================================================== #
# Manager Component
# ================================================== #
*/

ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and mods.") { }

ManagerComponent::~ManagerComponent()
{
	Commands.clear();
	Settings.clear();
}

void ManagerComponent::KeyPressed(const std::string& key)
{
	if (key == "NumPadNine")
	{
		// Example of modifying a custom setting in a mod, we assign this command in the "Initialize" function below.
		ConsoleCommand("placeholder_enabled", "true");
	}
}

void ManagerComponent::UnrealCommand(const std::string& unrealCommand)
{
	AActor* randomActor = Instances.GetInstanceOf<AActor>();

	if (randomActor)
	{
		Console.Write(GetNameFormatted() +  "Executing enreal command: \"" + unrealCommand + "\"");
		//randomActor->ConsoleCommand(StringWrapper(unrealCommand).ToUnrealString(), false); // I have my own custom wrappers, they are too game-specific to implement directly here.
	}
}

void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments)
{
	std::shared_ptr<Command> consoleCommand = GetCommand<Command>(command);

	if (consoleCommand->GetType() == CommandTypes::TYPE_NONE)
	{
		std::shared_ptr<Setting> consoleSetting = GetSetting<Setting>(command);

		if (consoleSetting->GetType() == SettingTypes::TYPE_NONE)
		{
			Console.Error(GetNameFormatted() + "Unrecognized command: \"" + command + "\"");
		}
		else if (!consoleSetting->IsHidden())
		{
			if (consoleSetting->IsModifiable() && arguments.size() > 0)
			{
				ModifySetting(consoleSetting, arguments);
			}
			else
			{
				Console.Write(GetNameFormatted() + consoleSetting->GetName() + ", " + consoleSetting->GetDescription());
			}
		}
		else
		{
			Console.Error(GetNameFormatted() + "Unrecognized command: \"" + command + "\"");
		}
	}
	else if (consoleCommand->GetType() == CommandTypes::TYPE_CALLBACK)
	{
		std::reinterpret_pointer_cast<CallbackCommand>(consoleCommand)->TriggerCallback();
	}
	else if (consoleCommand->GetType() == CommandTypes::TYPE_ARGUMENT)
	{
		std::reinterpret_pointer_cast<ArgumentCommand>(consoleCommand)->TriggerCallback(arguments);
	}
}

void ManagerComponent::ModifySetting(std::shared_ptr<class Setting>& setting, const std::string& arguments)
{
	if (setting->GetType() == SettingTypes::TYPE_INT)
	{
		std::shared_ptr<IntSetting> intSetting = std::reinterpret_pointer_cast<IntSetting>(setting);

		intSetting->SetValue(std::stoi(arguments));
		
		if (intSetting->HasCallback())
		{
			intSetting->TriggerCallback();
		}
	}
	else if (setting->GetType() == SettingTypes::TYPE_BOOL)
	{
		std::shared_ptr<BoolSetting> boolSetting = std::reinterpret_pointer_cast<BoolSetting>(setting);

		if (arguments == "true" || arguments == "1")
		{
			boolSetting->SetValue(true);
		}
		else if (arguments == "false" || arguments == "0")
		{
			boolSetting->SetValue(false);
		}

		if (boolSetting->HasCallback())
		{
			boolSetting->TriggerCallback();
		}
	}
	else if (setting->GetType() == SettingTypes::TYPE_FLOAT)
	{
		std::shared_ptr<FloatSetting> floatSetting = std::reinterpret_pointer_cast<FloatSetting>(setting);

		floatSetting->SetValue(std::stof(arguments));

		if (floatSetting->HasCallback())
		{
			floatSetting->TriggerCallback();
		}
	}
	else if (setting->GetType() == SettingTypes::TYPE_STRING)
	{
		std::shared_ptr<StringSetting> stringSetting = std::reinterpret_pointer_cast<StringSetting>(setting);

		stringSetting->SetValue(arguments);

		if (stringSetting->HasCallback())
		{
			stringSetting->TriggerCallback();
		}
	}
	else if (setting->GetType() == SettingTypes::TYPE_COLOR)
	{
		std::shared_ptr<ColorSetting> colorSetting = std::reinterpret_pointer_cast<ColorSetting>(setting);

		if (arguments.find("#") != std::string::npos)
		{
			colorSetting->SetValue(arguments);
		}
	}
}

std::string ManagerComponent::PhraseArguments(std::string arguments)
{
	size_t spacePos = arguments.find(' ');

	if (spacePos != std::string::npos)
	{
		arguments = arguments.substr(0, spacePos);
	}

	return arguments;
}

std::vector<std::string> ManagerComponent::SplitArguments(const std::string& arguments)
{
	std::vector<std::string> returnValue;
	std::string word;

	for (const char& x : arguments) 
	{
		if (x == ' ')
		{
			returnValue.push_back(word);
			word = "";
		}
		else
		{
			word = word + x;
		}
	}

	return returnValue;
}

void ManagerComponent::Initialize()
{
	Commands.push_back(std::make_shared<Command>(Command("command_not_found", "Command not found.", false, CommandTypes::TYPE_NONE)));
	Settings.push_back(std::make_shared<Setting>(Setting("setting_not_found", "Setting not found.", false, SettingTypes::TYPE_NONE)));
	
	// Assigning the shared pointer with the CasualMatch and RankedMatch flags, so this mod will only be able to be used in casual or ranked games.
	PlaceholderMod = std::make_shared<PlaceholderModule>(PlaceholderModule("Paceholder", "An example module.", States::CasualMatch | States::RankedMatch));
	
	// When someone uses the console command "placeholder_some_bool true", we automatically callback to PlaceholderMod and tell it to update its "SomeBoolSetting" property.
	Commands.push_back(std::make_shared<CallbackCommand>("placeholder_do_thing", "Calls the \"DoAThing\" function in \"PlaceholderMod\".", false, std::bind(&PlaceholderModule::DoAThing, PlaceholderMod)));
	Settings.push_back(std::make_shared<BoolSetting>(BoolSetting("placeholder_enabled", "Enable/disable the placeholder module.", true, false, std::bind(&PlaceholderModule::SetPlaceholderEnabled, PlaceholderMod, std::placeholders::_1))));
	PlaceholderMod->LoadSettings();

	Console.Write(GetNameFormatted() + std::to_string(Settings.size() - 1) + " Setting(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(Commands.size() - 1) + " Command(s) Initialized!");
}

class ManagerComponent Manager;