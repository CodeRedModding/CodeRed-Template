#pragma once
#include "../pch.hpp"

// Base class for all components.
// Stores it's name, formatted name, and description for retrieving later on.

class Component
{
private:
	std::string Name;
	std::string FormattedName;
	std::string Description;

public:
	Component(const std::string& name, const std::string& description);
	~Component();

public:
	std::string GetName() const;
	std::string GetNameFormatted() const;
	std::string GetDescription() const;
};