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

struct CRColor Setting::GetColorValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Colors::DecimalToColor(std::stoi(CurrentValue));
	}

	return CRColor();
}

struct CRLinearColor Setting::GetLinearValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Colors::DecimalToLinearColor(std::stoi(CurrentValue));
	}

	return CRLinearColor();
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
	// Assigning the shared pointer with the CasualMatch and RankedMatch flags, so this mod will only be able to be used in casual or ranked games.
	PlaceholderMod = std::make_shared<PlaceholderModule>(PlaceholderModule("Paceholder", "An example module.", States::CasualMatch | States::RankedMatch));
	
	// When someone uses the console command "placeholder_some_bool true", we automatically callback to PlaceholderMod and tell it to update its "SomeBoolSetting" property.
	CreateCommand(Command("placeholder_do_thing", "Calls the \"DoAThing\" function in \"PlaceholderMod\"."))->BindCallback(std::bind(&PlaceholderModule::DoAThing, PlaceholderMod));
	CreateSetting(Setting("placeholder_enabled", "Enable/disable the placeholder module.", "false", SettingTypes::TYPE_BOOL, true, std::bind(&PlaceholderModule::SetPlaceholderEnabled, PlaceholderMod, std::placeholders::_1)));
	PlaceholderMod->LoadSettings();

	Console.Write(GetNameFormatted() + std::to_string(CommandMap.size() - 1) + " Command(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(SettingMap.size() - 1) + " Setting(s) Initialized!");
}

class ManagerComponent Manager;