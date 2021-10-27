#pragma once
#include "../pch.hpp"
#include "../Extensions/Includes.hpp"

// Base class for all in-game modules.
class Module
{
private:
	std::string Name;				// Modules name.
	std::string FormattedName;		// Modules formatted name.
	std::string Description;		// Modules description.
	uint32_t AllowedStates;			// Game states that this module can be used in.
	bool Initialized;				// If the modules settings have been loaded, and it's safe to use its functions.

public:
	Module(const std::string& name, const std::string& description, uint32_t states);
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