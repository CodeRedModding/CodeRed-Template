#include "Placeholder.hpp"
#include "../Components/Includes.hpp"

PlaceholderModule::PlaceholderModule(const std::string& name, const std::string& description, uint32_t states) : Module(name, description, states)
{
	m_placeholder = false;
	m_someValue = 0;
}

PlaceholderModule::~PlaceholderModule() {}

void PlaceholderModule::UpdateSettings()
{
	SetInitialized(false);
	m_placeholder = Manager.GetSetting(VariableIds::PLACEHOLDER_ENABLED)->GetBoolValue();
	m_someValue = Manager.GetSetting(VariableIds::PLACEHOLDER_SOME_VALUE)->GetInt32Value();
	SetInitialized(true);
}

void PlaceholderModule::DoAThing()
{
	if (IsInitialized() && IsAllowed())
	{
		if (m_placeholder)
		{
			Console.Success("Wow! Did a thing!");
		}
	}
}