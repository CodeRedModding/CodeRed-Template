#include "Component.hpp"

Component::Component(const std::string& name, const std::string& description) : m_name(name), m_description(description) { OnCreate(); }

Component::Component(const Component& component) : m_name(component.m_name), m_description(component.m_description) { OnCreate(); }

Component::~Component() { OnDestroy(); }

void Component::OnCreate() {}

void Component::OnDestroy() {}

std::string Component::GetName() const
{
	return m_name;
}

std::string Component::GetNameFormatted() const
{
	return ("[" + GetName() + " Component] ");
}

std::string Component::GetDescription() const
{
	return m_description;
}

Component& Component::operator=(const Component& component)
{
	m_name = component.m_name;
	m_description = component.m_description;
	return *this;
}