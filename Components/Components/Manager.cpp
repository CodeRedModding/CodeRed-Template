#include "Manager.hpp"
#include "../Includes.hpp"

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable) :
	m_variable(variable),
	m_type(settingType),
	m_description(description),
	m_defaultValue(defaultValue),
	m_currentValue(defaultValue),
	m_modifiable(bModifiable),
	m_callback(nullptr),
	m_argumentCallback(nullptr)
{

}

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, std::function<void()> callback) :
	m_variable(variable),
	m_type(settingType),
	m_description(description),
	m_defaultValue(defaultValue),
	m_currentValue(defaultValue),
	m_modifiable(bModifiable),
	m_callback(callback),
	m_argumentCallback(nullptr)
{

}

Setting::Setting(VariableIds variable, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bModifiable, std::function<void(std::string)> callback) :
	m_variable(variable),
	m_type(settingType),
	m_description(description),
	m_defaultValue(defaultValue),
	m_currentValue(defaultValue),
	m_modifiable(bModifiable),
	m_callback(nullptr),
	m_argumentCallback(callback)
{

}

Setting::Setting(const Setting& setting) :
	m_variable(setting.m_variable),
	m_type(setting.m_type),
	m_description(setting.m_description),
	m_defaultValue(setting.m_defaultValue),
	m_currentValue(setting.m_currentValue),
	m_range(setting.m_range),
	m_modifiable(setting.m_modifiable),
	m_callback(setting.m_callback),
	m_argumentCallback(setting.m_argumentCallback)
{

}

Setting::~Setting() {}

VariableIds Setting::GetId() const
{
	return m_variable;
}

SettingTypes Setting::GetType() const
{
	return m_type;
}

std::string Setting::GetName() const
{
	return ManagerComponent::GetVariableName(GetId());
}

const std::string& Setting::GetDescription() const
{
	return m_description;
}

bool Setting::HasRange() const
{
	return (!m_range.first.empty() && !m_range.second.empty());
}

bool Setting::InRange(const std::string& sValue) const
{
	if (HasRange())
	{
		if (GetType() == SettingTypes::TYPE_BOOL)
		{
			return (sValue == "0" || sValue == "false" || sValue == "1" || sValue == "true");
		}
		else if (GetType() == SettingTypes::TYPE_INT)
		{
			int32_t formattedMin = std::stoi(m_range.first);
			int32_t formattedMax = std::stoi(m_range.second);
			int32_t formattedValue = std::stoi(sValue);
			return (formattedValue >= formattedMin && formattedValue <= formattedMax);
		}
		else if (GetType() == SettingTypes::TYPE_FLOAT)
		{
			float formattedMin = std::stof(m_range.first);
			float formattedMax = std::stof(m_range.second);
			float formattedValue = std::stof(sValue);
			return ((formattedValue >= formattedMin) && (formattedValue <= formattedMax));
		}
		else if (GetType() == SettingTypes::TYPE_ROTATOR)
		{
			Rotator value = Format::ToRotator(sValue);
			Rotator rangeMin = Format::ToRotator(m_range.first);
			Rotator rangeMax = Format::ToRotator(m_range.second);

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
			VectorF rangeMin = Format::ToVectorF(m_range.first);
			VectorF rangeMax = Format::ToVectorF(m_range.second);

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
			Vector2DF rangeMin = Format::ToVector2DF(m_range.first);
			Vector2DF rangeMax = Format::ToVector2DF(m_range.second);

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
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		if (!(sValue == "1" || sValue == "true" || sValue == "0" || sValue == "false"))
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports true or false values.");
			return false;
		}
	}
	if (GetType() == SettingTypes::TYPE_INT)
	{
		if (!Format::IsStringFloat(sValue))
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports floating point numbers or 32 bit integer values.");
			return false;
		}
	}
	else if (GetType() == SettingTypes::TYPE_FLOAT)
	{
		if (!Format::IsStringFloat(sValue))
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports floating point numbers or optionally 32 bit integer values.");
			return false;
		}
	}
	else if (GetType() == SettingTypes::TYPE_COLOR)
	{
		std::string cValue = Format::RemoveAllChars(sValue, '#');

		if (Format::IsStringHexadecimal(cValue) && (cValue.length() <= 8))
		{
			uint64_t dValue = Format::ToDecimal(cValue);

			if (dValue <= 0xFFFFFFFF)
			{
				return true;
			}
		}

		Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports RGBA hexadecimal values (alpha value optional).");
		return false;
	}
	else if (GetType() == SettingTypes::TYPE_ROTATOR)
	{
		std::vector<std::string> values = Format::Split(sValue, ' ');

		if (values.size() >= 3)
		{
			if (Format::IsStringDecimal(values[0]) && Format::IsStringDecimal(values[1]) && Format::IsStringDecimal(values[2]))
			{
				return true;
			}
		}

		Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports three 32 bit integer values.");
		return false;
	}
	else if (GetType() == SettingTypes::TYPE_VECTOR_3D)
	{
		std::vector<std::string> values = Format::Split(sValue, ' ');

		if (values.size() >= 3)
		{
			if (Format::IsStringFloat(values[0]) && Format::IsStringFloat(values[1]) && Format::IsStringFloat(values[2]))
			{
				return true;
			}
		}

		Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports three floating point numbers or optionally 32 bit integer values.");
		return false;
	}
	else if (GetType() == SettingTypes::TYPE_VECTOR_2D)
	{
		std::vector<std::string> values = Format::Split(sValue, ' ');

		if (values.size() >= 2)
		{
			if (Format::IsStringFloat(values[0]) && Format::IsStringFloat(values[1]))
			{
				return true;
			}
		}

		Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports two floating point numbers or optionally 32 bit integer values.");
		return false;
	}

	return true;
}

