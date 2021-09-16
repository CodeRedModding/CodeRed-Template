#pragma once
#include "../Component.hpp"
#include "../Modules/Includes.hpp"
#include "../Extensions/Includes.hpp"

enum class SettingTypes : uint8_t
{
	TYPE_NONE = 0,
	TYPE_INT = 1,
	TYPE_BOOL = 2,
	TYPE_FLOAT = 3,
	TYPE_STRING = 4,
	TYPE_COLOR = 5,
	TYPE_VECTOR_3D = 6,
	TYPE_VECTOR_2D = 7
};

enum class CommandTypes : uint8_t
{
	TYPE_NONE = 0,
	TYPE_CALLBACK = 1,
	TYPE_ARGUMENT = 2
};

class Setting
{
private:
	std::string Name;										// Settings name.
	std::string Description;								// Settings description.
	std::string DefaultValue;								// Settings default value.
	std::string CurrentValue;								// Settings current value.
	SettingTypes Type;										// Settings underlying type.
	bool Modifiable;										// If the setting is modifiable/visible by the user.
	bool ShouldCallback;									// If the setting has a callback function.
	std::function<void(Setting*)> Callback;					// Callback function if the user has one bound.

public:
	Setting(const std::string& name, const std::string& description, const std::string& defaultValue, const SettingTypes valueType, const bool bModifiable);
	Setting(const std::string& name, const std::string& description, const std::string& defaultValue, const SettingTypes valueType, const bool bModifiable, std::function<void(Setting*)> callback);
	~Setting();

public:
	std::string GetName() const;
	std::string GetDescription() const;
	SettingTypes GetType() const;
	int32_t GetIntValue() const;
	bool GetBoolValue() const;
	float GetFloatValue() const;
	std::string GetStringValue() const;
	Color GetColorValue() const;
	LinearColor GetLinearValue() const;
	Vector GetVector3DValue() const;
	Vector2D GetVector2DValue() const;
	void SetValue(const std::string& value);
	void ResetToDefault();
	bool IsModifiable() const;
	bool HasCallback() const;
	void TriggerCallback();
	void BindCallback(const std::function<void(Setting*)>& callback);
	void UnbindCallback();
};

class Command
{
private:
	std::string Name;										// Commands name.
	std::string Description;								// Commands description.
	std::string Arguments;									// Commands possible arguments.
	CommandTypes Type;										// Commands underlying type.
	std::function<void()> Callback;							// Commands callback.
	std::function<void(std::string)> ArgumentCallback;		// Commands callback with arguments

public:
	Command(const std::string& name, const std::string& description);
	~Command();

public:
	std::string GetName() const;
	std::string GetDescription() const;
	CommandTypes GetType() const;
	void SetArguments(const std::string& arguments);
	void BindCallback(std::function<void()> callback);
	void BindArguments(std::function<void(std::string)> callback);
	void TriggerCallback();
	void TriggerCallback(const std::string& arguments);
};

// Manager component, manages commands, settings, and mods.

class ManagerComponent : public Component
{
private:
	std::unordered_map<std::string, std::shared_ptr<Command>> CommandMap;
	std::unordered_map<std::string, std::shared_ptr<Setting>> SettingMap;
	std::unordered_map<std::string, std::shared_ptr<Module>> ModuleMap;
	std::vector<std::pair<std::string, std::string>> CommandQueue;

public:
	std::shared_ptr<class PlaceholderModule> PlaceholderMod;

public:
	ManagerComponent();
	~ManagerComponent();

public:
	void KeyPressed(const std::string& key);
	void UnrealCommand(const std::string& unrealCommand);
	void ConsoleCommand(const std::string& command, const std::string& arguments);
	void AddToQueue(const std::string& command, const std::string& arguments); // Use this if you have ImGui interaction for console commands, as you CANNOT call Process Event on the ImGui render thread.
	void QueueTick(); // Checks the "Queue" vector to see if there are any commands that need to be sent through the "ConsoleCommand" function above.

public:
	void ResetSetting(const std::string& settingName, bool bLogToConsole = true);
	void PrintModule(const std::string& moduleName);
	template <typename T> std::shared_ptr<T> CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind);
	template <typename T> std::shared_ptr<T> GetModule(const std::string& moduleName);
	std::shared_ptr<Command> CreateCommand(Command* command);
	std::shared_ptr<Command> GetCommand(const std::string& commandName);
	std::shared_ptr<Setting> CreateSetting(Setting* setting);
	std::shared_ptr<Setting> GetSetting(const std::string& settingName);
	void Initialize(); // Creates all settings, commands, and modules.
};

extern class ManagerComponent Manager;