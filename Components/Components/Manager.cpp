#include "Manager.hpp"
#include "../Includes.hpp"

Setting::Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable) :
	Variable(variable),
	Description(description),
	Type(valueType),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(nullptr),
	ArgumentCallback(nullptr)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") != std::string::npos || CurrentValue.find("0x") != std::string::npos)
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable, std::function<void()> callback) :
	Variable(variable),
	Type(valueType),
	Description(description),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(callback),
	ArgumentCallback(nullptr)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") != std::string::npos || CurrentValue.find("0x") != std::string::npos)
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::Setting(VariableIds variable, const std::string& description, const std::string& defaultValue, SettingTypes valueType, bool bModifiable, std::function<void(std::string)> callback) :
	Variable(variable),
	Type(valueType),
	Description(description),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(nullptr),
	ArgumentCallback(callback)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") != std::string::npos || CurrentValue.find("0x") != std::string::npos)
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::~Setting() {}

VariableIds Setting::GetId() const
{
	return Variable;
}

SettingTypes Setting::GetType() const
{
	return Type;
}

std::string Setting::GetName() const
{
	return Manager.GetVariableName(Variable);
}

std::string Setting::GetDescription() const
{
	return Description;
}

int32_t Setting::GetIntValue() const
{
	if (GetType() == SettingTypes::TYPE_INT)
	{
		return std::stoi(CurrentValue);
	}

	return 0;
}

bool Setting::GetBoolValue() const
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		if (CurrentValue == "true" || CurrentValue == "1")
		{
			return true;
		}
	}

	return false;
}

float Setting::GetFloatValue() const
{
	if (GetType() == SettingTypes::TYPE_FLOAT)
	{
		return std::stof(CurrentValue);
	}

	return 0.0f;
}

std::string Setting::GetDefaultValue() const
{
	return DefaultValue;
}

std::string Setting::GetStringValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Colors::DecimalToHex(std::stoi(CurrentValue), true);
	}

	return CurrentValue;
}

Color Setting::GetColorValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Colors::DecimalToColor(std::stoi(CurrentValue));
	}

	return Color();
}

LinearColor Setting::GetLinearValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Colors::DecimalToLinear(std::stoi(CurrentValue));
	}

	return LinearColor();
}

VectorF Setting::GetVector3DValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D)
	{
		std::vector<std::string> values = Format::SplitArguments(GetStringValue());
		VectorF returnVector;

		if (values.size() == 3)
		{
			returnVector.X = std::stof(values[0]);
			returnVector.Y = std::stof(values[1]);
			returnVector.Z = std::stof(values[2]);
		}

		return returnVector;
	}

	return VectorF();
}

Vector2DF Setting::GetVector2DValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D || GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		std::vector<std::string> values = Format::SplitArguments(GetStringValue());
		Vector2DF returnVector;

		if (values.size() >= 2)
		{
			returnVector.X = std::stof(values[0]);
			returnVector.Y = std::stof(values[1]);
		}

		return returnVector;
	}

	return Vector2DF();
}

bool Setting::IsModifiable() const
{
	return Modifiable;
}

bool Setting::HasRange() const
{
	return (!Range.first.empty() && !Range.second.empty());
}

bool Setting::InRange(const std::string& value) const
{
	if (HasRange())
	{
		if (IsValueValid(value))
		{
			if (GetType() == SettingTypes::TYPE_INT)
			{
				int32_t formattedMin = std::stoi(Range.first);
				int32_t formattedMax = std::stoi(Range.second);
				int32_t formattedValue = std::stoi(value);
				return (formattedValue >= formattedMin && formattedValue <= formattedMax);
			}
			else if (GetType() == SettingTypes::TYPE_BOOL)
			{
				return (value == "0" || value == "false" || value == "1" || value == "true");
			}
			else if (GetType() == SettingTypes::TYPE_FLOAT)
			{
				float formattedMin = std::stof(Range.first);
				float formattedMax = std::stof(Range.second);
				float formattedValue = std::stof(value);
				return (formattedValue >= formattedMin && formattedValue <= formattedMax);
			}
		}
	}

	return true;
}