bool Setting::IsModifiable() const
{
	return m_modifiable;
}

bool Setting::HasCallback() const
{
	return !!m_callback;
}

bool Setting::HasArgumentCallback() const
{
	return !!m_argumentCallback;
}

const std::string& Setting::GetDefaultValue() const
{
	return m_defaultValue;
}

const std::string& Setting::GetStringValue() const
{
	return m_currentValue;
}

int32_t Setting::GetIntValue() const
{
	if (GetType() == SettingTypes::TYPE_INT)
	{
		return std::stoi(GetStringValue());
	}

	return 0;
}

bool Setting::GetBoolValue() const
{
	if (GetType() == SettingTypes::TYPE_BOOL)
	{
		if ((GetStringValue() == "true") || (GetStringValue() == "1"))
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
		return std::stof(GetStringValue());
	}

	return 0.0f;
}

Color Setting::GetColorValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return Color(GetStringValue());
	}

	return Color();
}

LinearColor Setting::GetLinearValue() const
{
	if (GetType() == SettingTypes::TYPE_COLOR)
	{
		return LinearColor(GetStringValue());
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
	if ((GetType() == SettingTypes::TYPE_VECTOR_3D) || (GetType() == SettingTypes::TYPE_VECTOR_2D))
	{
		return Format::ToVectorF(GetStringValue());
	}

	return VectorF();
}

VectorI Setting::GetVector3DIValue() const
{
	if ((GetType() == SettingTypes::TYPE_VECTOR_3D) || (GetType() == SettingTypes::TYPE_VECTOR_2D))
	{
		return Format::ToVectorI(GetStringValue());
	}

	return VectorI();
}

Vector2DF Setting::GetVector2DFValue() const
{
	if ((GetType() == SettingTypes::TYPE_VECTOR_3D) || (GetType() == SettingTypes::TYPE_VECTOR_2D))
	{
		return Format::ToVector2DF(GetStringValue());
	}

	return Vector2DF();
}

Vector2DI Setting::GetVector2DIValue() const
{
	if ((GetType() == SettingTypes::TYPE_VECTOR_3D) || (GetType() == SettingTypes::TYPE_VECTOR_2D))
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
		Manager.InternalCommand(GetName(), sValue, thread);
	}
	else if (IsValueValid(sValue))
	{
		if (InRange(sValue))
		{
			if (GetType() == SettingTypes::TYPE_BOOL)
			{
				if ((sValue == "1") || (sValue == "true"))
				{
					m_currentValue = "true";
				}
				else
				{
					m_currentValue = "false";
				}
			}
			else
			{
				m_currentValue = sValue;
			}

			TriggerCallbacks();
		}
		else
		{
			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is out of range, this setting has a minimum value of \"" + m_range.first + "\" and a maximum value of \"" + m_range.second + "\".");
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
	SetStringValue(cValue.ToHexAlpha(), thread);
	return this;
}

Setting* Setting::SetLinearValue(const LinearColor& lValue, ThreadTypes thread)
{
	SetStringValue(lValue.ToHexAlpha(), thread);
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
	m_range.first.clear();
	m_range.second.clear();
	return this;
}

Setting* Setting::SetStringRange(const std::string& minValue, const std::string& maxValue)
{
	m_range = { minValue, maxValue };
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
	return { static_cast<int32_t>(floatRange.first), static_cast<int32_t>(floatRange.second) };
}

std::pair<float, float> Setting::GetFloatRange() const
{
	std::pair<float, float> returnRange{ 0.0f, 0.0f };

	if (HasRange())
	{
		if (Format::IsStringFloat(m_range.first) && Format::IsStringFloat(m_range.second))
		{
			returnRange.first = std::stof(m_range.first);
			returnRange.second = std::stof(m_range.second);
		}
	}

	return returnRange;
}

std::pair<Rotator, Rotator> Setting::GetRotatorRange() const
{
	std::pair<Rotator, Rotator> returnRange;

	if (HasRange())
	{
		returnRange.first = Format::ToRotator(m_range.first);
		returnRange.second = Format::ToRotator(m_range.second);
	}

	return returnRange;
}

std::pair<VectorF, VectorF> Setting::GetVectorFRange() const
{
	std::pair<VectorF, VectorF> returnRange;

	if (HasRange())
	{
		returnRange.first = Format::ToVectorF(m_range.first);
		returnRange.second = Format::ToVectorF(m_range.second);
	}

	return returnRange;
}

std::pair<VectorI, VectorI> Setting::GetVectorIRange() const
{
	std::pair<VectorI, VectorI> returnRange;

	if (HasRange())
	{
		returnRange.first = Format::ToVectorI(m_range.first);
		returnRange.second = Format::ToVectorI(m_range.second);
	}

	return returnRange;
}

std::pair<Vector2DF, Vector2DF> Setting::GetVector2DFRange() const
{
	std::pair<Vector2DF, Vector2DF> returnRange;

	if (HasRange())
	{
		returnRange.first = Format::ToVector2DF(m_range.first);
		returnRange.second = Format::ToVector2DF(m_range.second);
	}

	return returnRange;
}

std::pair<Vector2DI, Vector2DI> Setting::GetVector2DIRange() const
{
	std::pair<Vector2DI, Vector2DI> returnRange;

	if (HasRange())
	{
		returnRange.first = Format::ToVector2DI(m_range.first);
		returnRange.second = Format::ToVector2DI(m_range.second);
	}

	return returnRange;
}

Setting* Setting::UnbindCallbacks()
{
	m_callback = nullptr;
	m_argumentCallback = nullptr;
	return this;
}

Setting* Setting::BindCallback(std::function<void()> callback)
{
	m_callback = callback;
	return this;
}

Setting* Setting::BindCallback(std::function<void(std::string)> callback)
{
	m_argumentCallback = callback;
	return this;
}

void Setting::TriggerCallbacks() const
{
	if (HasCallback())
	{
		m_callback();
	}
	else if (HasArgumentCallback())
	{
		m_argumentCallback(GetStringValue());
	}
}

Setting& Setting::operator=(const Setting& setting)
{
	m_variable = setting.m_variable;
	m_type = setting.m_type;
	m_description = setting.m_description;
	m_defaultValue = setting.m_defaultValue;
	m_currentValue = setting.m_currentValue;
	m_range = setting.m_range;
	m_modifiable = setting.m_modifiable;
	m_callback = setting.m_callback;
	m_argumentCallback = setting.m_argumentCallback;
	return *this;
}

Command::Command(VariableIds variable, const std::string& description, bool bSearchable) :
	m_variable(variable),
	m_description(description),
	m_searchable(bSearchable),
	m_callback(nullptr),
	m_argumentCallback(nullptr)
{

}

Command::Command(const Command& command) :
	m_variable(command.m_variable),
	m_description(command.m_description),
	m_searchable(command.m_searchable),
	m_callback(command.m_callback),
	m_argumentCallback(command.m_argumentCallback)
{

}

Command::~Command() {}

VariableIds Command::GetId() const
{
	return m_variable;
}

std::string Command::GetName() const
{
	return Manager.GetVariableName(GetId());
}

const std::string& Command::GetDescription() const
{
	return m_description;
}

bool Command::IsSearchable() const
{
	return m_searchable;
}

bool Command::HasCallback() const
{
	return !!m_callback;
}

bool Command::HasArgumentCallback() const
{
	return !!m_argumentCallback;
}

Command* Command::BindCallback(std::function<void()> callback)
{
	m_callback = callback;
	return this;
}

Command* Command::BindCallback(std::function<void(std::string)> m_argumentCallback)
{
	m_argumentCallback = m_argumentCallback;
	return this;
}

Command* Command::UnbindCallbacks()
{
	m_callback = nullptr;
	m_argumentCallback = nullptr;
	return this;
}

void Command::TriggerCallback() const
{
	if (HasCallback())
	{
		m_callback();
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
		m_argumentCallback(arguments);
	}
	else
	{
		Console.Warning("[Command] (" + GetName() + ") Warning: Unbound argument callback triggered!");
	}
}

Command& Command::operator=(const Command& command)
{
	m_variable = command.m_variable;
	m_description = command.m_description;
	m_searchable = command.m_searchable;
	m_callback = command.m_callback;
	m_argumentCallback = command.m_argumentCallback;
	return *this;
}

QueueData::QueueData() : Delay(0), Delta(0), Internal(false), Expired(false) {}

QueueData::QueueData(const std::string& command, const std::string& arguments, bool bInternal) :
	Command(command),
	Arguments(arguments),
	Delay(0),
	Delta(0),
	Internal(bInternal),
	Expired(false)
{

}

QueueData::QueueData(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier) :
	Command(command),
	Arguments(arguments),
	Delay(delay* multiplier),
	Delta(0),
	Internal(bInternal),
	Expired(false)
{

}

QueueData::QueueData(const QueueData& queueData) :
	Command(queueData.Command),
	Arguments(queueData.Arguments),
	Delay(queueData.Delay),
	Delta(queueData.Delta),
	Internal(queueData.Internal),
	Expired(queueData.Expired)
{

}

QueueData::~QueueData() {}

bool QueueData::HasArguments() const
{
	return (!Arguments.empty());
}

bool QueueData::IsAsync() const
{
	return (Delay > 0);
}

bool QueueData::OnTick()
{
	if (IsAsync())
	{
		Delta++;
		return (Delta >= Delay);
	}

	return true;
}

QueueData& QueueData::operator=(const QueueData& queueData)
{
	Command = queueData.Command;
	Arguments = queueData.Arguments;
	Delay = queueData.Delay;
	Delta = queueData.Delta;
	Internal = queueData.Internal;
	Expired = queueData.Expired;
	return *this;
}

ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and mods.") { OnCreate(); }

ManagerComponent::~ManagerComponent() { OnDestroy(); }

void ManagerComponent::OnCreate()
{
	PlaceholderMod = nullptr;
}

void ManagerComponent::OnDestroy()
{
	m_variables.clear();
	m_modules.clear();
	m_commands.clear();
	m_settings.clear();
	PlaceholderMod = nullptr;
}

void ManagerComponent::UnrealCommand(const std::string& unrealCommand, bool bLogToConsole)
{
	static AActor* defaultActor = nullptr;

	if (!defaultActor)
	{
		defaultActor = Instances.GetDefaultInstanceOf<AActor>();
	}

	if (defaultActor)
	{
		if (unrealCommand.find("unreal_command") == 0)
		{
			std::string formattedCommand = unrealCommand;
			formattedCommand.erase(0, 15);

			if (bLogToConsole)
			{
				Console.Write(GetNameFormatted() + "Executing unreal command \"" + formattedCommand + "\".");
			}

			defaultActor->ConsoleCommand(formattedCommand);
		}
		else
		{
			if (bLogToConsole)
			{
				Console.Write(GetNameFormatted() + "Executing unreal command \"" + unrealCommand + "\".");
			}

			defaultActor->ConsoleCommand(unrealCommand);
		}
	}
}

std::pair<CommandTypes, std::string> ManagerComponent::InternalCommand(const QueueData& queueData, ThreadTypes thread)
{
	if ((thread != ThreadTypes::THREAD_GAME) || (queueData.IsAsync()))
	{
		m_queue.push_back(queueData);
		return { CommandTypes::TYPE_QUEUED, "" };
	}
	else
	{
		std::shared_ptr<Command> consoleCommand = GetCommand(queueData.Command);

		if (consoleCommand)
		{
			if (consoleCommand->HasCallback() && !queueData.HasArguments())
			{
				consoleCommand->TriggerCallback();
			}
			else if (consoleCommand->HasArgumentCallback())
			{
				if (queueData.HasArguments())
				{
					consoleCommand->TriggerCallback(queueData.Arguments);
				}
				else
				{
					return { CommandTypes::TYPE_EMPTY_ARGUMENTS, "" };
				}
			}
			else
			{
				return { CommandTypes::TYPE_INVALID_ARGUMENTS, "" };
			}
		}
		else
		{
			std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

			if (consoleSetting && consoleSetting->IsModifiable())
			{
				if (queueData.HasArguments())
				{
					std::string oldValue = consoleSetting->GetStringValue();
					consoleSetting->SetStringValue(queueData.Arguments);
					return { CommandTypes::TYPE_MODIFY_SETTING, oldValue };
				}
				else
				{
					return { CommandTypes::TYPE_PRINT_SETTING, "" };
				}
			}
			else
			{
				return { CommandTypes::TYPE_UNRECOGNIZED, "" };
			}
		}
	}

	return { CommandTypes::TYPE_NONE, "" };
}

std::pair<CommandTypes, std::string> ManagerComponent::InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread)
{
	return InternalCommand(QueueData(command, arguments, true), thread);
}

