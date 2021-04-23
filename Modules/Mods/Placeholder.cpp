#include "Placeholder.hpp"
#include "../Components/Includes.hpp"

PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, const uint32_t& states) : Module(name, description, states, ModuleTypes::TYPE_PLACEHOLDER)
{
	SomeBoolSetting = false;

	Console.Write(GetNameFormatted() + "Initialized!");
}

PlaceholderModule::~PlaceholderModule() { }

// This is where you would load your custom settings for this module, whether it be from an external file or just defined here on compilation.
void PlaceholderModule::LoadSettings()
{
	SetInitialized(); // Make sure this is always placed after you load your settings.
}

void PlaceholderModule::SetSomeBoolEnabled(const class BoolSetting& someBoolEnabled)
{
	if (someBoolEnabled.GetType() == SettingTypes::TYPE_BOOL)
	{
		SomeBoolSetting = someBoolEnabled.GetValue();
	}
}

void PlaceholderModule::DoAThing()
{
	if (IsInitialized() && IsAllowed())
	{
		if (SomeBoolSetting)
		{
			Console.Success("Wow! Did a thing!");
		}
	}
}