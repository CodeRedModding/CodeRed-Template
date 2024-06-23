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
	TYPE_BOOL,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_COLOR,
	TYPE_ROTATOR,
	TYPE_VECTOR_3D,
	TYPE_VECTOR_2D
};

enum class CommandTypes : uint8_t
{
	TYPE_NONE,
	TYPE_QUEUED,
	TYPE_UNRECOGNIZED,
	TYPE_EMPTY_ARGUMENTS,
	TYPE_INVALID_ARGUMENTS,
	TYPE_MODIFY_SETTING,
	TYPE_PRINT_SETTING
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

class Setting
{
private: // Storage.
	VariableIds m_variable;									// Settings identification.
	SettingTypes m_type;									// Settings underlying type.
	std::string m_description;								// Settings description.
	std::string m_defaultValue;								// Settings default value.
	std::string m_currentValue;								// Settings current value.
	std::pair<std::string, std::string> m_range;			// Settings minimum and maximum value range.
	bool m_modifiable;										// If the setting is modifiable/visible by the user.

public: // Callbacks.
	std::function<void()> m_callback;						// Callback function if the user has one bound.
	std::function<void(std::string)> m_argumentCallback;	// Argument callback function if the user has one bound.

public:
	Setting() = delete;
	Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable);
	Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, const std::function<void()>& callback);
	Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, const std::function<void(std::string)>& callback);
	Setting(const Setting& setting);
	~Setting();

public:
	VariableIds GetId() const;
	SettingTypes GetType() const;
	std::string GetName() const;
	const std::string& GetDescription() const;
	bool HasRange() const;
	bool InRange(const std::string& sValue) const;
	bool IsValueValid(const std::string& sValue) const;
	bool IsModifiable() const;
	bool HasCallback() const;
	bool HasArgumentCallback() const;

public:
	const std::string& GetDefaultValue() const;
	const std::string& GetStringValue() const;
	int32_t GetIntValue() const;
	bool GetBoolValue() const;
	float GetFloatValue() const;
	Color GetColorValue() const;
	LinearColor GetLinearValue() const;
	Rotator GetRotatorValue() const;
	VectorF GetVector3DFValue() const;
	VectorI GetVector3DIValue() const;
	Vector2DF GetVector2DFValue() const;
	Vector2DI GetVector2DIValue() const;
	Setting* ResetToDefault(ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetStringValue(const std::string& sValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetIntValue(int32_t iValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetBoolValue(bool bValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetFloatValue(float fValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetColorValue(const Color& cValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetLinearValue(const LinearColor& lValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetRotatorValue(const Rotator& rValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetVectorFValue(const VectorF& vfValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetVectorIValue(const VectorI& viValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetVector2DFValue(const Vector2DF& vfValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	Setting* SetVector2DIValue(const Vector2DI& viValue, ThreadTypes thread = ThreadTypes::THREAD_GAME);

public:
	Setting* RemoveRange();
	Setting* SetStringRange(const std::string& minValue, const std::string& maxValue);
	Setting* SetIntRange(int32_t minValue, int32_t maxValue);
	Setting* SetFloatRange(float minValue, float maxValue);
	Setting* SetRotatorRange(const Rotator& minValue, const Rotator& maxValue);
	Setting* SetVectorFRange(const VectorF& minValue, const VectorF& maxValue);
	Setting* SetVectorIRange(const VectorI& minValue, const VectorI& maxValue);
	Setting* SetVector2DFRange(const Vector2DF& minValue, const Vector2DF& maxValue);
	Setting* SetVector2DIRange(const Vector2DI& minValue, const Vector2DI& maxValue);
	std::pair<int32_t, int32_t> GetIntRange() const;
	std::pair<float, float> GetFloatRange() const;
	std::pair<Rotator, Rotator> GetRotatorRange() const;
	std::pair<VectorF, VectorF> GetVectorFRange() const;
	std::pair<VectorI, VectorI> GetVectorIRange() const;
	std::pair<Vector2DF, Vector2DF> GetVector2DFRange() const;
	std::pair<Vector2DI, Vector2DI> GetVector2DIRange() const;

public:
	Setting* UnbindCallbacks();
	Setting* BindCallback(const std::function<void()>& callback);
	Setting* BindCallback(const std::function<void(std::string)>& callback);
	void TriggerCallbacks() const;

public:
	Setting& operator=(const Setting& setting);
};

class Command
{
private: // Storage.
	VariableIds m_variable;									// Commands identification.
	std::string m_description;								// Commands description.
	bool m_searchable;										// If the command can be searched in the console.

public: // Callbacks.
	std::function<void()> m_callback;						// Commands callback.
	std::function<void(std::string)> m_argumentCallback;	// Commands callback with arguments

public:
	Command() = delete;
	Command(VariableIds variable, const std::string& description, bool bSearchable = true);
	Command(const Command& command);
	~Command();

public:
	VariableIds GetId() const;
	std::string GetName() const;
	const std::string& GetDescription() const;
	bool IsSearchable() const;
	bool HasCallback() const;
	bool HasArgumentCallback() const;
	Command* UnbindCallbacks();
	Command* BindCallback(const std::function<void()>& callback);
	Command* BindCallback(const std::function<void(std::string)>& callback);
	void TriggerCallback() const;
	void TriggerCallback(const std::string& arguments) const;

public:
	Command& operator=(const Command& command);
};

class QueueData
{
public:
	std::string Command;
	std::string Arguments;
	uint64_t Delay;
	uint64_t Delta;
	bool Internal;
	bool Expired;

public:
	QueueData();
	QueueData(const std::string& command, const std::string& arguments, bool bInternal);
	QueueData(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier = 60); // Multiplier depends on the tick rate at which "ManagerComponent::OnTick()" is called.
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
	std::vector<QueueData> m_queue;

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
	std::pair<CommandTypes, std::string> InternalCommand(const QueueData& queueData, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	std::pair<CommandTypes, std::string> InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	std::pair<CommandTypes, std::string> InternalCommand(const std::string& command, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void ConsoleCommand(const QueueData& queueData, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void ConsoleCommand(const std::string& command, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread = ThreadTypes::THREAD_GAME);
	void OnTick(); // Checks the "CommandQueue" vector to see if there are any commands that need to be sent through the "ConsoleCommand" function above.

public:
	void ResetSetting(const std::string& settingName, bool bLogToConsole = true);
	void CreateVariable(std::string name, VariableIds variable);
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
	std::shared_ptr<Command> CreateCommand(Command* command);
	std::shared_ptr<Command> GetCommand(const std::string& commandName);
	std::shared_ptr<Command> GetCommand(VariableIds variable);
	std::shared_ptr<Setting> CreateSetting(Setting* setting);
	std::shared_ptr<Setting> GetSetting(const std::string& settingName);
	std::shared_ptr<Setting> GetSetting(VariableIds variable);
	void Initialize(); // Creates all settings, commands, modules, and maps them out.
};

extern class ManagerComponent Manager;