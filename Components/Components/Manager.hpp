#pragma once
#include "../Component.hpp"
#include "../Modules/Includes.hpp"

// Underlying Class Types.

enum class SettingTypes : uint8_t
{
	TYPE_NONE = 0,
	TYPE_INT = 1,
	TYPE_BOOL = 2,
	TYPE_FLOAT = 3,
	TYPE_STRING = 4,
	TYPE_COLOR = 5
};

enum class ColorTypes : uint8_t
{
	TYPE_RGBA = 0,
	TYPE_DECIMAL = 1,
	TYPE_COLOR = 2,
	TYPE_LINEAR = 3,
	TYPE_HEX = 4
};

enum class CommandTypes : uint8_t
{
	TYPE_NONE = 0,
	TYPE_CALLBACK = 1,
	TYPE_ARGUMENT = 2
};

// Setting Base Class.

class Setting
{
private:
	std::string Name;				// Settings name.
	std::string Description;		// Settings description.
	SettingTypes Type;				// Settings underlying type.
	bool Modifiable;				// If the setting is modifiable by the user.
	bool Hidden;					// If the setting is hidden from the user.
	bool ShouldCallback;			// If the setting has a callback function.

public:
	Setting(const std::string& name, const std::string& description, const bool& bModifiable, const SettingTypes type);
	~Setting();

public:
	std::string GetName() const;
	std::string GetDescription() const;
	SettingTypes GetType() const;
	bool IsModifiable() const;
	bool IsHidden() const;
	bool HasCallback() const;
	void SetShouldCallback(const bool& bShouldCallback);
};

// Int Setting Sub-Class.

class IntSetting : public Setting
{
private:
	std::function<void(class IntSetting)> Callback;
	int32_t Value;

public:
	IntSetting(const std::string& name, const std::string& description, bool bModifiable, const int& value, std::function<void(class IntSetting)> callback);
	IntSetting(const std::string& name, const std::string& description, bool bModifiable, const int& value);
	IntSetting(const std::string& name, const std::string& description, bool bModifiable);
	~IntSetting();

public:
	void SetValue(const int& value);
	int GetValue() const;
	void TriggerCallback() const;
};

// Float Setting Sub-Class.

class FloatSetting : public Setting
{
private:
	std::function<void(class FloatSetting)> Callback;
	float Value;

public:
	FloatSetting(const std::string& name, const std::string& description, bool bModifiable, const float& value, std::function<void(class FloatSetting)> callback);
	FloatSetting(const std::string& name, const std::string& description, bool bModifiable, const float& value);
	FloatSetting(const std::string& name, const std::string& description, bool bModifiable);
	~FloatSetting();

public:
	void SetValue(const float& value);
	float GetValue() const;
	void TriggerCallback() const;
};

// Bool Setting Sub-Class.

class BoolSetting : public Setting
{
private:
	std::function<void(class BoolSetting)> Callback;
	bool Value;

public:
	BoolSetting(const std::string& name, const std::string& description, bool bModifiable, const bool& value, std::function<void(class BoolSetting)> callback);
	BoolSetting(const std::string& name, const std::string& description, bool bModifiable, const bool& value);
	BoolSetting(const std::string& name, const std::string& description, bool bModifiable);
	~BoolSetting();

public:
	void SetValue(const bool& value);
	bool GetValue() const;
	void TriggerCallback() const;
};

// String Setting Sub-Class.

class StringSetting : public Setting
{
private:
	std::function<void(class StringSetting)> Callback;
	std::string Value;

public:
	StringSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& value, std::function<void(class StringSetting)> callback);
	StringSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& value);
	StringSetting(const std::string& name, const std::string& description, bool bModifiable);
	~StringSetting();

public:
	void SetValue(const std::string& value);
	std::string GetValue() const;
	std::vector<std::string> GetSplitValue(const char split);
	void TriggerCallback() const;
};

// Color Setting Sub-Class.

