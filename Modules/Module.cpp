#include "Module.hpp"
#include "../Components/Includes.hpp"

Module::Module(const std::string& name, const std::string& description, const uint32_t& states, const ModuleTypes& type)
{
	Name = name;
	FormattedName = std::string("[") + Name + std::string("] ");
	Description = description;
	Type = type;
	AllowedStates = states;
	Initialized = false;
}

Module::~Module() { }

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

ModuleTypes Module::GetType() const
{
	return Type;
}

bool Module::IsAllowed() const
{
	if (AllowedStates & States::All
		|| AllowedStates & GameState.GetStateId())
	{
		return true;
	}

	return false;
}

bool Module::IsInitialized() const
{
	return Initialized;
}

void Module::SetInitialized()
{
	Initialized = true;
	Console.Success(GetNameFormatted() + "Initialized!");
}