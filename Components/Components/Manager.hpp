#pragma once
#include "../Component.hpp"
#include "../Modules/Includes.hpp"

enum class ThreadTypes : uint8_t
{
	THREAD_GAME,
	THREAD_CANVAS,
	THREAD_RENDER,
	THREAD_UNIQUE
};

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
	bool IsValueValid(const std::string& value) const;
	Setting* SetValue(const std::string& value, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetRange(const std::string& minimumvalue, const std::string& maximumValue);
	bool HasCallback() const;
	bool HasArgumentCallback() const;
	Setting* BindCallback(std::function<void()> callback);
	Setting* BindCallback(std::function<void(std::string)> callback);
	Setting* UnbindCallbacks();
	void TriggerCallback() const;
	void ResetToDefault(ThreadTypes thread = ThreadTypes::THREAD_GAME);
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
	static inline std::map<std::string, VariableIds> VariableMap_SID;
	static inline std::map<VariableIds, std::string> VariableMap_IDS;

private:
	static inline std::unordered_map<std::string, std::shared_ptr<Module>> ModuleMap;
	static inline std::unordered_map<std::string, std::shared_ptr<Command>> CommandMap;
	static inline std::unordered_map<std::string, std::shared_ptr<Setting>> SettingMap;
	static inline std::vector<QueueData> CommandQueue;

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
	void QueueTick(); // Checks the "CommandQueue" vector to see if there are any commands that need to be sent through the "ConsoleCommand" function above.
	static void QueueCommand(const std::string& command, const std::string& arguments, bool bLogToConsole = true); // Use this if you have ImGui interaction for console commands, as you CANNOT call Process Event on the ImGui render thread.

public:
	static void ResetSetting(const std::string& settingName, bool bLogToConsole = true);
	static void PrintModule(const std::string& moduleName);
	static void CreateVariable(const std::string& name, VariableIds variable);
	static std::string GetVariableName(VariableIds variable);
	template <typename T> static std::shared_ptr<T> CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind);
	template <typename T> static std::shared_ptr<T> GetModule(const std::string& moduleName);
	static std::shared_ptr<Command> CreateCommand(Command* command);
	static std::shared_ptr<Command> GetCommand(const std::string& commandName);
	static std::shared_ptr<Command> GetCommand(VariableIds variable);
	static std::shared_ptr<Setting> CreateSetting(Setting* setting);
	static std::shared_ptr<Setting> GetSetting(const std::string& settingName);
	static std::shared_ptr<Setting> GetSetting(VariableIds variable);
	void Initialize(); // Creates all settings, commands, modules, and maps them out.
};

extern class ManagerComponent Manager;