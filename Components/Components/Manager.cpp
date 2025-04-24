#include "Manager.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	static constexpr uint32_t VARIABLE_NAME_LENGTH = 256; // Max length of a variable name in character length.
	static constexpr uint32_t VARIABLE_VALUE_LENGTH = 5120; // Max length of a value a variable can store in character length.

	Variable::Variable(VariableIds id, uint32_t flags) : m_id(id), m_flags(flags) {}

	Variable::Variable(const Variable& variable) : m_id(variable.m_id), m_flags(variable.m_flags) {}

	Variable::~Variable() {}

	VariableIds Variable::GetId() const
	{
		return m_id;
	}

	std::string Variable::GetName() const
	{
		return Manager.GetVariableName(GetId());
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
		m_toggleCallback(nullptr),
		m_stringCallback(nullptr)
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
		m_toggleCallback(setting.m_toggleCallback),
		m_stringCallback(setting.m_stringCallback)
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

	bool Setting::HasToggleCallback() const
	{
		return !!m_toggleCallback;
	}

	bool Setting::HasArgumentCallback() const
	{
		return !!m_stringCallback;
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

	Setting* Setting::SetStringValue(const std::string& sValue, ThreadTypes thread)
	{
		if (thread != ThreadTypes::Main)
		{
			Manager.InternalCommand(GetName(), sValue, thread);
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

				TriggerCallbacks();
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
		m_toggleCallback = nullptr;
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
		m_toggleCallback = callback;
		return this;
	}

	Setting* Setting::BindStringCallback(const std::function<void(std::string)>& callback)
	{
		m_stringCallback = callback;
		return this;
	}

	void Setting::TriggerCallbacks()
	{
		if (HasCallback())
		{
			m_callback();
		}
		else if (HasToggleCallback())
		{
			m_toggleCallback(GetBoolValue());
		}
		else if (HasSettingCallback())
		{
			m_settingCallback(this);
		}
		else if (HasArgumentCallback())
		{
			m_stringCallback(GetStringValue());
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
		m_toggleCallback = setting.m_toggleCallback;
		m_stringCallback = setting.m_stringCallback;
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

	QueueData::QueueData(const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave) :
		Thread(ThreadTypes::Main),
		Command(command),
		Arguments(arguments),
		Delay(0),
		Delta(0),
		Internal(bInternal),
		Completed(false)
	{

	}

	QueueData::QueueData(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier, bool bSkipSave) :
		Thread(ThreadTypes::Main),
		Command(command),
		Arguments(arguments),
		Delay(delay* multiplier),
		Delta(0),
		Internal(bInternal),
		Completed(false)
	{

	}

	QueueData::QueueData(const QueueData& queueData) :
		Thread(queueData.Thread),
		Command(queueData.Command),
		Arguments(queueData.Arguments),
		Delay(queueData.Delay),
		Delta(queueData.Delta),
		Internal(queueData.Internal),
		Completed(queueData.Completed)
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
		Thread = queueData.Thread;
		Command = queueData.Command;
		Arguments = queueData.Arguments;
		Delay = queueData.Delay;
		Delta = queueData.Delta;
		Internal = queueData.Internal;
		Completed = queueData.Completed;
		return *this;
	}

	ManagerComponent::ManagerComponent() : Component("Manager", "Manages settings, commands, and modules.") { OnCreate(); }

	ManagerComponent::~ManagerComponent() { OnDestroy(); }

	void ManagerComponent::OnCreate()
	{
		PlaceholderMod = nullptr;
		m_queueLocked = false;
	}

	void ManagerComponent::OnDestroy()
	{
		m_variables.clear();
		m_modules.clear();
		m_commands.clear();
		m_settings.clear();
		PlaceholderMod = nullptr;
	}

	void ManagerComponent::OnTick()
	{
		GRainbowColor::OnTick();

		if (!m_queueLocked)
		{
			if (!m_threadQueue.empty())
			{
				for (QueueData& data : m_threadQueue)
				{
					m_queue.push_back(data);
				}

				m_threadQueue.clear();
			}

			if (!m_queue.empty())
			{
				bool safeToClear = true;

				for (QueueData& data : m_queue)
				{
					if (!data.Completed)
					{
						safeToClear = false;

						if (data.IsAsync())
						{
							if (data.OnTick())
							{
								data.Delay = 0;
							}
							else
							{
								continue;
							}
						}

						data.Completed = true;

						if ((data.Thread == ThreadTypes::Main) || (data.Thread == ThreadTypes::Render))
						{
							if (data.Internal)
							{
								InternalCommand(data.Command, data.Arguments, ThreadTypes::Main);
							}
							else
							{
								ConsoleCommand(data.Command, data.Arguments, ThreadTypes::Main);
							}
						}
						else
						{
							return;
						}
					}
				}

				if (safeToClear)
				{
					m_queue.clear();
				}
			}
		}
	}

	void ManagerComponent::OnCanvasDraw(class UCanvas* unrealCanvas)
	{
		OnTick();

		if (unrealCanvas)
		{
			for (auto& modulePair : m_modules)
			{
				if (modulePair.second)
				{
					modulePair.second->OnCanvasDraw(unrealCanvas);
				}
			}
		}
	}

	bool ManagerComponent::CreateVariable(std::string name, VariableIds variable)
	{
		if (!name.empty())
		{
			CodeRed::Format::ToLowerInline(name);

			if (name.length() > VARIABLE_NAME_LENGTH)
			{
				Console.Warning("(CreateVariable) Warning: Variable name exceeds maximum length for \"" + name + "\"!");
				name = name.substr(0, VARIABLE_NAME_LENGTH);
			}

			for (const auto& variablePair : m_variables)
			{
				if (variablePair.second == name)
				{
					Console.Warning("[Manager Component] Warning: Duplicate variable name detected for \"" + name + "\"!");
					return false;
				}
			}

			if (!m_variables.contains(variable))
			{
				m_variables[variable] = name;
				return true;
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

		return false;
	}

	void ManagerComponent::QueueCommand(const QueueData& queueData, ThreadTypes thread)
	{
		if (!queueData.Command.empty())
		{
			QueueData queueCopy = queueData;
			queueCopy.Thread = thread;

			if (m_queueLocked)
			{
				std::thread queueThread([this, queueCopy]() {
					while (m_queueLocked)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}

					m_queueLocked = true;
					m_threadQueue.push_back(queueCopy);
					m_queueLocked = false;
					});

				queueThread.detach();
			}
			else
			{
				m_queueLocked = true;
				m_threadQueue.push_back(queueCopy);
				m_queueLocked = false;
			}
		}
	}

	void ManagerComponent::ProcessCommand(const QueueData& queueData, ThreadTypes thread)
	{
		if (thread != ThreadTypes::Main)
		{
			if (!queueData.Command.empty())
			{
				QueueCommand(queueData, thread);
			}
		}
		else
		{
			std::pair<CommandTypes, std::string> returnType = ProcessCommandInternal(queueData, thread);

			if (returnType.first != CommandTypes::None)
			{
				if (returnType.first == CommandTypes::Unrecognized)
				{
					Console.Error(GetNameFormatted() + "Unrecognized command or setting: \"" + queueData.Command + "\".");
				}
				else if (returnType.first == CommandTypes::EmptyArguments)
				{
					Console.Error(GetNameFormatted() + "Invalid arguments provided for: \"" + queueData.Command + "\".");
				}
				else if (returnType.first == CommandTypes::InvalidArguments)
				{
					Console.Error(GetNameFormatted() + "Arguments are not supported for: \"" + queueData.Command + "\".");
				}
				else if (returnType.first == CommandTypes::ModifySetting)
				{
					std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

					if (consoleSetting)
					{
						Console.Notify(GetNameFormatted() + consoleSetting->GetName() + ": " + returnType.second + " -> " + consoleSetting->GetStringValue());
					}
				}
				else if (returnType.first == CommandTypes::PrintSetting)
				{
					std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

					if (consoleSetting)
					{
						Console.Notify(GetNameFormatted() + "Name: " + consoleSetting->GetName());
						Console.Notify(GetNameFormatted() + "Description: " + consoleSetting->GetDescription());
						Console.Notify(GetNameFormatted() + "Current Value: " + consoleSetting->GetStringValue());
					}
				}
			}
		}
	}

	std::pair<CommandTypes, std::string> ManagerComponent::ProcessCommandInternal(const QueueData& queueData, ThreadTypes thread)
	{
		if ((thread != ThreadTypes::Main) || (queueData.IsAsync()))
		{
			if (!queueData.Command.empty())
			{
				QueueCommand(queueData, thread);
				return { CommandTypes::Queued, "" };
			}
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
					if (queueData.HasArguments() || !consoleCommand->NeedsArgs())
					{
						consoleCommand->TriggerCallback(queueData.Arguments);
					}
					else
					{
						return { CommandTypes::EmptyArguments, "" };
					}
				}
				else
				{
					return { CommandTypes::InvalidArguments, "" };
				}
			}
			else
			{
				std::shared_ptr<Setting> consoleSetting = GetSetting(queueData.Command);

				if (consoleSetting && !consoleSetting->IsLocked())
				{
					if (queueData.HasArguments())
					{
						std::string oldValue = consoleSetting->GetStringValue();
						consoleSetting->SetStringValue(queueData.Arguments);
						return { CommandTypes::ModifySetting, oldValue };
					}

					return { CommandTypes::PrintSetting, "" };
				}

				return { CommandTypes::Unrecognized, "" };
			}
		}

		return { CommandTypes::None, "" };
	}

	std::pair<CommandTypes, std::string> ManagerComponent::InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread)
	{
		return ProcessCommandInternal(QueueData(command, arguments, true), thread);
	}

	std::pair<CommandTypes, std::string> ManagerComponent::InternalCommand(const std::string& command, ThreadTypes thread)
	{
		return InternalCommand(command, "", thread);
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread)
	{
		ProcessCommand(QueueData(command, arguments, false), thread);
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, ThreadTypes thread)
	{
		ConsoleCommand(command, "", thread);
	}

	void ManagerComponent::AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread)
	{
		ProcessCommand(QueueData(command, arguments, false, delay), thread);
	}

	void ManagerComponent::AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread)
	{
		ProcessCommand(QueueData(command, "", false, delay), thread);
	}

	void ManagerComponent::UnrealCommand(std::string unrealCommand, bool bPrintToConsole)
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
				unrealCommand = unrealCommand.erase(0, 15);
			}

			if (bPrintToConsole)
			{
				Console.Write(GetNameFormatted() + "Executing unreal command \"" + unrealCommand + "\".");
			}

			defaultActor->ConsoleCommand(unrealCommand); // May need your own function to convert std::string to FString if your game uses wchar_t.
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

	std::string ManagerComponent::GetVariableName(VariableIds variable)
	{
		if (m_variables.contains(variable))
		{
			return m_variables[variable];
		}

		return "UnknownVariable";
	}

	template <typename T> std::shared_ptr<T> ManagerComponent::CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind)
	{
		if (mod)
		{
			std::string moduleName = mod->GetName();

			if (!m_modules.contains(moduleName))
			{
				m_modules[moduleName] = std::shared_ptr<Module>(mod);
				moduleToBind = std::static_pointer_cast<T>(m_modules[moduleName]);
				moduleToBind->OnCreateVariables();
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

	std::shared_ptr<Command> ManagerComponent::CreateCommand(const std::string& commandName, Command* command)
	{
		if (command)
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
				else
				{
					Console.Warning("(CreateCommand) Warning: Duplicate command name detected for \"" + commandName + "\"!");
				}
			}
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

	std::shared_ptr<Setting> ManagerComponent::CreateSetting(const std::string& settingName, Setting* setting)
	{
		if (setting)
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
				else
				{
					Console.Warning("(CreateSetting) Warning: Duplicate setting name detected for \"" + settingName + "\"!");
				}
			}
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
		// Assigning the "STATES_CasualMatch" and "STATES_RankedMatch" flags, so this module will only be able to be used in casual or ranked games.
		CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);

		CreateCommand("reset_setting", new Command(VariableIds::MANAGER_RESET_SETTING, "Reset a setting to its default/original value."))->BindStringCallback([&](const std::string& arguments) { ResetSetting(arguments); });
		CreateCommand("unreal_command", new Command(VariableIds::MANAGER_UNREAL_COMMAND, "Execute a Unreal Engine 3 command with the given arguments."))->BindStringCallback([&](const std::string& arguments) { UnrealCommand(arguments); });

		for (auto& modulePair : m_modules)
		{
			if (modulePair.second)
			{
				modulePair.second->OnSettingChanged(); // Load all your variables with their default values you assigned when creating their setting/
			}
		}

		Console.Write(GetNameFormatted() + std::to_string(m_commands.size()) + " Command(s) Initialized!");
		Console.Write(GetNameFormatted() + std::to_string(m_settings.size()) + " Setting(s) Initialized!");
	}

	class ManagerComponent Manager;
}