std::pair<CommandTypes, std::string> ManagerComponent::InternalCommand(const std::string& command, ThreadTypes thread)
{
	return InternalCommand(command, "", thread);
}

void ManagerComponent::ConsoleCommand(const QueueData& queueData, ThreadTypes thread)
{
	if (thread != ThreadTypes::THREAD_GAME)
	{
		m_queue.push_back(queueData);
	}
	else
	{
		std::pair<CommandTypes, std::string> returnType = InternalCommand(queueData, thread);

		if (returnType.first != CommandTypes::TYPE_NONE)
		{
			if (returnType.first == CommandTypes::TYPE_UNRECOGNIZED)
			{
				Console.Error(GetNameFormatted() + "Unrecognized command or setting: \"" + queueData.Command + "\".");
			}
			else if (returnType.first == CommandTypes::TYPE_EMPTY_ARGUMENTS)
			{
				Console.Error(GetNameFormatted() + "Invalid arguments provided for: \"" + queueData.Command + "\".");
			}
			else if (returnType.first == CommandTypes::TYPE_INVALID_ARGUMENTS)
			{
				Console.Error(GetNameFormatted() + "Arguments are not supported for: \"" + queueData.Command + "\".");
			}
			else if (returnType.first == CommandTypes::TYPE_MODIFY_SETTING)
			{
				std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

				if (consoleSetting)
				{
					Console.Notify(GetNameFormatted() + consoleSetting->GetName() + ": " + returnType.second + " -> " + consoleSetting->GetStringValue());
				}
			}
			else if (returnType.first == CommandTypes::TYPE_PRINT_SETTING)
			{
				std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

				if (consoleSetting)
				{
					Console.Notify(GetNameFormatted() + consoleSetting->GetName());
					Console.Notify(GetNameFormatted() + "Description: " + consoleSetting->GetDescription());
					Console.Notify(GetNameFormatted() + "Current Value: " + consoleSetting->GetStringValue());
				}
			}
		}
	}
}

