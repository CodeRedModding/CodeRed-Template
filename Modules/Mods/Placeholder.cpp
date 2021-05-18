#include "Placeholder.hpp"
#include "../Components/Includes.hpp"

PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, const uint32_t& states) : Module(name, description, states, ModuleTypes::TYPE_PLACEHOLDER)
{
	PlaceholderEnabled = false;
}

PlaceholderModule::~PlaceholderModule() { }

// This is where you would load your custom settings for this module, whether it be from an external file or just defined here on compilation.
void PlaceholderModule::LoadSettings()
{
	SetInitialized(); // Make sure this is always placed after you load your settings.
}

void PlaceholderModule::SetPlaceholderEnabled(const class BoolSetting& placeholderEnabled)
{
	if (placeholderEnabled.GetType() == SettingTypes::TYPE_BOOL)
	{
		PlaceholderEnabled = placeholderEnabled.GetValue();
	}
}

void PlaceholderModule::DoAThing()
{
	if (IsInitialized() && IsAllowed())
	{
		if (PlaceholderEnabled)
		{
			Console.Success("Wow! Did a thing!");
		}
	}
}