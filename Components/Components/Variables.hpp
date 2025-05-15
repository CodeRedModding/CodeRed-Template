#pragma once
#include "../Component.hpp"

namespace CodeRed
{
	enum SettingFlags : uint32_t
	{
		SETTING_None =			0UL,		// Default flags.
		SETTING_Hidden =		1UL << 0UL,	// Marks the setting as hidden, has no current use but it can be used to hide certain variables from ImGui windows.
		SETTING_Locked =		1UL << 1UL	// Prevents the user from setting a new value and will use the last set value until unlocked.
	};

	enum CommandFlags : uint32_t
	{
		COMMAND_None =			0UL,		// Default flags.
		COMMAND_Hidden =		1UL << 0UL,	// Marks the command as hidden, has no current use but it can be used to hide certain variables from ImGui windows.
		COMMAND_Locked =		1UL << 1UL,	// Prevents the command from triggering its callbacks by the user.
		COMMAND_NeedsArgs =		1UL << 2UL	// Command does not have optional arguments and requires input from the user, this is set to true by default.
	};

	enum class SettingTypes : uint8_t
	{
		None,
		Bool,
		Byte,
		Int32,
		Int64,
		Float,
		String,
		Color,
		Rotator,
		Vector2D,
		Vector3D
	};

	enum class VariableIds : uint32_t
	{
		UNKNOWN,

		// Default manager commands.
		MANAGER_RESET_SETTING,
		MANAGER_UNREAL_COMMAND,

		// User created variables.
		PLACEHOLDER_DO_THING,
		PLACEHOLDER_ENABLED,
		PLACEHOLDER_SOME_VALUE,

		END
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
	private: // Storage
		SettingTypes m_type;									// Settings underlying type.
		std::string m_description;								// Settings description.
		std::string m_defaultValue;								// Settings default value.
		std::string m_currentValue;								// Settings current value.
		std::pair<std::string, std::string> m_range;			// Settings minimum and maximum value range.
		std::map<VariableIds, bool> m_sharedSettings;			// Settings used as a reference in radio buttons.

	private: // Callbacks
		std::function<void()> m_callback;						// Callback function if one is bound.
		std::function<void(Setting*)> m_settingCallback;		// Optional callback containing a pointer to itself as an argument.
		std::function<void(std::string)> m_stringCallback;		// Optional callback containing the settings new string value as an argument.
		std::function<void(bool)> m_boolCallback;				// Optional callback containing the settings new bool value as an argument.

	public:
		Setting() = delete;
		Setting(VariableIds id, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bHidden = false);
		Setting(const Setting& setting);
		~Setting();

	public:
		SettingTypes GetType() const;
		bool IsType(SettingTypes settingType) const;
		const std::string& GetDescription() const;
		bool HasValue() const;
		bool HasRange() const;
		bool InRange(const std::string& sValue) const;
		bool IsValueValid(const std::string& sValue) const;
		bool IsHidden(bool bSkipDev = false) const;
		bool IsLocked() const;
		bool HasCallback() const;
		bool HasSettingCallback() const;
		bool HasStringCallback() const;
		bool HasBoolCallback() const;

	public: // Get and set value functions.
		const std::string& GetDefaultValue() const;
		const std::string& GetStringValue() const;
		bool GetBoolValue() const;
		uint8_t GetByteValue() const;
		int32_t GetInt32Value() const;
		int64_t GetInt64Value() const;
		float GetFloatValue() const;
		Color GetColorValue() const;
		LinearColor GetLinearValue() const;
		Rotator GetRotatorValue() const;
		VectorF GetVector3DFValue() const;
		VectorI GetVector3DIValue() const;
		Vector2DF GetVector2DFValue() const;
		Vector2DI GetVector2DIValue() const;
		Setting* ResetToDefault(ThreadTypes thread = ThreadTypes::Main);
		Setting* SetStringValue(const std::string& sValue, ThreadTypes thread = ThreadTypes::Main, bool bSkipSave = false);
		Setting* SetBoolValue(bool bValue, ThreadTypes thread = ThreadTypes::Main);
		Setting* SetInt32Value(int32_t iValue, ThreadTypes thread = ThreadTypes::Main);
		Setting* SetInt64Value(int64_t iValue, ThreadTypes thread = ThreadTypes::Main);
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
		Setting* SetByteRange(uint8_t minValue, uint8_t maxValue);
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
		void TriggerCallbacks(bool bSkipSave = false);

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

	class VariableComponent : public Component
	{
	private:
		std::map<VariableIds, std::string> m_variables;
		std::map<std::string, std::shared_ptr<Command>> m_commands;
		std::map<std::string, std::shared_ptr<Setting>> m_settings;
		bool m_variablesParsed;
		bool m_writingLocked; // Used when loading the users existing settings, so it doesn't create a read write loop.

	public:
		VariableComponent();
		~VariableComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;
		bool Initialize() override;

	public: // Variables.
		bool CreateVariable(std::string variableName, VariableIds variableId);
		std::string GetVariableName(VariableIds variableId) const;

	public: // Commands and settings.
		std::shared_ptr<Command> CreateCommand(const std::string& commandName, Command* command);
		std::shared_ptr<Command> GetCommand(const std::string& commandName);
		std::shared_ptr<Command> GetCommand(VariableIds variableId);
		std::shared_ptr<Setting> CreateSetting(const std::string& settingName, Setting* setting);
		std::shared_ptr<Setting> GetSetting(const std::string& settingName);
		std::shared_ptr<Setting> GetSetting(VariableIds variableId);

	public:
		void SetupVariables();
		void WriteVariables() const;
		void ResetSetting(const std::string& settingName, bool bPrintToConsole = true);
		void ToggleSetting(const std::string& settingName, bool bPrintToConsole = true);

	private:
		void FindUnusedVariables() const; // Helper function to see if you missed creating anything.
		void ParseVariables();
	};

	extern class VariableComponent Variables;
}