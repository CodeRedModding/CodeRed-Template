#include "Variables.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	static constexpr uint32_t VARIABLE_NAME_LENGTH = 256; // Max length of a variableId name in character length.
	static constexpr uint32_t VARIABLE_VALUE_LENGTH = 5120; // Max length of a value a variableId can store in character length.

	Variable::Variable(VariableIds id, uint32_t flags) : m_id(id), m_flags(flags) {}

	Variable::Variable(const Variable& variable) : m_id(variable.m_id), m_flags(variable.m_flags) {}

	Variable::~Variable() {}

	VariableIds Variable::GetId() const
	{
		return m_id;
	}

	std::string Variable::GetName() const
	{
		return Variables.GetVariableName(GetId());
	}

	uint32_t Variable::GetFlags() const
	{
		return m_flags;
	}

	Variable& Variable::operator=(const Variable& variable)
	{
		m_id = variable.m_id;
		m_flags = variable.m_flags;
		return *this;
	}

	Setting::Setting(VariableIds id, SettingTypes settingType, const std::string& description, const std::string& defaultValue, bool bHidden) :
		Variable(id, SettingFlags::SETTING_None),
		m_type(settingType),
		m_description(description),
		m_defaultValue(defaultValue),
		m_currentValue(defaultValue),
		m_callback(nullptr),
		m_settingCallback(nullptr),
		m_stringCallback(nullptr),
		m_boolCallback(nullptr)
	{
		SetHidden(bHidden);
	}

	Setting::Setting(const Setting& setting) :
		Variable(setting),
		m_type(setting.m_type),
		m_description(setting.m_description),
		m_defaultValue(setting.m_defaultValue),
		m_currentValue(setting.m_currentValue),
		m_range(setting.m_range),
		m_sharedSettings(setting.m_sharedSettings),
		m_callback(setting.m_callback),
		m_settingCallback(setting.m_settingCallback),
		m_stringCallback(setting.m_stringCallback),
		m_boolCallback(setting.m_boolCallback)
	{

	}

	Setting::~Setting() {}

	SettingTypes Setting::GetType() const
	{
		return m_type;
	}

	bool Setting::IsType(SettingTypes settingType) const
	{
		return (GetType() == settingType);
	}

	const std::string& Setting::GetDescription() const
	{
		return m_description;
	}

	bool Setting::HasValue() const
	{
		return !m_currentValue.empty();
	}

	bool Setting::HasRange() const
	{
		return (!m_range.first.empty() && !m_range.second.empty());
	}

	bool Setting::InRange(const std::string& sValue) const
	{
		if (HasRange())
		{
			if (IsType(SettingTypes::Bool))
			{
				return ((sValue == "0") || (sValue == "false") || (sValue == "1") || (sValue == "true"));
			}
			else if (IsType(SettingTypes::Int32))
			{
				int32_t formattedMin = std::stoi(m_range.first);
				int32_t formattedMax = std::stoi(m_range.second);

				if (Format::IsStringDecimal(sValue))
				{
					int32_t formattedValue = std::stoi(sValue);
					return ((formattedValue >= formattedMin) && (formattedValue <= formattedMax));
				}
			}
			else if (IsType(SettingTypes::Int64))
			{
				int64_t formattedMin = std::stoll(m_range.first);
				int64_t formattedMax = std::stoll(m_range.second);

				if (Format::IsStringDecimal(sValue))
				{
					int64_t formattedValue = std::stoll(sValue);
					return ((formattedValue >= formattedMin) && (formattedValue <= formattedMax));
				}
			}
			else if (IsType(SettingTypes::Float))
			{
				float formattedMin = std::stof(m_range.first);
				float formattedMax = std::stof(m_range.second);

				if (Format::IsStringFloat(sValue))
				{
					float formattedValue = std::stof(sValue);
					return ((formattedValue >= formattedMin) && (formattedValue <= formattedMax));
				}
			}
			else if (IsType(SettingTypes::Rotator))
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
			else if (IsType(SettingTypes::Vector3D))
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
			else if (IsType(SettingTypes::Vector2D))
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
		if (IsType(SettingTypes::Bool))
		{
			if (!(sValue == "1" || sValue == "true" || sValue == "0" || sValue == "false"))
			{
				Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports true or false values.");
				return false;
			}
		}
		if (IsType(SettingTypes::Int32))
		{
			if (!CodeRed::Format::IsStringFloat(sValue))
			{
				Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports floating point numbers or 32 bit integer values.");
				return false;
			}
		}
		else if (IsType(SettingTypes::Float))
		{
			if (!CodeRed::Format::IsStringFloat(sValue))
			{
				Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports floating point numbers or optionally 32 bit integer values.");
				return false;
			}
		}
		else if (IsType(SettingTypes::Color))
		{
			std::string cValue = CodeRed::Format::RemoveAllChars(sValue, '#');

			if (CodeRed::Format::IsStringHexadecimal(cValue) && (cValue.length() <= 8))
			{
				uint64_t dValue = CodeRed::Format::ToDecimal(cValue);

				if (dValue <= 0xFFFFFFFF)
				{
					return true;
				}
			}

			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports RGBA hexadecimal values (alpha value optional).");
			return false;
		}
		else if (IsType(SettingTypes::Rotator))
		{
			std::vector<std::string> values = CodeRed::Format::Split(sValue, ' ');

			if (values.size() >= 3)
			{
				if (CodeRed::Format::IsStringDecimal(values[0]) && CodeRed::Format::IsStringDecimal(values[1]) && CodeRed::Format::IsStringDecimal(values[2]))
				{
					return true;
				}
			}

			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports three 32 bit integer values.");
			return false;
		}
		else if (IsType(SettingTypes::Vector2D))
		{
			std::vector<std::string> values = CodeRed::Format::Split(sValue, ' ');

			if (values.size() >= 2)
			{
				if (CodeRed::Format::IsStringFloat(values[0]) && CodeRed::Format::IsStringFloat(values[1]))
				{
					return true;
				}
			}

			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports two floating point numbers or optionally 32 bit integer values.");
			return false;
		}
		else if (IsType(SettingTypes::Vector3D))
		{
			std::vector<std::string> values = CodeRed::Format::Split(sValue, ' ');

			if (values.size() >= 3)
			{
				if (CodeRed::Format::IsStringFloat(values[0]) && CodeRed::Format::IsStringFloat(values[1]) && CodeRed::Format::IsStringFloat(values[2]))
				{
					return true;
				}
			}

			Console.Warning("[Setting] (" + GetName() + ") Warning: Input is invalid, this setting only supports three floating point numbers or optionally 32 bit integer values.");
			return false;
		}

		return true;
	}

	bool Setting::IsHidden(bool bSkipDev) const
	{
#ifdef DEV_BUILD
		if (!bSkipDev)
		{
			return false;
		}
#endif
		return (GetFlags() & SettingFlags::SETTING_Hidden);
	}

	bool Setting::IsLocked() const
	{
		return (GetFlags() & SettingFlags::SETTING_Locked);
	}

	bool Setting::HasCallback() const
	{
		return !!m_callback;
	}

	bool Setting::HasSettingCallback() const
	{
		return !!m_settingCallback;
	}

	bool Setting::HasStringCallback() const
	{
		return !!m_stringCallback;
	}

	bool Setting::HasBoolCallback() const
	{
		return !!m_boolCallback;
	}

	const std::string& Setting::GetDefaultValue() const
	{
		return m_defaultValue;
	}

	const std::string& Setting::GetStringValue() const
	{
		return m_currentValue;
	}

	bool Setting::GetBoolValue() const
	{
		if (IsType(SettingTypes::Bool))
		{
			if ((GetStringValue() == "true") || (GetStringValue() == "1"))
			{
				return true;
			}
		}

		return false;
	}

	uint8_t Setting::GetByteValue() const
	{
		if (IsType(SettingTypes::Byte) || IsType(SettingTypes::Int32))
		{
			int32_t value = std::stoi(GetStringValue());

			if ((value >= 0) && (value <= UINT8_MAX))
			{
				return value;
			}
		}

		return 0;
	}

	int32_t Setting::GetInt32Value() const
	{
		if (IsType(SettingTypes::Int32))
		{
			return std::stoi(GetStringValue());
		}
		else if (IsType(SettingTypes::Int64))
		{
			int64_t value = std::stoll(GetStringValue());

			if ((value >= INT32_MIN) && (value <= INT32_MAX))
			{
				return static_cast<int32_t>(value);
			}
		}
		else if (IsType(SettingTypes::Float))
		{
			return static_cast<int32_t>(GetFloatValue());
		}

		return 0;
	}

	int64_t Setting::GetInt64Value() const
	{
		if (IsType(SettingTypes::Int32) || IsType(SettingTypes::Int64))
		{
			return std::stoll(GetStringValue());
		}
		else if (IsType(SettingTypes::Float))
		{
			return static_cast<int64_t>(GetFloatValue());
		}

		return 0;
	}

	float Setting::GetFloatValue() const
	{
		if (IsType(SettingTypes::Float))
		{
			return std::stof(GetStringValue());
		}
		else if (IsType(SettingTypes::Int32))
		{
			return static_cast<float>(GetInt32Value());
		}
		else if (IsType(SettingTypes::Int64))
		{
			int64_t value = GetInt64Value();

			if ((value >= FLT_MIN) && (value <= FLT_MAX))
			{
				return static_cast<float>(value);
			}
		}

		return 0.0f;
	}

	Color Setting::GetColorValue() const
	{
		if (IsType(SettingTypes::Color))
		{
			return Color(GetStringValue());
		}

		return Color();
	}

	LinearColor Setting::GetLinearValue() const
	{
		if (IsType(SettingTypes::Color))
		{
			return LinearColor(GetStringValue());
		}

		return LinearColor();
	}

	Rotator Setting::GetRotatorValue() const
	{
		if (IsType(SettingTypes::Rotator))
		{
			return Format::ToRotator(GetStringValue());
		}

		return Rotator();
	}

	VectorF Setting::GetVector3DFValue() const
	{
		if (IsType(SettingTypes::Vector3D) || IsType(SettingTypes::Vector2D))
		{
			return Format::ToVectorF(GetStringValue());
		}

		return VectorF();
	}

	VectorI Setting::GetVector3DIValue() const
	{
		if (IsType(SettingTypes::Vector3D) || IsType(SettingTypes::Vector2D))
		{
			return Format::ToVectorI(GetStringValue());
		}

		return VectorI();
	}

	Vector2DF Setting::GetVector2DFValue() const
	{
		if (IsType(SettingTypes::Vector3D) || IsType(SettingTypes::Vector2D))
		{
			return Format::ToVector2DF(GetStringValue());
		}

		return Vector2DF();
	}

	Vector2DI Setting::GetVector2DIValue() const
	{
		if (IsType(SettingTypes::Vector3D) || IsType(SettingTypes::Vector2D))
		{
			return Format::ToVector2DI(GetStringValue());
		}

		return Vector2DI();
	}

	Setting* Setting::ResetToDefault(ThreadTypes thread)
	{
		return SetStringValue(GetDefaultValue(), thread);
	}

	Setting* Setting::SetStringValue(const std::string& sValue, ThreadTypes thread, bool bSkipSave)
	{
		if (thread != ThreadTypes::Main)
		{
			Manager.InternalCommand(GetName(), sValue, thread, bSkipSave);
		}
		else if (IsValueValid(sValue) && !IsLocked())
		{
			if (InRange(sValue))
			{
				if (IsType(SettingTypes::Bool))
				{
					if ((sValue == "true") || (sValue == "1"))
					{
						m_currentValue = "true";
					}
					else
					{
						m_currentValue = "false";
					}
				}
				else if (IsType(SettingTypes::Float))
				{
					m_currentValue = Format::RemoveAllChars(sValue, 'f');
				}
				else if (IsType(SettingTypes::Color))
				{
					m_currentValue = Color(sValue).ToHexAlpha();
				}
				else
				{
					m_currentValue = sValue;
				}

				if (m_currentValue.length() > VARIABLE_VALUE_LENGTH)
				{
					m_currentValue = m_currentValue.substr(0, VARIABLE_VALUE_LENGTH);
				}

				TriggerCallbacks(bSkipSave);
			}
			else
			{
				Console.Warning("[Setting] (" + GetName() + ") Warning: Input is out of range, this setting has a minimum value of \"" + m_range.first + "\" and a maximum value of \"" + m_range.second + "\".");
			}
		}

		return this;
	}

	Setting* Setting::SetBoolValue(bool bValue, ThreadTypes thread)
	{
		return SetStringValue((bValue ? "true" : "false"), thread);
	}

	Setting* Setting::SetInt32Value(int32_t iValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(iValue), thread);
	}

	Setting* Setting::SetInt64Value(int64_t iValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(iValue), thread);
	}

	Setting* Setting::SetFloatValue(float fValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(fValue), thread);
	}

	Setting* Setting::SetColorValue(const Color& cValue, ThreadTypes thread)
	{
		return SetStringValue(cValue.ToHexAlpha(), thread);
	}

	Setting* Setting::SetLinearValue(const LinearColor& lValue, ThreadTypes thread)
	{
		return SetStringValue(lValue.ToHexAlpha(), thread);
	}

	Setting* Setting::SetRotatorValue(const Rotator& rValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(rValue.Pitch) + " " + std::to_string(rValue.Yaw) + " " + std::to_string(rValue.Roll), thread);
	}

	Setting* Setting::SetVectorFValue(const VectorF& vfValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(vfValue.X) + " " + std::to_string(vfValue.Y) + " " + std::to_string(vfValue.Z), thread);
	}

	Setting* Setting::SetVectorIValue(const VectorI& viValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(viValue.X) + " " + std::to_string(viValue.Y) + " " + std::to_string(viValue.Z), thread);
	}

	Setting* Setting::SetVector2DFValue(const Vector2DF& vfValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(vfValue.X) + " " + std::to_string(vfValue.Y), thread);
	}

	Setting* Setting::SetVector2DIValue(const Vector2DI& viValue, ThreadTypes thread)
	{
		return SetStringValue(std::to_string(viValue.X) + " " + std::to_string(viValue.Y), thread);
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

	Setting* Setting::SetByteRange(uint8_t minValue, uint8_t maxValue)
	{
		return SetStringRange(std::to_string(static_cast<int32_t>(minValue)), std::to_string(static_cast<int32_t>(maxValue)));
	}

	Setting* Setting::SetInt32Range(int32_t minValue, int32_t maxValue)
	{
		return SetStringRange(std::to_string(minValue), std::to_string(maxValue));
	}

	Setting* Setting::SetInt64Range(int64_t minValue, int64_t maxValue)
	{
		return SetStringRange(std::to_string(minValue), std::to_string(maxValue));
	}

	Setting* Setting::SetFloatRange(float minValue, float maxValue)
	{
		return SetStringRange(std::to_string(minValue), std::to_string(maxValue));
	}

	Setting* Setting::SetRotatorRange(const Rotator& minValue, const Rotator& maxValue)
	{
		return SetStringRange(std::to_string(minValue.Pitch) + " " + std::to_string(minValue.Yaw) + " " + std::to_string(minValue.Roll), std::to_string(maxValue.Pitch) + " " + std::to_string(maxValue.Yaw) + " " + std::to_string(maxValue.Roll));
	}

	Setting* Setting::SetVectorFRange(const VectorF& minValue, const VectorF& maxValue)
	{
		return SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y) + " " + std::to_string(minValue.Z), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y) + " " + std::to_string(maxValue.Z));
	}

	Setting* Setting::SetVectorIRange(const VectorI& minValue, const VectorI& maxValue)
	{
		return SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y) + " " + std::to_string(minValue.Z), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y) + " " + std::to_string(maxValue.Z));
	}

	Setting* Setting::SetVector2DFRange(const Vector2DF& minValue, const Vector2DF& maxValue)
	{
		return SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y));
	}

	Setting* Setting::SetVector2DIRange(const Vector2DI& minValue, const Vector2DI& maxValue)
	{
		return SetStringRange(std::to_string(minValue.X) + " " + std::to_string(minValue.Y), std::to_string(maxValue.X) + " " + std::to_string(maxValue.Y));
	}

	std::pair<int32_t, int32_t> Setting::GetInt32Range() const
	{
		std::pair<int32_t, int32_t> returnRange;

		if (HasRange())
		{
			if (CodeRed::Format::IsStringDecimal(m_range.first) && CodeRed::Format::IsStringDecimal(m_range.second))
			{
				returnRange.first = std::stoi(m_range.first);
				returnRange.second = std::stoi(m_range.second);
			}
		}

		return returnRange;
	}

	std::pair<int64_t, int64_t> Setting::GetInt64Range() const
	{
		std::pair<int64_t, int64_t> returnRange;

		if (HasRange())
		{
			if (CodeRed::Format::IsStringDecimal(m_range.first) && CodeRed::Format::IsStringDecimal(m_range.second))
			{
				returnRange.first = std::stoll(m_range.first);
				returnRange.second = std::stoll(m_range.second);
			}
		}

		return returnRange;
	}

	std::pair<float, float> Setting::GetFloatRange() const
	{
		std::pair<float, float> returnRange{ 0.0f, 0.0f };

		if (HasRange())
		{
			if (CodeRed::Format::IsStringFloat(m_range.first) && CodeRed::Format::IsStringFloat(m_range.second))
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
			returnRange.first = CodeRed::Format::ToRotator(m_range.first);
			returnRange.second = CodeRed::Format::ToRotator(m_range.second);
		}

		return returnRange;
	}

	std::pair<VectorF, VectorF> Setting::GetVectorFRange() const
	{
		std::pair<VectorF, VectorF> returnRange;

		if (HasRange())
		{
			returnRange.first = CodeRed::Format::ToVectorF(m_range.first);
			returnRange.second = CodeRed::Format::ToVectorF(m_range.second);
		}

		return returnRange;
	}

	std::pair<VectorI, VectorI> Setting::GetVectorIRange() const
	{
		std::pair<VectorI, VectorI> returnRange;

		if (HasRange())
		{
			returnRange.first = CodeRed::Format::ToVectorI(m_range.first);
			returnRange.second = CodeRed::Format::ToVectorI(m_range.second);
		}

		return returnRange;
	}

	std::pair<Vector2DF, Vector2DF> Setting::GetVector2DFRange() const
	{
		std::pair<Vector2DF, Vector2DF> returnRange;

		if (HasRange())
		{
			returnRange.first = CodeRed::Format::ToVector2DF(m_range.first);
			returnRange.second = CodeRed::Format::ToVector2DF(m_range.second);
		}

		return returnRange;
	}

	std::pair<Vector2DI, Vector2DI> Setting::GetVector2DIRange() const
	{
		std::pair<Vector2DI, Vector2DI> returnRange;

		if (HasRange())
		{
			returnRange.first = CodeRed::Format::ToVector2DI(m_range.first);
			returnRange.second = CodeRed::Format::ToVector2DI(m_range.second);
		}

		return returnRange;
	}

	Setting* Setting::SetHidden(bool bHidden)
	{
		if (bHidden)
		{
			m_flags |= SettingFlags::SETTING_Hidden;
		}
		else
		{
			m_flags &= ~SettingFlags::SETTING_Hidden;
		}

		return this;
	}

	Setting* Setting::SetLocked(bool bLocked)
	{
		if (bLocked)
		{
			m_flags |= SettingFlags::SETTING_Locked;
		}
		else
		{
			m_flags &= ~SettingFlags::SETTING_Locked;
		}

		return this;
	}

	Setting* Setting::LockWithValue(const std::string& sValue)
	{
		if (IsValueValid(sValue) && InRange(sValue))
		{
			SetLocked(false);
			SetStringValue(sValue);
			SetLocked(true);
		}

		return this;
	}

	Setting* Setting::UnbindCallbacks()
	{
		m_callback = nullptr;
		m_settingCallback = nullptr;
		m_boolCallback = nullptr;
		m_stringCallback = nullptr;
		return this;
	}

	Setting* Setting::BindCallback(const std::function<void()>& callback)
	{
		m_callback = callback;
		return this;
	}

	Setting* Setting::BindSettingCallback(const std::function<void(class Setting*)>& callback)
	{
		m_settingCallback = callback;
		return this;
	}

	Setting* Setting::BindBoolCallback(const std::function<void(bool)>& callback)
	{
		m_boolCallback = callback;
		return this;
	}

	Setting* Setting::BindStringCallback(const std::function<void(std::string)>& callback)
	{
		m_stringCallback = callback;
		return this;
	}

	void Setting::TriggerCallbacks(bool bSkipSave)
	{
		if (HasCallback())
		{
			m_callback();
		}
		else if (HasSettingCallback())
		{
			m_settingCallback(this);
		}
		else if (HasStringCallback())
		{
			m_stringCallback(GetStringValue());
		}
		else if (HasBoolCallback())
		{
			m_boolCallback(GetBoolValue());
		}

		if (!bSkipSave)
		{
			Variables.WriteVariables();
		}
	}

	Setting& Setting::operator=(const Setting& setting)
	{
		m_id = setting.m_id;
		m_flags = setting.m_flags;
		m_type = setting.m_type;
		m_description = setting.m_description;
		m_defaultValue = setting.m_defaultValue;
		m_currentValue = setting.m_currentValue;
		m_range = setting.m_range;
		m_sharedSettings = setting.m_sharedSettings;
		m_callback = setting.m_callback;
		m_settingCallback = setting.m_settingCallback;
		m_stringCallback = setting.m_stringCallback;
		m_boolCallback = setting.m_boolCallback;
		return *this;
	}

	Command::Command(VariableIds id, const std::string& description, bool bHidden) :
		Variable(id, CommandFlags::COMMAND_None),
		m_description(description),
		m_callback(nullptr),
		m_stringCallback(nullptr)
	{
		SetHidden(bHidden);
		SetNeedsArgs(true);
	}

	Command::Command(const Command& command) :
		Variable(command),
		m_description(command.m_description),
		m_callback(command.m_callback),
		m_stringCallback(command.m_stringCallback)
	{

	}

	Command::~Command() {}

	const std::string& Command::GetDescription() const
	{
		return m_description;
	}

	bool Command::IsHidden() const
	{
		return (GetFlags() & CommandFlags::COMMAND_Hidden);
	}

	bool Command::IsLocked() const
	{
		return (GetFlags() & CommandFlags::COMMAND_Locked);
	}

	bool Command::NeedsArgs() const
	{
		return (GetFlags() & CommandFlags::COMMAND_NeedsArgs);
	}

	bool Command::HasCallback() const
	{
		return !!m_callback;
	}

	bool Command::HasArgumentCallback() const
	{
		return !!m_stringCallback;
	}

	void Command::TriggerCallback() const
	{
		if (HasCallback())
		{
			if (!IsLocked())
			{
				m_callback();
			}
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
			if (!IsLocked())
			{
				m_stringCallback(arguments);
			}
		}
		else
		{
			Console.Warning("[Command] (" + GetName() + ") Warning: Unbound argument callback triggered!");
		}
	}

	Command* Command::SetHidden(bool bHidden)
	{
		if (bHidden)
		{
			m_flags |= CommandFlags::COMMAND_Hidden;
		}
		else
		{
			m_flags &= ~CommandFlags::COMMAND_Hidden;
		}

		return this;
	}

	Command* Command::SetLocked(bool bLocked)
	{
		if (bLocked)
		{
			m_flags |= CommandFlags::COMMAND_Locked;
		}
		else
		{
			m_flags &= ~CommandFlags::COMMAND_Locked;
		}

		return this;
	}

	Command* Command::SetNeedsArgs(bool bNeedsArgs)
	{
		if (bNeedsArgs)
		{
			m_flags |= CommandFlags::COMMAND_NeedsArgs;
		}
		else
		{
			m_flags &= ~CommandFlags::COMMAND_NeedsArgs;
		}

		return this;
	}

	Command* Command::BindCallback(const std::function<void()>& callback)
	{
		m_callback = callback;
		return this;
	}

	Command* Command::BindStringCallback(const std::function<void(std::string)>& callback)
	{
		m_stringCallback = callback;
		return this;
	}

	Command* Command::UnbindCallbacks()
	{
		m_callback = nullptr;
		m_stringCallback = nullptr;
		return this;
	}

	Command& Command::operator=(const Command& command)
	{
		m_id = command.m_id;
		m_flags = command.m_flags;
		m_description = command.m_description;
		m_callback = command.m_callback;
		m_stringCallback = command.m_stringCallback;
		return *this;
	}

	VariableComponent::VariableComponent() : Component("Variables", "Manages commands and settings.") { OnCreate(); }

	VariableComponent::~VariableComponent() { OnDestroy(); }

	void VariableComponent::OnCreate()
	{
		m_variablesParsed = false;
		m_writingLocked = false;
	}

	void VariableComponent::OnDestroy() {}

	bool VariableComponent::Initialize()
	{
		if (!IsInitialized())
		{
			CreateCommand("reset_setting", new Command(VariableIds::MANAGER_RESET_SETTING, "Reset a setting to its default/original value."))->BindStringCallback([&](const std::string& arguments) { ResetSetting(arguments); });
			CreateCommand("unreal_command", new Command(VariableIds::MANAGER_UNREAL_COMMAND, "Execute a Unreal Engine 3 command with the given arguments."))->BindStringCallback([&](const std::string& arguments) { Manager.UnrealCommand(arguments); });

			Console.Write(GetNameFormatted() + std::to_string(m_commands.size()) + " Command(s) Initialized!");
			Console.Write(GetNameFormatted() + std::to_string(m_settings.size()) + " Setting(s) Initialized!");

			Console.Success(GetNameFormatted() + "Initialized!");
			SetInitialized(true);
		}

		return IsInitialized();
	}

	void VariableComponent::SetupVariables()
	{
		if (!m_variablesParsed)
		{
			ParseVariables(); // Loads all of the users previously saved variables, and then updates them with any newly added ones.
			WriteVariables(); // Writes any new settings as well as cleanup any invalid setting ranges or values.
		}
	}

	bool VariableComponent::CreateVariable(std::string variableName, VariableIds variableId)
	{
		if (!variableName.empty())
		{
			Format::ToLowerInline(variableName);

			if (variableName.length() > VARIABLE_NAME_LENGTH)
			{
				Console.Warning("(CreateVariable) Warning: Variable name exceeds maximum length for \"" + variableName + "\"!");
				variableName = variableName.substr(0, VARIABLE_NAME_LENGTH);
			}

			for (const auto& variablePair : m_variables)
			{
				if (variablePair.second == variableName)
				{
					Console.Warning("(CreateVariable) Warning: Duplicate variable name detected for \"" + variableName + "\"!");
					return false;
				}
			}

			if (!m_variables.contains(variableId))
			{
				m_variables[variableId] = variableName;
				return true;
			}
			else
			{
				Console.Warning("(CreateVariable) Warning: Duplicate variable id detected for \"" + std::to_string(static_cast<int32_t>(variableId)) + "\"!");
			}
		}

		return false;
	}

	std::string VariableComponent::GetVariableName(VariableIds variableId) const
	{
		if (m_variables.contains(variableId))
		{
			return m_variables.at(variableId);
		}
		else
		{
			Console.Error("(GetVariableName) Error: Variable name not found for \"" + std::to_string(static_cast<int32_t>(variableId)) + "\"!");
		}

		return "";
	}

	std::shared_ptr<Command> VariableComponent::CreateCommand(const std::string& commandName, Command* command)
	{
		if (!commandName.empty() && command)
		{
			if (CreateVariable(commandName, command->GetId()))
			{
				if (!m_commands.contains(commandName))
				{
					if (!command->IsHidden())
					{
						Console.Notify("(CreateCommand) Created command \"" + commandName + "\".");
					}

					m_commands[commandName] = std::shared_ptr<Command>(command);
					return m_commands[commandName];
				}
			}
		}

		return nullptr;
	}

	std::shared_ptr<Command> VariableComponent::GetCommand(const std::string& commandName)
	{
		if (m_commands.contains(commandName))
		{
			return m_commands[commandName];
		}
		else
		{
			Console.Warning("(GetCommand) Warning: Failed to find a command with the name \"" + commandName + "\"!");
		}

		return nullptr;
	}

	std::shared_ptr<Command> VariableComponent::GetCommand(VariableIds variableId)
	{
		return GetCommand(GetVariableName(variableId));
	}

	std::shared_ptr<Setting> VariableComponent::CreateSetting(const std::string& settingName, Setting* setting)
	{
		if (!settingName.empty() && setting)
		{
			if (CreateVariable(settingName, setting->GetId()))
			{
				if (!m_settings.contains(settingName))
				{
					if (!setting->IsHidden())
					{
						Console.Notify("(CreateSetting) Created setting \"" + settingName + "\".");
					}

					m_settings[settingName] = std::shared_ptr<Setting>(setting);
					return m_settings[settingName];
				}
			}
		}

		return nullptr;
	}

	std::shared_ptr<Setting> VariableComponent::GetSetting(const std::string& settingName)
	{
		if (m_settings.contains(settingName))
		{
			return m_settings[settingName];
		}
		else
		{
			Console.Warning("(GetSetting) Warning: Failed to find a setting with the name \"" + settingName + "\"!");
		}

		return nullptr;
	}

	std::shared_ptr<Setting> VariableComponent::GetSetting(VariableIds variableId)
	{
		return GetSetting(GetVariableName(variableId));
	}

	void VariableComponent::WriteVariables() const
	{
		if (!m_writingLocked && m_variablesParsed)
		{
#ifdef WALKTHROUGH
			Console.Notify("\"Components\\Components\\Variables.cpp -> VariableComponent::WriteVariables\". Looks like you forgot to check this file, you have no path set for saving or loading variables!");
			return;
#endif
			std::filesystem::path filePath("YOUR_FILE_PATH_HERE");

			if (std::filesystem::exists(filePath))
			{
				std::ofstream variablesFile(filePath);

				for (const auto& settingPair : m_settings)
				{
					if (settingPair.second)
					{
						std::string value = settingPair.second->GetStringValue();

						if (value.length() > VARIABLE_VALUE_LENGTH)
						{
							value = value.substr(0, VARIABLE_VALUE_LENGTH);
						}

						variablesFile << settingPair.first << " " << value << std::endl;
					}
				}

				variablesFile.close();
			}
		}
	}

	void VariableComponent::ResetSetting(const std::string& settingName, bool bPrintToConsole)
	{
		std::shared_ptr<Setting> setting = GetSetting(settingName);

		if (setting)
		{
			setting->ResetToDefault();

			if (bPrintToConsole)
			{
				Console.Notify("(ResetSetting) Reset setting to its default value!");
				Console.Notify("(ResetSetting) Name: " + setting->GetName());
				Console.Notify("(ResetSetting) Description: " + setting->GetDescription());
				Console.Notify("(ResetSetting) Default value: " + setting->GetStringValue());
			}
		}
		else if (bPrintToConsole)
		{
			Console.Error("(ResetSetting) Error: Unrecognized setting \"" + settingName + "\"!");
		}
	}

	void VariableComponent::ToggleSetting(const std::string& settingName, bool bPrintToConsole)
	{
		std::shared_ptr<Setting> setting = GetSetting(settingName);

		if (setting)
		{
			if (setting->IsType(SettingTypes::Bool))
			{
				bool oldValue = setting->GetBoolValue();

				if (bPrintToConsole)
				{
					if (oldValue)
					{
						Console.Notify("(ToggleSetting) " + settingName + ": true -> false");
					}
					else
					{
						Console.Notify("(ToggleSetting) " + settingName + ": " + "false -> true");
					}
				}

				setting->SetBoolValue(!oldValue);
			}
			else if (bPrintToConsole)
			{
				Console.Error("(ToggleSetting) Error: The setting \"" + settingName + "\" does not accept booleans, it cannot be toggled!");
			}
		}
		else if (bPrintToConsole)
		{
			Console.Error("(ToggleSetting) Error: Unrecognized setting \"" + settingName + "\"!");
		}
	}

	void VariableComponent::FindUnusedVariables() const
	{
		for (uint32_t i = (static_cast<uint32_t>(VariableIds::UNKNOWN) + 1); i < static_cast<uint32_t>(VariableIds::END); i++)
		{
			if (!m_variables.contains(static_cast<VariableIds>(i)))
			{
				Console.Warning("(VerifyVariables) Warning: Unused variable id \"" + std::to_string(i) + "\" detected!");
			}
		}
	}

	void VariableComponent::ParseVariables()
	{
		if (!m_variablesParsed)
		{
#ifdef WALKTHROUGH
			Console.Notify("\"Components\\Components\\Variables.cpp -> VariableComponent::ParseVariables\". Looks like you forgot to check this file, you have no path set for saving or loading variables!");
			return;
#endif
			std::filesystem::path filePath("YOUR_FILE_PATH_HERE");

			if (std::filesystem::exists(filePath))
			{
				m_writingLocked = true;

				std::vector<std::string> variables;
				std::ifstream fileStream(filePath);
				std::string line;

				while (std::getline(fileStream, line))
				{
					variables.push_back(line);
				}

				fileStream.close();

				for (const std::string& variable : variables)
				{
					size_t spacePos = variable.find_first_of(" ");

					if (spacePos != std::string::npos)
					{
						std::string name = variable.substr(0, spacePos);
						std::shared_ptr<Setting> setting = GetSetting(name);

						if (setting)
						{
							std::string value = variable.substr((spacePos + 1), variable.length());

							if (!value.empty() && !setting->IsLocked())
							{
								if (setting->IsValueValid(value))
								{
									setting->SetStringValue(value);
								}
								else
								{
									Console.Warning("(ParseVariables) Warning: Found invalid data for \"" + name + "\" detected, \"" + value + "\" is not supported for this setting!");
								}
							}
						}
						else
						{
							Console.Warning("(ParseVariables) Warning: Could not find a variableId with the name \"" + name + "\"!");
						}
					}
					else
					{
						Console.Warning("(ParseVariables) Warning: Invalid variableId format detected for \"" + variable + "\"!");
					}
				}

				m_writingLocked = false;
				m_variablesParsed = true;
			}
			else
			{
				Console.Warning("(ParseVariables) Warning: Failed to locate variables file, everything will be reset to their default values!");
				m_writingLocked = false;
				m_variablesParsed = true;
				WriteVariables();
			}
		}
	}

	class VariableComponent Variables;
}