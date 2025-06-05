#include "Placeholder.hpp"
#include "../../Components/Includes.hpp"

namespace CodeRed
{
	PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, uint32_t states) : Module(name, description, states) { OnCreate(); }

	PlaceholderModule::~PlaceholderModule() { OnDestroy(); }

	void PlaceholderModule::OnCreate()
	{
		m_placeholder = false;
		m_someValue = 0;
	}

	void PlaceholderModule::OnDestroy()
	{
		SetInitialized(false);
	}

	void PlaceholderModule::OnCreateVariables()
	{
		// When someone uses the command "placeholder_do_thing", this will trigger the function "DoAThing" in "PlaceholderModule".
		Variables.CreateCommand("placeholder_do_thing", new Command(VariableIds::PLACEHOLDER_DO_THING, "Calls the \"DoAThing\" function in the placeholder module."))->BindCallback([&]() { Manager.PlaceholderMod->OnSettingChanged(); });

		// When changes the setting "placeholder_can_do_thing true", we automatically callback to "PlaceholderModule" and tell it to update its settings stored in that class.
		Variables.CreateSetting("placeholder_can_do_thing", new Setting(VariableIds::PLACEHOLDER_ENABLED, SettingTypes::Bool, "Enable disable a boolean in the placeholder module.", "false", true))->BindCallback([&]() { Manager.PlaceholderMod->OnSettingChanged(); });

		// Integer setting that has a minimum value of "0" and a maximum value of "100".
		Variables.CreateSetting("placeholder_some_value", new Setting(VariableIds::PLACEHOLDER_SOME_VALUE, SettingTypes::Int32, "Some random integer value with a custom range.", "0", true))->SetInt32Range(0, 100)->BindCallback([&]() { Manager.PlaceholderMod->OnSettingChanged(); });
	}

	void PlaceholderModule::OnSettingChanged()
	{
		SetInitialized(false);
		m_placeholder = Variables.GetSetting(VariableIds::PLACEHOLDER_ENABLED)->GetBoolValue();
		m_someValue = Variables.GetSetting(VariableIds::PLACEHOLDER_SOME_VALUE)->GetInt32Value();
		SetInitialized(true);
	}

	void PlaceholderModule::OnCanvasDraw(class UCanvas* unrealCanvas)
	{
		if (IsInitialized() && IsAllowed())
		{
			if (unrealCanvas)
			{
				// Draw with the canvas here!
			}
		}
	}

	void PlaceholderModule::DoAThing()
	{
		if (IsInitialized() && IsAllowed())
		{
			if (m_placeholder)
			{
				Console.Success("(DoAThing) Wow! Placeholder is set to true!");
			}
			else
			{
				Console.Error("(DoAThing) Wow! Placeholder is set to false!");
			}
		}
	}
}