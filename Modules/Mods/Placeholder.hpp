#pragma once
#include "../Module.hpp"

// This is an example module.
class PlaceholderModule : public Module
{
private:
	bool PlaceholderEnabled;
	int32_t SomeValue;

public:
	PlaceholderModule(const std::string& name, const std::string& description, uint32_t states);
	~PlaceholderModule() override;

public:
	void UpdateSettings();
	void DoAThing();
};