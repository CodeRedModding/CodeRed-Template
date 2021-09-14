#include "Manager.hpp"
#include "../Components/Includes.hpp"
#include "../Extensions/Includes.hpp"

Setting::Setting(const std::string& name, const std::string& description, const std::string& defaultValue, const SettingTypes valueType, const bool bModifiable)
{
	Name = name;
	Description = description;
	Type = valueType;
	DefaultValue = defaultValue;
	CurrentValue = defaultValue;
	Modifiable = bModifiable;
	ShouldCallback = false;
	Callback = nullptr;

	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") == 0 || CurrentValue.find("0x") == 0)
		{
			CurrentValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::Setting(const std::string& name, const std::string& description, const std::string& defaultValue, const SettingTypes valueType, const bool bModifiable, std::function<void(Setting*)> callback)
{
	Name = name;
	Description = description;
	Type = valueType;
	DefaultValue = defaultValue;
	CurrentValue = defaultValue;
	Modifiable = bModifiable;
	ShouldCallback = true;
	Callback = callback;

	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") == 0 || CurrentValue.find("0x") == 0)
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::~Setting() { };

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

	return 0.f;
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
		return Colors::DecimalToLinearColor(std::stoi(CurrentValue));
	}

	return LinearColor();
}

Vector Setting::GetVector3DValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D
		|| GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		std::vector<std::string> values = Manager.SplitArguments(GetStringValue());

		Vector returnVector;

		if (values.size() == 3)
		{
			returnVector.X = std::stof(values[0]);
			returnVector.Y = std::stof(values[1]);
			returnVector.Z = std::stof(values[2]);
		}

		return returnVector;
	}

	return Vector();
}

Vector2D Setting::GetVector2DValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D
		|| GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		std::vector<std::string> values = Manager.SplitArguments(GetStringValue());

		Vector2D returnVector;

		if (values.size() == 2)
		{
			returnVector.X = std::stof(values[0]);
			returnVector.Y = std::stof(values[1]);
		}

		return returnVector;
	}

	return Vector2D();
}

void Setting::SetValue(const std::string& value)
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (value.find("#") == 0 || value.find("0x") == 0)
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
		TriggerCallback();
	}
}

void Setting::ResetToDefault()
{
	CurrentValue = DefaultValue;
}

bool Setting::IsModifiable() const
{
	return Modifiable;
}

bool Setting::HasCallback() const
{
	return ShouldCallback;
}

void Setting::TriggerCallback()
{
	if (HasCallback() && Callback)
	{
		Callback(this);
	}
}

void Setting::BindCallback(const std::function<void(Setting*)>& callback)
{
	ShouldCallback = true;
	Callback = callback;
}

void Setting::UnbindCallback()
{
	ShouldCallback = false;
	Callback = nullptr;
}

Command::Command(const std::string& name, const std::string& description)
{
	Name = name;
	Description = description;
	Type = CommandTypes::TYPE_NONE;
	Callback = nullptr;
	ArgumentCallback = nullptr;
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

void Command::SetArguments(const std::string& arguments)
{
	Arguments = arguments;
}

void Command::BindCallback(std::function<void()> callback)
{
	Callback = callback;
	Type = CommandTypes::TYPE_CALLBACK;
}

void Command::BindArguments(std::function<void(std::string)> callback)
{
	ArgumentCallback = callback;
	Type = CommandTypes::TYPE_ARGUMENT;
}

void Command::TriggerCallback()
{
	if (GetType() == CommandTypes::TYPE_NONE)
	{
		Console.Warning("Warning: Unbound callback triggered for command \"" + GetName() + "\".");
	}
	else if (GetType() == CommandTypes::TYPE_CALLBACK)
	{
		if (Callback)
		{
			Callback();
		}
	}
	else if (GetType() == CommandTypes::TYPE_ARGUMENT)
	{
		if (ArgumentCallback)
		{
			ArgumentCallback(Arguments);
		}
	}
}

void Command::TriggerCallback(const std::string& arguments)
{
	if (GetType() == CommandTypes::TYPE_NONE)
	{
		Console.Warning("Warning: Unbound callback triggered for command \"" + GetName() + "\".");
	}
	else if (Type == CommandTypes::TYPE_ARGUMENT)
	{
		if (ArgumentCallback)
		{
			Arguments = arguments;
			ArgumentCallback(arguments);
		}
	}
}

ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and mods.") { }

ManagerComponent::~ManagerComponent() { }

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
		Console.Write(GetNameFormatted() +  "Executing unreal command: \"" + unrealCommand + "\"");
		//randomActor->ConsoleCommand(StringWrapper(unrealCommand).ToUnrealString(), false); // I have my own custom wrappers, they are too game-specific to implement directly here.
	}
}