void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread)
{
	ConsoleCommand(QueueData(command, arguments, false), thread);
}

void ManagerComponent::ConsoleCommand(const std::string& command, ThreadTypes thread)
{
	ConsoleCommand(command, "", thread);
}

void ManagerComponent::AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread)
{
	ConsoleCommand(QueueData(command, arguments, false, delay), thread);
}

void ManagerComponent::AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread)
{
	ConsoleCommand(QueueData(command, "", false, delay), thread);
}

void ManagerComponent::OnTick()
{
	if (!m_queue.empty())
	{
		bool safeToClear = !m_queue.empty();

		for (QueueData& data : m_queue)
		{
			if (!data.Expired)
			{
				if (data.IsAsync())
				{
					if (data.OnTick())
					{
						data.Delay = 0;
					}
					else
					{
						safeToClear = false;
						continue;
					}
				}

				if (data.Internal)
				{
					safeToClear = false;
					data.Expired = true;
					InternalCommand(data.Command, data.Arguments, ThreadTypes::THREAD_GAME);
				}
				else
				{
					safeToClear = false;
					data.Expired = true;
					ConsoleCommand(data.Command, data.Arguments, ThreadTypes::THREAD_GAME);
				}
			}
		}

		if (safeToClear)
		{
			m_queue.clear();
		}
	}

	GRainbowColor::OnTick();
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

std::string ManagerComponent::GetVariableName(VariableIds variable)
{
	if (m_variables.contains(variable))
	{
		return m_variables[variable];
	}

	return "UnknownVariable";
}

void ManagerComponent::CreateVariable(std::string name, VariableIds variable)
{
	if (!name.empty())
	{
		Format::ToLowerInline(name);

		for (const auto& variablePair : m_variables)
		{
			if (variablePair.second == name)
			{
				Console.Warning("[Manager Component] Warning: Duplicate variable name detected for \"" + name + "\"!");
				return;
			}
		}

		if (!m_variables.contains(variable))
		{
			m_variables[variable] = name;
		}
		else
		{
			Console.Warning("[Manager Component] Warning: Duplicate variable id detected for \"" + std::to_string(static_cast<int32_t>(variable)) + "\"!");
		}
	}
	else
	{
		Console.Warning("[Manager Component] Warning: Invalid variable name detected for \"" + std::to_string(static_cast<int32_t>(variable)) + "\"!");
	}
}

template <typename T> std::shared_ptr<T> ManagerComponent::CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind)
{
	if (mod)
	{
		std::string moduleName = mod->GetName();
		moduleToBind = nullptr;

		if (!m_modules.contains(moduleName))
		{
			m_modules[moduleName] = std::shared_ptr<Module>(mod);
			moduleToBind = std::static_pointer_cast<T>(m_modules[moduleName]);
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

std::shared_ptr<Command> ManagerComponent::CreateCommand(Command* command)
{
	if (command)
	{
		std::string commandName = command->GetName();

		if (!m_commands.contains(commandName))
		{
			m_commands[commandName] = std::shared_ptr<Command>(command);
			return m_commands[commandName];
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
	if (m_commands.contains(commandName))
	{
		return m_commands[commandName];
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

		if (!m_settings.contains(settingName))
		{
			m_settings[settingName] = std::shared_ptr<Setting>(setting);
			return m_settings[settingName];
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
	if (m_settings.contains(settingName))
	{
		return m_settings[settingName];
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
	CreateVariable("unreal_command", VariableIds::MANAGER_UNREAL_COMMAND);

	CreateVariable("placeholder_do_thing", VariableIds::PLACEHOLDER_DO_THING);
	CreateVariable("placeholder_can_do_thing", VariableIds::PLACEHOLDER_ENABLED);
	CreateVariable("placeholder_some_value", VariableIds::PLACEHOLDER_SOME_VALUE);

	// Assigning the "STATES_CasualMatch" and "STATES_RankedMatch" flags, so this module will only be able to be used in casual or ranked games.
	CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);
	
	CreateCommand(new Command(VariableIds::MANAGER_RESET_SETTING, "Reset a setting to its default/original value."))->BindCallback([&](const std::string& arguments) { ResetSetting(arguments); });
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

	Console.Write(GetNameFormatted() + std::to_string(m_commands.size()) + " Command(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(m_settings.size()) + " Setting(s) Initialized!");
}

class ManagerComponent Manager{};