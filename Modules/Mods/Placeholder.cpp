#include "Placeholder.hpp"
#include "../Components/Includes.hpp"

PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, uint32_t states) : Module(name, description, states)
{
	PlaceholderEnabled = false;
	SomeValue = 0;
}

PlaceholderModule::~PlaceholderModule() {}

void PlaceholderModule::UpdateSettings()
{
	SetInitialized(false);
	PlaceholderEnabled = Manager.GetSetting(VariableIds::PLACEHOLDER_ENABLED)->GetBoolValue();
	SomeValue = Manager.GetSetting(VariableIds::PLACEHOLDER_SOME_VALUE)->GetIntValue();
	SetInitialized(true);
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