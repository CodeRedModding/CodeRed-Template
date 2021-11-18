#include "Component.hpp"

Component::Component(const std::string& name, const std::string& description) : Name(name), FormattedName("[" + Name + " Component] "), Description(description)
{
	OnCreate();
}

Component::~Component()
{
	OnDestroy();
}

void Component::OnCreate() {}

void Component::OnDestroy() {}

std::string Component::GetName() const
{
	return Name;
}

std::string Component::GetNameFormatted() const
{
	return FormattedName;
}

std::string Component::GetDescription() const
{
	return Description;
}