#pragma once
#include "../pch.hpp"
#include "../Extensions/Includes.hpp"

namespace CodeRed
{
	// Base class for all components, stores its name and description.
	class Component
	{
	private:
		std::string m_name;
		std::string m_description;
		bool m_initialized;

	public:
		Component() = delete;
		Component(const std::string& name, const std::string& description);
		Component(const Component& component);
		virtual ~Component();

	public: // These can be manually called out side of the classes constructor and deconstructor.
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual bool Initialize(); // Do component specific Initialization here.

	public:
		std::string GetName() const;
		std::string GetNameFormatted() const;
		std::string GetDescription() const;
		bool IsInitialized() const;
		void SetInitialized(bool bInitialized);

	public:
		Component& operator=(const Component& component);
	};
}