void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments)
{
	std::shared_ptr<Command> consoleCommand = GetCommand(command);

	if (consoleCommand)
	{
		CommandTypes commandType = consoleCommand->GetType();

		if (commandType == CommandTypes::TYPE_CALLBACK)
		{
			consoleCommand->TriggerCallback();
		}
		else if (commandType == CommandTypes::TYPE_ARGUMENT)
		{
			if (arguments.size() > 0)
			{
				consoleCommand->TriggerCallback(arguments);
			}
			else
			{
				Console.Error(GetNameFormatted() + "Invalid arguments for command: \"" + command + "\"");
			}
		}
	}
	else
	{
		std::shared_ptr<Setting> Setting = GetSetting(command);

		if (Setting)
		{
			if (Setting->IsModifiable())
			{
				if (arguments.size() > 0)
				{
					Setting->SetValue(arguments);
				}
				else
				{
					Console.Notify(GetNameFormatted() + "\"" + Setting->GetName() + "\"'s current value: " + Setting->GetStringValue());
				}
			}
			else
			{
				Console.Error(GetNameFormatted() + "Unrecognized setting: \"" + command + "\"");
			}
		}
		else
		{
			Console.Error(GetNameFormatted() + "Unrecognized command: \"" + command + "\"");
		}
	}
}

void ManagerComponent::AddToQueue(const std::string& command, const std::string& arguments)
{
	Queue.emplace_back(command, arguments);
}

void ManagerComponent::QueueTick()
{
	if (Queue.size() > 0)
	{
		for (const auto& command : Queue)
		{
			ConsoleCommand(command.first, command.second);
		}

		Queue.clear();
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
			Console.Success(GetNameFormatted() + "Reset setting to its default value!");
			Console.Notify(GetNameFormatted() + "Name: " + setting->GetName());
			Console.Notify(GetNameFormatted() + "Description: " + setting->GetDescription());
			Console.Notify(GetNameFormatted() + "Current Value: " + setting->GetStringValue());
		}
	}
	else
	{
		if (bLogToConsole)
		{
			Console.Error(GetNameFormatted() + "Unrecognized setting: \"" + settingName + "\"");
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

		Console.Notify(GetNameFormatted() + "Module Name: " + mod->GetName());
		Console.Notify(GetNameFormatted() + "Module Description: " + mod->GetDescription());
		Console.Notify(GetNameFormatted() + "Allowed States/Permissions: " + allowedStates);
		Console.Notify(GetNameFormatted() + "Allowed In Current State: " + (mod->IsAllowed() ? "true" : "false"));
	}
	else
	{
		Console.Error(GetNameFormatted() + "Failed to find module \"" + moduleName + "\"!");
	}
}

template <typename T> std::shared_ptr<T> ManagerComponent::CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind)
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
		Console.Warning(GetNameFormatted() + "Warning: Duplicate module name detected for \"" + moduleName + "\"!");
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
	std::string commandName = command->GetName();

	if (CommandMap.find(commandName) == CommandMap.end())
	{
		CommandMap.emplace(std::make_pair(commandName, std::shared_ptr<Command>(command)));
		return CommandMap[commandName];
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Duplicate command name detected for \"" + commandName + "\"!");
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

std::shared_ptr<Setting> ManagerComponent::CreateSetting(Setting* setting)
{
	std::string settingName = setting->GetName();

	if (SettingMap.find(settingName) == SettingMap.end())
	{
		SettingMap.emplace(std::make_pair(settingName, std::shared_ptr<Setting>(setting)));
		return SettingMap[settingName];
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Duplicate setting name detected for \"" + settingName + "\"!");
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
	// Assigning the CasualMatch and RankedMatch flags, so this module will only be able to be used in casual or ranked games.
	CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);
	
	// When someone uses the console command "placeholder_some_bool true", we automatically callback to PlaceholderMod and tell it to update its "SomeBoolSetting" property.
	CreateCommand(new Command("placeholder_do_thing", "Calls the \"DoAThing\" function in \"PlaceholderMod\"."))->BindCallback([&](){ PlaceholderMod->DoAThing(); });
	CreateSetting(new Setting("placeholder_enabled", "Enable/disable the placeholder module.", "false", SettingTypes::TYPE_BOOL, true, std::bind(&PlaceholderModule::SetPlaceholderEnabled, PlaceholderMod, std::placeholders::_1)));
	PlaceholderMod->LoadSettings();

	Console.Write(GetNameFormatted() + std::to_string(CommandMap.size() - 1) + " Command(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(SettingMap.size() - 1) + " Setting(s) Initialized!");
}

class ManagerComponent Manager;