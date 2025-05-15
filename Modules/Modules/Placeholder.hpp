#pragma once
#include "../Module.hpp"

namespace CodeRed
{
	// This is an example module.
	class PlaceholderModule : public Module
	{
	private:
		bool m_placeholder;
		int32_t m_someValue;

	public:
		PlaceholderModule() = delete;
		PlaceholderModule(const std::string& name, const std::string& description, uint32_t states);
		~PlaceholderModule() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;
		void OnCreateVariables() override;
		void OnSettingChanged() override;
		void OnCanvasDraw(class UCanvas* unrealCanvas) override;

	public:
		void DoAThing();
	};
}