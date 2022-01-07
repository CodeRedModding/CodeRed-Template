#pragma once
#include "../Component.hpp"
#include "../Modules/Includes.hpp"

enum class SettingTypes : uint8_t
{
	TYPE_NONE,
	TYPE_INT,
	TYPE_BOOL,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_COLOR,
	TYPE_VECTOR_3D,
	TYPE_VECTOR_2D
};

enum class VariableIds : int32_t
{
	// Default manager commands.
	MANAGER_RESET_SETTING,
	MANAGER_PRINT_MODULE,
	MANAGER_UNREAL_COMMAND,
	// User created variables.
	PLACEHOLDER_DO_THING,
	PLACEHOLDER_ENABLED,
	PLACEHOLDER_SOME_VALUE
};

class Setting
{
private:
	VariableIds Variable;									// Settings identification.
	SettingTypes Type;										// Settings underlying type.
	std::string Description;								// Settings description.
	std::string DefaultValue;								// Settings default value.
	std::string CurrentValue;								// Settings current value.
	std::pair<std::string, std::string> Range;				// Settings minimum and maximum value range.
	bool Modifiable;										// If the setting is modifiable/visible by the user.
	std::function<void()> Callback;							// Callback function if the user has one bound.
	std::function<void(std::string)> ArgumentCallback;		// Argument callback function if the user has one bound.

public:
	Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable);
	Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable, std::function<void()> callback);
	Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable, std::function<void(std::string)> callback);
	~Setting();

public:
	VariableIds GetId() const;
	SettingTypes GetType() const;
	std::string GetName() const;
	std::string GetDescription() const;
	int32_t GetIntValue() const;
	bool GetBoolValue() const;
	float GetFloatValue() const;
	std::string GetDefaultValue() const;
	std::string GetStringValue() const;
	Color GetColorValue() const;
	LinearColor GetLinearValue() const;
	VectorF GetVector3DValue() const;
	Vector2DF GetVector2DValue() const;

public:
	bool IsModifiable() const;
	bool HasRange() const;
	bool InRange(const std::string& value) const;
	Setting* SetValue(const std::string& value);
	Setting* SetRange(const std::string& minimumvalue, const std::string& maximumValue);
	bool HasCallback() const;
	bool HasArgumentCallback() const;
	Setting* BindCallback(std::function<void()> callback);
	Setting* BindCallback(std::function<void(std::string)> callback);
	Setting* UnbindCallbacks();
	void TriggerCallback() const;
	void ResetToDefault();
};

class Command
{
private:
	VariableIds Variable;									// Commands identification.
	std::string Description;								// Commands description.
	bool Searchable;										// If the command can be searched in the console.
	std::function<void()> Callback;							// Commands callback.
	std::function<void(std::string)> ArgumentCallback;		// Commands callback with arguments

public:
	Command(VariableIds variable, const std::string& description, bool bSearchable = true);
	~Command();

public:
	VariableIds GetId() const;
	std::string GetName() const;
	std::string GetDescription() const;
	bool IsSearchable() const;
	bool HasCallback() const;
	bool HasArgumentCallback() const;
	Command* BindCallback(std::function<void()> callback);
	Command* BindCallback(std::function<void(std::string)> callback);
	Command* UnbindCallbacks();
	void TriggerCallback() const;
	void TriggerCallback(const std::string& arguments) const;
};

class QueueData
{
public:
	std::string Command;
	std::string Arguments;
	bool LogToConsole;

public:
	QueueData();
	QueueData(const std::string& command, const std::string& arguments, bool bLogToConsole = true);
	~QueueData();
};

// Manages variables, commands, settings, and modules.
class ManagerComponent : public Component
{
private:
	std::map<std::string, VariableIds> VariableMap_SID;
	std::map<VariableIds, std::string> VariableMap_IDS;

private:
	std::unordered_map<std::string, std::shared_ptr<Module>> ModuleMap;
	std::unordered_map<std::string, std::shared_ptr<Command>> CommandMap;
	std::unordered_map<std::string, std::shared_ptr<Setting>> SettingMap;
	std::vector<QueueData> CommandQueue;

public:
	std::shared_ptr<PlaceholderModule> PlaceholderMod;

public:
	ManagerComponent();
	~ManagerComponent() override;

public:
	void OnCreate() override;
	void OnDestroy() override;

public:
	void UnrealCommand(const std::string& unrealCommand, bool bLogToConsole = true);
	void ConsoleCommand(const std::string& command, const std::string& arguments, bool bLogToConsole = true);
	void QueueCommand(const std::string& command, const std::string& arguments, bool bLogToConsole = true); // Use this if you have ImGui interaction for console commands, as you CANNOT call Process Event on the ImGui render thread.
	void QueueTick(); // Checks the "CommandQueue" vector to see if there are any commands that need to be sent through the "ConsoleCommand" function above.

public:
	void ResetSetting(const std::string& settingName, bool bLogToConsole = true);
	void PrintModule(const std::string& moduleName);
	void CreateVariable(const std::string& name, VariableIds variable);
	std::string GetVariableName(VariableIds variable);
	template <typename T> std::shared_ptr<T> CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind);
	template <typename T> std::shared_ptr<T> GetModule(const std::string& moduleName);
	std::shared_ptr<Command> CreateCommand(Command* command);
	std::shared_ptr<Command> GetCommand(const std::string& commandName);
	std::shared_ptr<Command> GetCommand(VariableIds variable);
	std::shared_ptr<Setting> CreateSetting(Setting* setting);
	std::shared_ptr<Setting> GetSetting(const std::string& settingName);
	std::shared_ptr<Setting> GetSetting(VariableIds variable);
	void Initialize(); // Creates all settings, commands, modules, and maps them out.
};

extern class ManagerComponent Manager;