#include "Module.hpp"
#include "../Components/Includes.hpp"

Module::Module(const std::string& name, const std::string& description, uint32_t states) : m_name(name), m_description(description), m_allowedStates(states), m_initialized(false) {}

Module::Module(const Module& module) : m_name(module.m_name), m_description(module.m_description), m_allowedStates(module.m_allowedStates), m_initialized(module.m_initialized) {}

Module::~Module() {}

std::string Module::GetName() const
{
	return m_name;
}

std::string Module::GetNameFormatted() const
{
	return ("[" + GetName() + "] ");
}

std::string Module::GetDescription() const
{
	return m_description;
}

uint32_t Module::GetAllowedStates() const
{
	return m_allowedStates;
}

bool Module::IsAllowed() const
{
	return (GetAllowedStates() & States::STATES_All) || (GetAllowedStates() & GameState.GetRawState());
}

bool Module::IsInitialized() const
{
	return m_initialized;
}

void Module::SetInitialized(bool bInitialized)
{
	m_initialized = bInitialized;
}

Module& Module::operator=(const Module& module)
{
	m_name = module.m_name;
	m_description = module.m_description;
	m_allowedStates = module.m_allowedStates;
	m_initialized = module.m_initialized;
	return *this;
}