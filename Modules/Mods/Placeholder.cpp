#include "Placeholder.hpp"
#include "../Components/Includes.hpp"

PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, uint32_t states) : Module(name, description, states)
{
	PlaceholderEnabled = false;
}

PlaceholderModule::~PlaceholderModule() { }

// This is where you would load your custom settings for this module, for example from an external file.
void PlaceholderModule::LoadSettings()
{
	SetInitialized(); // Make sure this is always placed after you load your settings.
}

void PlaceholderModule::SetPlaceholderEnabled(const class Setting* setting)
{
	if (setting->GetType() == SettingTypes::TYPE_BOOL)
	{
		PlaceholderEnabled = setting->GetBoolValue();
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