#pragma once
#include "../Component.hpp"
#include "../Modules/Includes.hpp"

enum SettingFlags : uint32_t
{
	SETTING_None = 0U,
	SETTING_Hidden = 1U << 0U,		// Marks the setting as hidden, has no current use but it can be used to hide certain variables from ImGui windows.
	SETTING_Locked = 1U << 1U		// Prevents the user from setting a new value and will use the last set value until unlocked.
};

enum CommandFlags : uint32_t
{
	COMMAND_None = 0U,
	COMMAND_Hidden = 1U << 0U,		// Marks the command as hidden, has no current use but it can be used to hide certain variables from ImGui windows.
	COMMAND_Locked = 1U << 1U,		// Prevents the command from triggering its callbacks by the user.
	COMMAND_NeedsArgs = 1U << 2U	// Command does not have optional arguments and requires input from the user, this is set to true by default.
};

enum class SettingTypes : uint8_t
{
	None,
	Bool,
	Int32,
	Int64,
	Float,
	String,
	Color,
	Rotator,
	Vector2D,
	Vector3D
};

enum class CommandTypes : uint8_t
{
	None,
	Queued,
	Unrecognized,
	EmptyArguments,
	InvalidArguments,
	ModifySetting,
	PrintSetting
};

enum class VariableIds : int32_t
{
	// Default manager commands.
	MANAGER_RESET_SETTING,
	MANAGER_UNREAL_COMMAND,

	// User created variables.
	PLACEHOLDER_DO_THING,
	PLACEHOLDER_ENABLED,
	PLACEHOLDER_SOME_VALUE
};

class Variable
{
protected:
	VariableIds m_id;	// Internal identification.
	uint32_t m_flags;	// Internal permission flags.

public:
	Variable() = delete;
	Variable(VariableIds id, uint32_t flags);
	Variable(const Variable& variable);
	~Variable();

public:
	VariableIds GetId() const;
	std::string GetName() const;
	uint32_t GetFlags() const;

public:
	Variable& operator=(const Variable& variable);
};