bool Setting::IsValueValid(const std::string& value) const
{
	if (GetType() == SettingTypes::TYPE_INT)
	{
		if (Format::IsStringNumber(value))
		{
			return true;
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports 32 bit integer values.");
			return false;
		}
	}
	else if (GetType() == SettingTypes::TYPE_BOOL)
	{
		if (value == "1"
			|| value == "true"
			|| value == "0"
			|| value == "false")
		{
			return true;
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports true or false values.");
			return false;
		}
	}
	else if (GetType() == SettingTypes::TYPE_FLOAT)
	{
		if (Format::IsStringFloat(value) || Format::IsStringNumber(value))
		{
			return true;
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports floating point numbers or optionally 32 bit integer values.");
			return false;
		}
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (value.find("#") == 0 || Format::IsStringNumber(value))
		{
			return true;
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports RGB hexadecimal or RGB decimal values.");
			return false;
		}
	}

	return true;
}

Setting* Setting::SetValue(const std::string& value, ThreadTypes thread)
{
	if (thread != ThreadTypes::THREAD_GAME)
	{
		ManagerComponent::QueueCommand(this->GetName(), value, (thread == ThreadTypes::THREAD_RENDER ? false : true));
	}
	else if (IsValueValid(value))
	{
		if (InRange(value))
		{
			if (GetType() == SettingTypes::TYPE_BOOL)
			{
				if (value == "1" || value == "true")
				{
					CurrentValue = "true";
				}
				else
				{
					CurrentValue = "false";
				}
			}
			else if (GetType() == SettingTypes::TYPE_COLOR)
			{
				if (value.find("#") == 0)
				{
					CurrentValue = std::to_string(Colors::HexToDecimal(value));
				}
				else
				{
					CurrentValue = value;
				}

				TriggerCallback();
			}
			else
			{
				CurrentValue = value;
			}

			TriggerCallback();
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is out of range, this setting has a minimum value of \"" + Range.first + "\" and a maximum value of \"" + Range.second + "\".");
		}
	}

	return this;
}

Setting* Setting::SetRange(const std::string& minimumvalue, const std::string& maximumValue)
{
	Range = std::make_pair(minimumvalue, maximumValue);
	return this;
}

bool Setting::HasCallback() const
{
	return (!!Callback);
}

bool Setting::HasArgumentCallback() const
{
	return (!!ArgumentCallback);
}

Setting* Setting::BindCallback(std::function<void()> callback)
{
	Callback = callback;
	return this;
}

Setting* Setting::BindCallback(std::function<void(std::string)> callback)
{
	ArgumentCallback = callback;
	return this;
}

Setting* Setting::UnbindCallbacks()
{
	Callback = nullptr;
	ArgumentCallback = nullptr;
	return this;
}

void Setting::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback();
	}
	else if (HasArgumentCallback())
	{
		ArgumentCallback(GetStringValue());
	}
}

void Setting::ResetToDefault(ThreadTypes thread)
{
	SetValue(GetDefaultValue());
}

Command::Command(VariableIds variable, const std::string& description, bool bSearchable) :
	Variable(variable),
	Description(description),
	Searchable(bSearchable),
	Callback(nullptr),
	ArgumentCallback(nullptr)
{

}

Command::~Command() {}

VariableIds Command::GetId() const
{
	return Variable;
}

std::string Command::GetName() const
{
	return Manager.GetVariableName(GetId());
}

std::string Command::GetDescription() const
{
	return Description;
}

bool Command::IsSearchable() const
{
	return Searchable;
}

bool Command::HasCallback() const
{
	return (!!Callback);
}

bool Command::HasArgumentCallback() const
{
	return (!!ArgumentCallback);
}

Command* Command::BindCallback(std::function<void()> callback)
{
	Callback = callback;
	return this;
}

Command* Command::BindCallback(std::function<void(std::string)> callback)
{
	ArgumentCallback = callback;
	return this;
}

Command* Command::UnbindCallbacks()
{
	Callback = nullptr;
	ArgumentCallback = nullptr;
	return this;
}

void Command::TriggerCallback() const
{
	if (HasCallback())
	{
		Callback();
	}
	else
	{
		Console.Warning("[Command] (" + GetName() + ") Warning: Unbound callback triggered!");
	}
}

void Command::TriggerCallback(const std::string& arguments) const
{
	if (HasArgumentCallback())
	{
		ArgumentCallback(arguments);
	}
	else
	{
		Console.Warning("[Command] (" + GetName() + ") Warning: Unbound argument callback triggered!");
	}
}

