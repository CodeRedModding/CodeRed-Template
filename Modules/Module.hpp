#pragma once
#include "../pch.hpp"

// Underlying module types.
enum class ModuleTypes : uint8_t
{
	TYPE_PLACEHOLDER = 0,
};

// Module Base Class
// Base class for all modules, 
class Module
{
private:
	std::string Name;				// Modules name.
	std::string FormattedName;		// Modules formatted name.
	std::string Description;		// Modules description.
	ModuleTypes Type;				// Modules underlying type.
	uint32_t AllowedStates;			// Game states that this module can be used in.
	bool Initialized;				// If the modules settings have been loaded, and it's safe to use its functions.

public:
	Module(const std::string& name, const std::string& description, const uint32_t& states, const ModuleTypes& type);
	~Module();

public:
	std::string GetName() const;
	std::string GetNameFormatted() const;
	std::string GetDescription() const;
	ModuleTypes GetType() const;
	bool IsAllowed() const;
	bool IsInitialized() const;
	void SetInitialized();
};