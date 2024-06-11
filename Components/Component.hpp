#pragma once
#include "../pch.hpp"
#include "../Extensions/Includes.hpp"

// Base class for all components, stores its name, formatted name, and description.
class Component
{
private:
	std::string m_name;
	std::string m_description;

public:
	Component(const std::string& name, const std::string& description);
	Component(const Component& component);
	virtual ~Component();

public: // These can be manually called out side of the classes constructor/deconstructor.
	virtual void OnCreate();
	virtual void OnDestroy();

public:
	std::string GetName() const;
	std::string GetNameFormatted() const;
	std::string GetDescription() const;

public:
	Component& operator=(const Component& component);
};