QueueData::QueueData() {}

QueueData::QueueData(const std::string& command, const std::string& arguments, bool bLogToConsole) : Command(command), Arguments(arguments), LogToConsole(bLogToConsole) {}

QueueData::~QueueData() {}

ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and mods.")
{
	OnCreate();
}

ManagerComponent::~ManagerComponent()
{
	OnDestroy();
}

void ManagerComponent::OnCreate()
{
	PlaceholderMod = nullptr;
}

void ManagerComponent::OnDestroy()
{
	ModuleMap.clear();
	CommandMap.clear();
	SettingMap.clear();
}

void ManagerComponent::UnrealCommand(const std::string& unrealCommand, bool bLogToConsole)
{
	AActor* randomActor = Instances.GetDefaultInstanceOf<AActor>();

	if (randomActor)
	{
		if (bLogToConsole)
		{
			Console.Write(GetNameFormatted() +  "Executing unreal command: \"" + unrealCommand + "\"");
		}

		//randomActor->ConsoleCommand(StringWrapper(unrealCommand).ToUnrealString(), false); // I have my own custom wrappers, they are too game-specific to implement directly here.
	}
}

void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments, bool bLogToConsole)
{
	std::shared_ptr<Command> consoleCommand = GetCommand(command);

	if (consoleCommand)
	{
		if (consoleCommand->HasCallback() && arguments.empty())
		{
			consoleCommand->TriggerCallback();
		}
		else if (consoleCommand->HasArgumentCallback())
		{
			if (!arguments.empty())
			{
				consoleCommand->TriggerCallback(arguments);
			}
			else if (bLogToConsole)
			{
				Console.Error(GetNameFormatted() + "Invalid arguments for command \"" + command + "\"!");
			}
		}
		else if (bLogToConsole)
		{
			Console.Error(GetNameFormatted() + "Custom arguments are not supported for the command \"" + command + "\"!");
		}
	}
	else
	{
		std::shared_ptr<Setting> consoleSetting = GetSetting(command);

		if (consoleSetting)
		{
			if (consoleSetting->IsModifiable())
			{
				if (arguments.length() > 0)
				{
					std::string oldValue = consoleSetting->GetStringValue();
					consoleSetting->SetValue(arguments);

					if (bLogToConsole)
					{
						if (oldValue != consoleSetting->GetStringValue())
						{
							Console.Notify(GetNameFormatted() + "" + consoleSetting->GetName() + ": " + oldValue + " -> " + consoleSetting->GetStringValue());
						}
					}
				}
				else
				{
					if (bLogToConsole)
					{
						Console.Notify(GetNameFormatted() + "Name: " + consoleSetting->GetName());
						Console.Notify(GetNameFormatted() + "Description: " + consoleSetting->GetDescription());
						Console.Notify(GetNameFormatted() + "Current Value: " + consoleSetting->GetStringValue());
					}
				}
			}
			else
			{
				if (bLogToConsole)
				{
					Console.Error(GetNameFormatted() + "Unrecognized setting: \"" + command + "\"");
				}
			}
		}
		else
		{
			if (bLogToConsole)
			{
				Console.Error(GetNameFormatted() + "Unrecognized command: \"" + command + "\"");
			}
		}
	}
}

void ManagerComponent::QueueCommand(const std::string& command, const std::string& arguments, bool bLogToConsole)
{
	CommandQueue.emplace_back(QueueData(command, arguments, bLogToConsole));
}

void ManagerComponent::QueueTick()
{
	if (!CommandQueue.empty())
	{
		for (const QueueData& data : CommandQueue)
		{
			ConsoleCommand(data.Command, data.Arguments, data.LogToConsole);
		}

		CommandQueue.clear();
	}
}

void ManagerComponent::ResetSetting(const std::string& settingName, bool bLogToConsole)
{
	std::shared_ptr<Setting> setting = GetSetting(settingName);

	if (setting)
	{
		setting->ResetToDefault();

		if (bLogToConsole)
		{
			Console.Notify("[Manager Component] Reset setting to its default value!");
			Console.Notify("[Manager Component] Name: " + setting->GetName());
			Console.Notify("[Manager Component] Description: " + setting->GetDescription());
			Console.Notify("[Manager Component] Current Value: " + setting->GetStringValue());
		}
	}
	else
	{
		if (bLogToConsole)
		{
			Console.Error("[Manager Component] Unrecognized setting: \"" + settingName + "\"");
		}
	}
}

