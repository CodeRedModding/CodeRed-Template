#include "Component.hpp"

namespace CodeRed
{
	Component::Component(const std::string& name, const std::string& description) : m_name(name), m_description(description), m_initialized(false) { OnCreate(); }

	Component::Component(const Component& component) : m_name(component.m_name), m_description(component.m_description), m_initialized(component.m_initialized) { OnCreate(); }

	Component::~Component() { OnDestroy(); }

	void Component::OnCreate() {}

	void Component::OnDestroy() {}

	bool Component::Initialize()
	{
		if (!IsInitialized())
		{
			// Do component specific Initialization here.
			SetInitialized(true);
		}

		return IsInitialized();
	}

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

	bool Component::IsInitialized() const
	{
		return m_initialized;
	}

	void Component::SetInitialized(bool bInitialized)
	{
		m_initialized = bInitialized;
	}

	Component& Component::operator=(const Component& component)
	{
		m_name = component.m_name;
		m_description = component.m_description;
		m_initialized = component.m_initialized;
		return *this;
	}
}