class ColorSetting : public Setting
{
private:
	std::function<void(class ColorSetting)> Callback;
	int32_t R, G, B, A;
	int32_t DecimalValue;
	struct FColor ColorValue;
	struct FLinearColor LinearValue;
	std::string HexValue;

public:
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a, std::function<void(class ColorSetting)> callback);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, int32_t decimalValue, std::function<void(class ColorSetting)> callback);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, int32_t decimalValue);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FColor& colorValue, std::function<void(class ColorSetting)> callback);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FColor& colorValue);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FLinearColor& linearValue, std::function<void(class ColorSetting)> callback);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const struct FLinearColor& linearValue);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& hexValue, std::function<void(class ColorSetting)> callback);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable, const std::string& hexValue);
	ColorSetting(const std::string& name, const std::string& description, bool bModifiable);
	~ColorSetting();

public:
	void SetValue(const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a);
	void SetValue(const int32_t& decimalValue);
	void SetValue(const struct FColor& colorValue);
	void SetValue(const struct FLinearColor& linearValue);
	void SetValue(const std::string& hexValue);
	void GetRGBAValue(int32_t& r, int32_t& g, int32_t& b, int32_t& a) const;
	int32_t GetDecimalValue() const;
	struct FColor GetColorValue() const;
	struct FLinearColor GetLinearValue() const;
	std::string GetHexValue() const;
	void TriggerCallback() const;

private:
	std::string ToHex();
	void InitializeValues(const ColorTypes& type);
};

// Command Base Class.

class Command
{
private:
	std::string Name;				// Commands name.
	std::string Description;		// Commands description.
	CommandTypes Type;				// Commands underlying type.
	bool Hidden;					// If the command is hidden from the user.

public:
	Command(const std::string& name, const std::string& description, const bool& bHidden, const CommandTypes type);
	~Command();

public:
	std::string GetName() const;
	std::string GetDescription() const;
	CommandTypes GetType() const;
	bool IsHidden() const;
};

// Callback Command Sub-Class.

class CallbackCommand : public Command
{
private:
	std::function<void()> Callback;

public:
	CallbackCommand(const std::string& name, const std::string& description, const bool& bHidden, std::function<void()> callback);
	~CallbackCommand();

public:
	void TriggerCallback() const;
};

// Argument Command Sub-Class.

class ArgumentCommand : public Command
{
private:
	std::function<void(std::string)> Callback;

public:
	ArgumentCommand(const std::string& name, const std::string& description, const bool& bHidden, std::function<void(std::string)> callback);
	~ArgumentCommand();

public:
	void TriggerCallback(const std::string& arguments) const;
};

// Manager component, manages commands, settings, and mods.

class ManagerComponent : public Component
{
private:
	std::vector<std::shared_ptr<class Command>> Commands;
	std::vector<std::shared_ptr<class Setting>> Settings;

public:
	std::shared_ptr<class PlaceholderModule> PlaceholderMod;

public:
	ManagerComponent();
	~ManagerComponent();

public:
	void KeyPressed(const std::string& key);
	void UnrealCommand(const std::string& unrealCommand);
	void ConsoleCommand(const std::string& command, const std::string& arguments);

public:
	void ModifySetting(std::shared_ptr<class Setting>& setting, const std::string& arguments);

	template<typename T> // Returns a command from the "Commands" vector based on its name.
	std::shared_ptr<T> GetCommand(const std::string& commandName) const
	{
		for (auto commandIt = Commands.begin(); commandIt != Commands.end(); commandIt++)
		{
			if ((*commandIt)->GetName() == commandName)
			{
				return std::reinterpret_pointer_cast<T>(*commandIt);
			}
		}

		return GetCommand<Command>("command_not_found");
	}

	template<typename T> // Returns a seting from the "Settings" vector based on its name.
	std::shared_ptr<T> GetSetting(const std::string& settingName) const
	{
		for (auto settingIt = Settings.begin(); settingIt != Settings.end(); settingIt++)
		{
			if ((*settingIt)->GetName() == settingName)
			{
				return std::reinterpret_pointer_cast<T>(*settingIt);
			}
		}

		return GetSetting<Setting>("setting_not_found");
	}

	std::string PhraseArguments(std::string arguments);
	std::vector<std::string> SplitArguments(const std::string& arguments); // Takes every word out of the argument string and adds it to the return vector.
	void Initialize(); // Creates all settings, commands, and modules.
};

extern class ManagerComponent Manager;