void ManagerComponent::PrintModule(const std::string& moduleName)
{
	if (ModuleMap.find(moduleName) != ModuleMap.end())
	{
		std::shared_ptr<Module> mod = ModuleMap[moduleName];

		bool first = true;
		uint32_t states = mod->GetAllowedStates();
		std::string allowedStates;

		// Again, these states will all be game specific, please check ou the "GameState.hpp" file for more info.
		if (states & States::STATES_MainMenu) { allowedStates += (first ? "(" : " | ") + std::string("MainMenu"); first = false; }
		if (states & States::STATES_Trading) { allowedStates += (first ? "(" : " | ") + std::string("Trading"); first = false; }
		if (states & States::STATES_CasualMatch) { allowedStates += (first ? "(" : " | ") + std::string("CasualMatch"); first = false; }
		if (states & States::STATES_RankedMatch) { allowedStates += (first ? "(" : " | ") + std::string("RankedMatch"); first = false; }
		if (states & States::STATES_All) { allowedStates += (first ? "(" : " | ") + std::string("All"); first = false; }

		if (!first)
		{
			allowedStates += ")";
		}

		Console.Notify("[Manager Component] Module Name: " + mod->GetName());
		Console.Notify("[Manager Component] Module Description: " + mod->GetDescription());
		Console.Notify("[Manager Component] Allowed States/Permissions: " + allowedStates);
		Console.Notify("[Manager Component] Allowed In Current State: " + std::string((mod->IsAllowed() ? "true" : "false")));
	}
	else
	{
		Console.Error("[Manager Component] Failed to find module \"" + moduleName + "\"!");
	}
}

std::string ManagerComponent::GetVariableName(VariableIds variable)
{
	if (VariableMap_IDS.find(variable) != VariableMap_IDS.end())
	{
		return VariableMap_IDS[variable];
	}

	return "UnknownVariable";
}

void ManagerComponent::CreateVariable(const std::string& name, VariableIds variable)
{
	if (VariableMap_SID.find(name) == VariableMap_SID.end())
	{
		VariableMap_SID.emplace(std::make_pair(name, variable));
		VariableMap_IDS.emplace(std::make_pair(variable, name));
	}
	else
	{
		Console.Warning("[Manager Component] Warning: Duplicate variable name detected for id: " + std::to_string(static_cast<int32_t>(variable)) + "!");
	}
}

template <typename T> std::shared_ptr<T> ManagerComponent::CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind)
{
	if (mod)
	{
		std::string moduleName = mod->GetName();
		moduleToBind = nullptr;

		if (ModuleMap.find(moduleName) == ModuleMap.end())
		{
			ModuleMap.emplace(std::make_pair(moduleName, std::shared_ptr<Module>(mod)));
			moduleToBind = std::static_pointer_cast<T>(ModuleMap[moduleName]);
			return moduleToBind;
		}
		else
		{
			Console.Warning("[Manager Component] Warning: Duplicate module name detected for \"" + moduleName + "\"!");
		}
	}
	else
	{
		Console.Error("[Manager Component] Error: Failed to create module, given pointer is invalid!");
	}

	return nullptr;
}

template <typename T> std::shared_ptr<T> ManagerComponent::GetModule(const std::string& moduleName)
{
	if (ModuleMap.find(moduleName) != ModuleMap.end())
	{
		return std::static_pointer_cast<T>(ModuleMap[moduleName]);
	}

	return nullptr;
}

std::shared_ptr<Command> ManagerComponent::CreateCommand(Command* command)
{
	if (command)
	{
		std::string commandName = command->GetName();

		if (CommandMap.find(commandName) == CommandMap.end())
		{
			CommandMap.emplace(std::make_pair(commandName, std::shared_ptr<Command>(command)));
			return CommandMap[commandName];
		}
		else
		{
			Console.Warning("[Manager Component] Warning: Duplicate command name detected for \"" + commandName + "\"!");
		}
	}
	else
	{
		Console.Error("[Manager Component] Error: Failed to create command, given pointer is invalid!");
	}

	return nullptr;
}

