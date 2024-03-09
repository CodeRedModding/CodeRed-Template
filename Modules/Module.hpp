#pragma once
#include "../pch.hpp"
#include "../Extensions/Includes.hpp"

// Base class for all in-game modules.
class Module
{
private:
	std::string m_name;				// Modules name.
	std::string m_description;		// Modules description.
	uint32_t m_allowedStates;		// Game states that this module can be used in.
	bool m_initialized;				// If the modules settings have been loaded, and it's safe to use its functions.

public:
	Module(const std::string& name, const std::string& description, uint32_t states);
	Module(const Module& module);
	virtual ~Module();

public:
	std::string GetName() const;
	std::string GetNameFormatted() const;
	std::string GetDescription() const;
	uint32_t GetAllowedStates() const;
	bool IsAllowed() const;
	bool IsInitialized() const;
	void SetInitialized(bool bInitialized);
};