class Setting : public Variable
{
private: // Storage.
	SettingTypes m_type;									// Settings underlying type.
	std::string m_description;								// Settings description.
	std::string m_defaultValue;								// Settings default value.
	std::string m_currentValue;								// Settings current value.
	std::pair<std::string, std::string> m_range;			// Settings minimum and maximum value range.
	std::map<VariableIds, bool> m_sharedSettings;			// Settings used as a reference in radio buttons.

private: // Callbacks.
	std::function<void()> m_callback;						// Callback function if one is bound.
	std::function<void(Setting*)> m_settingCallback;		// Optional callback containing a pointer to itself as an argument.
	std::function<void(bool)> m_toggleCallback;				// Optional callback containing the settings new bool value as an argument.
	std::function<void(std::string)> m_stringCallback;		// Optional callback containing the settings new string value as an argument.

public:
	Setting() = delete;
	Setting(VariableIds id, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bHidden = false);
	Setting(const Setting& setting);
	~Setting();

public:
	SettingTypes GetType() const;
	const std::string& GetDescription() const;
	bool HasValue() const;
	bool HasRange() const;
	bool InRange(const std::string& sValue) const;
	bool IsValueValid(const std::string& sValue) const;
	bool IsHidden(bool bSkipDev = false) const;
	bool IsLocked() const;
	bool HasCallback() const;
	bool HasSettingCallback() const;
	bool HasToggleCallback() const;
	bool HasArgumentCallback() const;

public: // Get and set value functions.
	const std::string& GetDefaultValue() const;
	const std::string& GetStringValue() const;
	int32_t GetInt32Value() const;
	int64_t GetInt64Value() const;
	bool GetBoolValue() const;
	float GetFloatValue() const;
	Color GetColorValue() const;
	LinearColor GetLinearValue() const;
	Rotator GetRotatorValue() const;
	VectorF GetVector3DFValue() const;
	VectorI GetVector3DIValue() const;
	Vector2DF GetVector2DFValue() const;
	Vector2DI GetVector2DIValue() const;
	Setting* ResetToDefault(ThreadTypes thread = ThreadTypes::Main);
	Setting* SetStringValue(const std::string& sValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetInt32Value(int32_t iValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetInt64Value(int64_t iValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetBoolValue(bool bValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetFloatValue(float fValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetColorValue(const Color& cValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetLinearValue(const LinearColor& lValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetRotatorValue(const Rotator& rValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetVectorFValue(const VectorF& vfValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetVectorIValue(const VectorI& viValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetVector2DFValue(const Vector2DF& vfValue, ThreadTypes thread = ThreadTypes::Main);
	Setting* SetVector2DIValue(const Vector2DI& viValue, ThreadTypes thread = ThreadTypes::Main);

public: // Get and set range functions.
	Setting* RemoveRange();
	Setting* SetStringRange(const std::string& minValue, const std::string& maxValue);
	Setting* SetInt32Range(int32_t minValue, int32_t maxValue);
	Setting* SetInt64Range(int64_t minValue, int64_t maxValue);
	Setting* SetFloatRange(float minValue, float maxValue);
	Setting* SetRotatorRange(const Rotator& minValue, const Rotator& maxValue);
	Setting* SetVectorFRange(const VectorF& minValue, const VectorF& maxValue);
	Setting* SetVectorIRange(const VectorI& minValue, const VectorI& maxValue);
	Setting* SetVector2DFRange(const Vector2DF& minValue, const Vector2DF& maxValue);
	Setting* SetVector2DIRange(const Vector2DI& minValue, const Vector2DI& maxValue);
	std::pair<int32_t, int32_t> GetInt32Range() const;
	std::pair<int64_t, int64_t> GetInt64Range() const;
	std::pair<float, float> GetFloatRange() const;
	std::pair<Rotator, Rotator> GetRotatorRange() const;
	std::pair<VectorF, VectorF> GetVectorFRange() const;
	std::pair<VectorI, VectorI> GetVectorIRange() const;
	std::pair<Vector2DF, Vector2DF> GetVector2DFRange() const;
	std::pair<Vector2DI, Vector2DI> GetVector2DIRange() const;

public: // Flag and callback settings.
	Setting* SetHidden(bool bHidden);
	Setting* SetLocked(bool bLocked);
	Setting* LockWithValue(const std::string& sValue);
	Setting* UnbindCallbacks();
	Setting* BindCallback(const std::function<void()>& callback);
	Setting* BindSettingCallback(const std::function<void(Setting*)>& callback);
	Setting* BindBoolCallback(const std::function<void(bool)>& callback);
	Setting* BindStringCallback(const std::function<void(std::string)>& callback);
	void TriggerCallbacks();

public:
	Setting& operator=(const Setting& setting);
};

class Command : public Variable
{
private:
	std::string m_description;								// Commands description.
	std::function<void()> m_callback;						// Commands callback.
	std::function<void(std::string)> m_stringCallback;		// Commands callback with string arguments.

public:
	Command() = delete;
	Command(VariableIds id, const std::string& description, bool bHidden = false);
	Command(const Command& command);
	~Command();

public:
	const std::string& GetDescription() const;
	bool IsHidden() const;
	bool IsLocked() const;
	bool NeedsArgs() const;
	bool HasCallback() const;
	bool HasArgumentCallback() const;
	void TriggerCallback() const;
	void TriggerCallback(const std::string& arguments) const;

public:
	Command* SetHidden(bool bHidden);
	Command* SetLocked(bool bLocked);
	Command* SetNeedsArgs(bool bNeedsArgs);
	Command* BindCallback(const std::function<void()>& callback);
	Command* BindStringCallback(const std::function<void(std::string)>& callback);
	Command* UnbindCallbacks();

public:
	Command& operator=(const Command& command);
};

class QueueData
{
public:
	ThreadTypes Thread;
	std::string Command;
	std::string Arguments;
	uint64_t Delay;
	uint64_t Delta;
	bool Internal;
	bool Completed;

public:
	QueueData() = delete;
	QueueData(const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave = false);
	QueueData(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier = 60, bool bSkipSave = false);
	QueueData(const QueueData& queueData);
	~QueueData();

public:
	bool HasArguments() const;
	bool IsAsync() const;
	bool OnTick();

public:
	QueueData& operator=(const QueueData& queueData);
};

// Manages variables, commands, settings, and modules.
class ManagerComponent : public Component
{
private:
	std::map<VariableIds, std::string> m_variables;
	std::map<std::string, std::shared_ptr<Module>> m_modules;
	std::map<std::string, std::shared_ptr<Command>> m_commands;
	std::map<std::string, std::shared_ptr<Setting>> m_settings;
	std::vector<QueueData> m_threadQueue;
	std::vector<QueueData> m_queue;
	std::atomic<bool> m_queueLocked; // Used for preventing a thread racing issue when calling commands from different threads.

public:
	std::shared_ptr<PlaceholderModule> PlaceholderMod;

public:
	ManagerComponent();
	~ManagerComponent() override;

public:
	void OnCreate() override;
	void OnDestroy() override;
	void OnTick(); // Checks the queue to see if there are any commands or settings that need to be processed.
	void OnCanvasDraw(class UCanvas* unrealCanvas); // Calls all canvas drawing functions in each module, called 60 times every second on the games main thread.

private:
	bool CreateVariable(std::string name, VariableIds variable);
	void QueueCommand(const QueueData& queueData, ThreadTypes thread);
	void ProcessCommand(const QueueData& queueData, ThreadTypes thread = ThreadTypes::Main);
	std::pair<CommandTypes, std::string> ProcessCommandInternal(const QueueData& queueData, ThreadTypes thread = ThreadTypes::Main);

public:
	std::pair<CommandTypes, std::string> InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::Main);
	std::pair<CommandTypes, std::string> InternalCommand(const std::string& command, ThreadTypes thread = ThreadTypes::Main);
	void ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::Main);
	void ConsoleCommand(const std::string& command, ThreadTypes thread = ThreadTypes::Main);
	void AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread = ThreadTypes::Main);
	void AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread = ThreadTypes::Main);
	void UnrealCommand(std::string unrealCommand, bool bPrintToConsole = true);

public:
	void ResetSetting(const std::string& settingName, bool bLogToConsole = true);
	std::string GetVariableName(VariableIds variable);
	template <typename T> std::shared_ptr<T> CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind);
	template <typename T> std::shared_ptr<T> GetModule(const std::string& moduleName)
	{
		if (m_modules.contains(moduleName))
		{
			return std::static_pointer_cast<T>(m_modules[moduleName]);
		}

		return nullptr;
	}
	std::shared_ptr<Command> CreateCommand(const std::string& commandName, Command* command);
	std::shared_ptr<Command> GetCommand(const std::string& commandName);
	std::shared_ptr<Command> GetCommand(VariableIds variable);
	std::shared_ptr<Setting> CreateSetting(const std::string& commandName, Setting* setting);
	std::shared_ptr<Setting> GetSetting(const std::string& settingName);
	std::shared_ptr<Setting> GetSetting(VariableIds variable);
	void Initialize(); // Creates all settings, commands, modules, and maps them out.
};

extern class ManagerComponent Manager;