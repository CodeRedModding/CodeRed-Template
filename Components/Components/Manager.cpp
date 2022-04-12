#include "Manager.hpp"
#include "../Includes.hpp"

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable) :
	Variable(variable),
	Type(settingType),
	Description(description),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(nullptr),
	ArgumentCallback(nullptr)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetStringRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") == 0)
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, std::function<void()> callback) :
	Variable(variable),
	Type(settingType),
	Description(description),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(callback),
	ArgumentCallback(nullptr)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetStringRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") == 0 || Format::IsStringHexadecimal(CurrentValue))
		{
			DefaultValue = std::to_string(Colors::HexToDecimal(CurrentValue));
			CurrentValue = DefaultValue;
		}
	}
}

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, std::function<void(std::string)> callback) :
	Variable(variable),
	Type(settingType),
	Description(description),
	DefaultValue(defaultValue),
	CurrentValue(defaultValue),
	Modifiable(bModifiable),
	Callback(nullptr),
	ArgumentCallback(callback)
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		SetStringRange("false", "true");
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		if (CurrentValue.find("#") == 0 || Format::IsStringHexadecimal(CurrentValue))
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
	return ManagerComponent::GetVariableName(GetId());
}

std::string Setting::GetDescription() const
{
	return Description;
}

bool Setting::HasRange() const
{
	return (!Range.first.empty() && !Range.second.empty());
}

bool Setting::InRange(const std::string& sValue) const
{
	if (HasRange())
	{
		if (GetType() == SettingTypes::TYPE_INT)
		{
			int32_t formattedMin = std::stoi(Range.first);
			int32_t formattedMax = std::stoi(Range.second);
			int32_t formattedValue = std::stoi(sValue);
			return (formattedValue >= formattedMin && formattedValue <= formattedMax);
		}
		else if (GetType() == SettingTypes::TYPE_BOOL)
		{
			return (sValue == "0" || sValue == "false" || sValue == "1" || sValue == "true");
		}
		else if (GetType() == SettingTypes::TYPE_FLOAT)
		{
			float formattedMin = std::stof(Range.first);
			float formattedMax = std::stof(Range.second);
			float formattedValue = std::stof(sValue);
			return ((formattedValue >= formattedMin) && (formattedValue <= formattedMax));
		}
		else if (GetType() == SettingTypes::TYPE_ROTATOR)
		{
			Rotator value = Format::ToRotator(sValue);
			Rotator rangeMin = Format::ToRotator(Range.first);
			Rotator rangeMax = Format::ToRotator(Range.second);

			if ((value.Pitch < rangeMin.Pitch) || (value.Pitch > rangeMax.Pitch)
				|| (value.Yaw < rangeMin.Yaw) || (value.Yaw > rangeMax.Yaw)
				|| (value.Roll < rangeMin.Roll) || (value.Yaw > rangeMax.Roll))
			{
				return false;
			}
		}
		else if (GetType() == SettingTypes::TYPE_VECTOR_3D)
		{
			VectorF value = Format::ToVectorF(sValue);
			VectorF rangeMin = Format::ToVectorF(Range.first);
			VectorF rangeMax = Format::ToVectorF(Range.second);

			if ((value.X < rangeMin.X) || (value.X > rangeMax.X)
				|| (value.Y < rangeMin.Y) || (value.Y > rangeMax.Y)
				|| (value.Z < rangeMin.Z) || (value.Z > rangeMax.Z))
			{
				return false;
			}
		}
		else if (GetType() == SettingTypes::TYPE_VECTOR_2D)
		{
			Vector2DF value = Format::ToVector2DF(sValue);
			Vector2DF rangeMin = Format::ToVector2DF(Range.first);
			Vector2DF rangeMax = Format::ToVector2DF(Range.second);

			if ((value.X < rangeMin.X) || (value.X > rangeMax.X)
				|| (value.Y < rangeMin.Y) || (value.Y > rangeMax.Y))
			{
				return false;
			}
		}
	}

	return true;
}

bool Setting::IsValueValid(const std::string& sValue) const
{
	if (GetType() == SettingTypes::TYPE_INT)
	{
		if (Format::IsStringDecimal(sValue))
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
		if (sValue == "1" || sValue == "true" || sValue == "0" || sValue == "false")
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
		if (Format::IsStringFloat(sValue))
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
		if (sValue.find("#") == 0 || Format::IsStringHexadecimal(sValue))
		{
			return true;
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports RGB hexadecimal values.");
			return false;
		}
	}

	return true;
}

bool Setting::IsModifiable() const
{
	return Modifiable;
}

bool Setting::HasCallback() const
{
	return (!!Callback);
}

bool Setting::HasArgumentCallback() const
{
	return (!!ArgumentCallback);
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

Rotator Setting::GetRotatorValue() const
{
	if (GetType() == SettingTypes::TYPE_ROTATOR)
	{
		return Format::ToRotator(GetStringValue());
	}

	return Rotator();
}

VectorF Setting::GetVector3DFValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D || GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		return Format::ToVectorF(GetStringValue());
	}

	return VectorF();
}