std::shared_ptr<Command> ManagerComponent::GetCommand(const std::string& commandName)
{
	if (CommandMap.find(commandName) != CommandMap.end())
	{
		return CommandMap[commandName];
	}

	return nullptr;
}

std::shared_ptr<Command> ManagerComponent::GetCommand(VariableIds variable)
{
	return GetCommand(GetVariableName(variable));
}

std::shared_ptr<Setting> ManagerComponent::CreateSetting(Setting* setting)
{
	if (setting)
	{
		std::string settingName = setting->GetName();

		if (SettingMap.find(settingName) == SettingMap.end())
		{
			SettingMap.emplace(std::make_pair(settingName, std::shared_ptr<Setting>(setting)));
			return SettingMap[settingName];
		}
		else
		{
			Console.Warning("[Manager Component] Warning: Duplicate setting name detected for \"" + settingName + "\"!");
		}
	}
	else
	{
		Console.Error("[Manager Component] Error: Failed to create setting, given pointer is invalid!");
	}

	return nullptr;
}

std::shared_ptr<Setting> ManagerComponent::GetSetting(const std::string& settingName)
{
	if (SettingMap.find(settingName) != SettingMap.end())
	{
		return SettingMap[settingName];
	}

	return nullptr;
}

std::shared_ptr<Setting> ManagerComponent::GetSetting(VariableIds variable)
{
	return GetSetting(GetVariableName(variable));
}

void ManagerComponent::Initialize()
{
	// Here where we are mapping commands/settings to their internal "VariableId", this is done on runtime.
	CreateVariable("reset_setting", VariableIds::MANAGER_RESET_SETTING);
	CreateVariable("print_module", VariableIds::MANAGER_PRINT_MODULE);
	CreateVariable("unreal_command", VariableIds::MANAGER_UNREAL_COMMAND);

	CreateVariable("placeholder_do_thing", VariableIds::PLACEHOLDER_DO_THING);
	CreateVariable("placeholder_can_do_thing", VariableIds::PLACEHOLDER_ENABLED);
	CreateVariable("placeholder_some_value", VariableIds::PLACEHOLDER_SOME_VALUE);

	// Assigning the CasualMatch and RankedMatch flags, so this module will only be able to be used in casual or ranked games.
	CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);
	
	CreateCommand(new Command(VariableIds::MANAGER_RESET_SETTING, "Reset a setting to its default/original value."))->BindCallback([&](const std::string& arguments) { ResetSetting(arguments); });
	CreateCommand(new Command(VariableIds::MANAGER_PRINT_MODULE, "Print information about a given module."))->BindCallback([&](const std::string& arguments) { PrintModule(arguments); });
	CreateCommand(new Command(VariableIds::MANAGER_UNREAL_COMMAND, "Execute a Unreal Engine 3 command with the given arguments."))->BindCallback([&](const std::string& arguments) { UnrealCommand(arguments); });

	// When someone uses the command "placeholder_do_thing", this will trigger the function "DoAThing" in "PlaceholderModule".
	CreateCommand(new Command(VariableIds::PLACEHOLDER_DO_THING, "Calls the \"DoAThing\" function in \"PlaceholderMod\"."))->BindCallback([&](){ PlaceholderMod->DoAThing(); });
	// When changes the setting "placeholder_can_do_thing true", we automatically callback to "PlaceholderModule" and tell it to update its settings stored in that class.
	CreateSetting(new Setting(VariableIds::PLACEHOLDER_ENABLED, "Enable/disable the placeholder module.", "false", SettingTypes::TYPE_BOOL, true))->BindCallback([&](){ PlaceholderMod->UpdateSettings(); });
	// Integer setting that has a minimum value of "0" and a maximum  value of "100".
	CreateSetting(new Setting(VariableIds::PLACEHOLDER_SOME_VALUE, "Some random integer value with a custom range.", "0", SettingTypes::TYPE_INT, true))->SetRange("0", "100")->BindCallback([&](){ PlaceholderMod->UpdateSettings(); });
	PlaceholderMod ? PlaceholderMod->UpdateSettings() : Console.Error(GetNameFormatted() + "Error: Failed to initialize \"Paceholder\"!");

	Console.Write(GetNameFormatted() + std::to_string(CommandMap.size()) + " Command(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(SettingMap.size()) + " Setting(s) Initialized!");
}

class ManagerComponent Manager{};