#pragma once
#include "../Module.hpp"

// This is an example module.
class PlaceholderModule : public Module
{
private:
	bool PlaceholderEnabled;

public:
	PlaceholderModule(const std::string& name, const std::string& description, const uint32_t& states);
	~PlaceholderModule();

public: // Functions related to user settings.
	void LoadSettings();
	void SetPlaceholderEnabled(const class BoolSetting& placeholderEnabled);

public: // Functions related to the module.
	void DoAThing();
};