VectorI Setting::GetVector3DIValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D || GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		return Format::ToVectorI(GetStringValue());
	}

	return VectorI();
}

Vector2DF Setting::GetVector2DFValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D || GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		return Format::ToVector2DF(GetStringValue());
	}

	return Vector2DF();
}

Vector2DI Setting::GetVector2DIValue() const
{
	if (GetType() == SettingTypes::TYPE_VECTOR_3D || GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		return Format::ToVector2DI(GetStringValue());
	}

	return Vector2DI();
}

Setting* Setting::ResetToDefault(ThreadTypes thread)
{
	SetStringValue(GetDefaultValue(), thread);
	return this;
}

Setting* Setting::SetStringValue(const std::string& sValue, ThreadTypes thread)
{
	if (thread != ThreadTypes::THREAD_GAME)
	{
		ManagerComponent::QueueCommand(GetName(), sValue, (thread == ThreadTypes::THREAD_RENDER ? false : true));
	}
	else if (IsValueValid(sValue))
	{
		if (InRange(sValue))
		{
			if (GetType() == SettingTypes::TYPE_BOOL)
			{
				if (sValue == "1" || sValue == "true")
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
				CurrentValue = std::to_string(Colors::HexToDecimal(sValue));
			}
			else
			{
				CurrentValue = sValue;
			}

			TriggerCallbacks();
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is out of range, this setting has a minimum value of \"" + Range.first + "\" and a maximum value of \"" + Range.second + "\".");
		}
	}

	return this;
}

Setting* Setting::SetIntValue(int32_t iValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(iValue), thread);
	return this;
}

Setting* Setting::SetBoolValue(bool bValue, ThreadTypes thread)
{
	SetStringValue((bValue ? "true" : "false"), thread);
	return this;
}

Setting* Setting::SetFloatValue(float fValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(fValue), thread);
	return this;
}

Setting* Setting::SetColorValue(const Color& cValue, ThreadTypes thread)
{
	SetStringValue(Colors::ColorToHex(cValue), thread);
	return this;
}

Setting* Setting::SetLinearValue(const LinearColor& lValue, ThreadTypes thread)
{
	SetStringValue(Colors::LinearToHex(lValue), thread);
	return this;
}

Setting* Setting::SetRotatorValue(const Rotator& rValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(rValue.Pitch) + " " + std::to_string(rValue.Yaw) + " " + std::to_string(rValue.Roll), thread);
	return this;
}

Setting* Setting::SetVectorFValue(const VectorF& vfValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(vfValue.X) + " " + std::to_string(vfValue.Y) + " " + std::to_string(vfValue.Z), thread);
	return this;
}

Setting* Setting::SetVectorIValue(const VectorI& viValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(viValue.X) + " " + std::to_string(viValue.Y) + " " + std::to_string(viValue.Z), thread);
	return this;
}

Setting* Setting::SetVector2DFValue(const Vector2DF& vfValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(vfValue.X) + " " + std::to_string(vfValue.Y), thread);
	return this;
}

Setting* Setting::SetVector2DIValue(const Vector2DI& viValue, ThreadTypes thread)
{
	SetStringValue(std::to_string(viValue.X) + " " + std::to_string(viValue.Y), thread);
	return this;
}

Setting* Setting::RemoveRange()
{
	Range.first.clear();
	Range.second.clear();
	return this;
}

Setting* Setting::SetStringRange(const std::string& minValue, const std::string& maxValue)
{
	Range = std::make_pair(minValue, maxValue);
	return this;
}

Setting* Setting::SetIntRange(int32_t minValue, int32_t maxValue)
{
	SetStringRange(std::to_string(minValue), std::to_string(maxValue));
	return this;
}

Setting* Setting::SetFloatRange(float minValue, float maxValue)
{
	SetStringRange(std::to_string(minValue), std::to_string(maxValue));
	return this;
}

Setting* Setting::SetRotatorRange(const Rotator& minValue, const Rotator& maxValue)
{
	SetStringRange(std::to_string(minValue.Pitch) + " " + std::to_string(minValue.Yaw) + " " + std::to_string(minValue.Roll), std::to_string(maxValue.Pitch) + " " + std::to_string(maxValue.Yaw) + " " + std::to_string(maxValue.Roll));
	return this;
}

Setting* Setting::SetVectorFRange(const VectorF& minValue, const VectorF& maxValue)
{
	SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y) + " " + std::to_string(minValue.Z), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y) + " " + std::to_string(maxValue.Z));
	return this;
}

Setting* Setting::SetVectorIRange(const VectorI& minValue, const VectorI& maxValue)
{
	SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y) + " " + std::to_string(minValue.Z), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y) + " " + std::to_string(maxValue.Z));
	return this;
}

Setting* Setting::SetVector2DFRange(const Vector2DF& minValue, const Vector2DF& maxValue)
{
	SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y));
	return this;
}

Setting* Setting::SetVector2DIRange(const Vector2DI& minValue, const Vector2DI& maxValue)
{
	SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y));
	return this;
}

