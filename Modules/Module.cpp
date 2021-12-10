#include "Module.hpp"
#include "../Components/Includes.hpp"

Module::Module(const std::string& name, const std::string& description, uint32_t states) : Name(name), FormattedName("[" + Name + "] "), Description(description), AllowedStates(states), Initialized(false) {}

Module::~Module() {}

std::string Module::GetName() const
{
	return Name;
}

std::string Module::GetNameFormatted() const
{
	return FormattedName;
}

std::string Module::GetDescription() const
{
	return Description;
}

uint32_t Module::GetAllowedStates() const
{
	return AllowedStates;
}

bool Module::IsAllowed() const
{
	if ((AllowedStates & States::STATES_All) || (AllowedStates & GameState.GetRawState()))
	{
		return true;
	}

	return false;
}

bool Module::IsInitialized() const
{
	return Initialized;
}

void Module::SetInitialized(bool bInitialized)
{
	Initialized = bInitialized;
}