std::pair<int32_t, int32_t> Setting::GetIntRange() const
{
	std::pair<float, float> floatRange = GetFloatRange();
	return std::make_pair(static_cast<int32_t>(floatRange.first), static_cast<int32_t>(floatRange.second));
}

std::pair<float, float> Setting::GetFloatRange() const
{
	std::pair<float, float> returnRange = std::make_pair(0.0f, 0.0f);

	if (HasRange())
	{
		if (Format::IsStringFloat(Range.first) && Format::IsStringFloat(Range.second))
		{
			returnRange.first = std::stof(Range.first);
			returnRange.second = std::stof(Range.second);
		}
	}

	return returnRange;
}

std::pair<Rotator, Rotator> Setting::GetRotatorRange() const
{
	std::pair<Rotator, Rotator> returnRange = std::make_pair(Rotator(), Rotator());

	if (HasRange())
	{
		returnRange.first = Format::ToRotator(Range.first);
		returnRange.second = Format::ToRotator(Range.second);
	}

	return returnRange;
}

std::pair<VectorF, VectorF> Setting::GetVectorFRange() const
{
	std::pair<VectorF, VectorF> returnRange = std::make_pair(VectorF(), VectorF());

	if (HasRange())
	{
		returnRange.first = Format::ToVectorF(Range.first);
		returnRange.second = Format::ToVectorF(Range.second);
	}

	return returnRange;
}

std::pair<VectorI, VectorI> Setting::GetVectorIRange() const
{
	std::pair<VectorI, VectorI> returnRange = std::make_pair(VectorI(), VectorI());

	if (HasRange())
	{
		returnRange.first = Format::ToVectorI(Range.first);
		returnRange.second = Format::ToVectorI(Range.second);
	}

	return returnRange;
}

std::pair<Vector2DF, Vector2DF> Setting::GetVector2DFRange() const
{
	std::pair<Vector2DF, Vector2DF> returnRange = std::make_pair(Vector2DF(), Vector2DF());

	if (HasRange())
	{
		returnRange.first = Format::ToVector2DF(Range.first);
		returnRange.second = Format::ToVector2DF(Range.second);
	}

	return returnRange;
}

std::pair<Vector2DI, Vector2DI> Setting::GetVector2DIRange() const
{
	std::pair<Vector2DI, Vector2DI> returnRange = std::make_pair(Vector2DI(), Vector2DI());

	if (HasRange())
	{
		returnRange.first = Format::ToVector2DI(Range.first);
		returnRange.second = Format::ToVector2DI(Range.second);
	}

	return returnRange;
}

Setting* Setting::UnbindCallbacks()
{
	Callback = nullptr;
	ArgumentCallback = nullptr;
	return this;
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

void Setting::TriggerCallbacks() const
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

ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and mods.") { OnCreate(); }

ManagerComponent::~ManagerComponent() { OnDestroy(); }

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
					consoleSetting->SetStringValue(arguments);

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

	// Assigning the "STATES_CasualMatch" and "STATES_RankedMatch" flags, so this module will only be able to be used in casual or ranked games.
	CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);
	
	CreateCommand(new Command(VariableIds::MANAGER_RESET_SETTING, "Reset a setting to its default/original value."))->BindCallback([&](const std::string& arguments) { ResetSetting(arguments); });
	CreateCommand(new Command(VariableIds::MANAGER_PRINT_MODULE, "Print information about a given module."))->BindCallback([&](const std::string& arguments) { PrintModule(arguments); });
	CreateCommand(new Command(VariableIds::MANAGER_UNREAL_COMMAND, "Execute a Unreal Engine 3 command with the given arguments."))->BindCallback([&](const std::string& arguments) { UnrealCommand(arguments); });

	if (PlaceholderMod)
	{
		// When someone uses the command "placeholder_do_thing", this will trigger the function "DoAThing" in "PlaceholderModule".
		CreateCommand(new Command(VariableIds::PLACEHOLDER_DO_THING, "Calls the \"DoAThing\" function in \"PlaceholderMod\"."))->BindCallback([&](){ PlaceholderMod->DoAThing(); });
		
		// When changes the setting "placeholder_can_do_thing true", we automatically callback to "PlaceholderModule" and tell it to update its settings stored in that class.
		CreateSetting(new Setting(VariableIds::PLACEHOLDER_ENABLED, SettingTypes::TYPE_BOOL, "Enable/disable the placeholder module.", "false", true))->BindCallback([&](){ PlaceholderMod->UpdateSettings(); });
		
		// Integer setting that has a minimum value of "0" and a maximum value of "100".
		CreateSetting(new Setting(VariableIds::PLACEHOLDER_SOME_VALUE, SettingTypes::TYPE_INT, "Some random integer value with a custom range.", "0", true))->SetIntRange(0, 100)->BindCallback([&](){ PlaceholderMod->UpdateSettings(); });

		PlaceholderMod->UpdateSettings();
	}
	else
	{
		Console.Error(GetNameFormatted() + "Error: Failed to initialize \"Paceholder\"!");
	}

	Console.Write(GetNameFormatted() + std::to_string(CommandMap.size()) + " Command(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(SettingMap.size()) + " Setting(s) Initialized!");
}

class ManagerComponent Manager{};