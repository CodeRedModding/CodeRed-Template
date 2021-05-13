#include "Console.hpp"

ConsoleComponent::ConsoleComponent() : Component("Console", "Displays and manages the standard out stream.")
{
	OutputHandle = nullptr;
	OutputFile = nullptr;
	WriteToLog = false;
	WriteTimestamp = false;

	// Open the standard out stream and show the window, then grab its handle so we can set the text color.

	AllocConsole();
	freopen_s(&OutputFile, "CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleComponent::ConsoleComponent(const std::string& directory, const std::string& fileName) : Component("Console", "Displays and manages the standard out stream.")
{
	OutputHandle = nullptr;
	OutputFile = nullptr;
	WriteToLog = true;
	WriteTimestamp = true;

	// Open the standard out stream and show the window, then grab its handle so we can set the text color.

	AllocConsole();
	freopen_s(&OutputFile, "CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (WriteToLog)
	{
		LogFile.Create(directory, fileName);
	}
}

ConsoleComponent::~ConsoleComponent()
{
	LogFile.Close();
	FreeConsole();
}

void ConsoleComponent::Write(std::string text)
{
	Write(text, TextColors::BrightWhite);
}

void ConsoleComponent::Write(std::string text, const TextColors textColor)
{
	SetTextColor(textColor);

	if (WriteTimestamp)
	{
		text = GetTimestamp(true) + text;
	}

	std::cout << text << std::endl;

	if (WriteToLog)
	{
		LogFile.WriteLine(text);
		LogFile.Flush();
	}

	SetTextColor(TextColors::BrightWhite);
}

void ConsoleComponent::Warning(std::string text)
{
	Write(text, TextColors::Yellow);
}

void ConsoleComponent::Error(std::string text)
{
	Write(text, TextColors::Red);
}

void ConsoleComponent::Success(std::string text)
{
	Write(text, TextColors::LightGreen);
}

void ConsoleComponent::Notify(std::string text)
{
	Write(text, TextColors::LightBlue);
}

void ConsoleComponent::Initialize(const std::string& directory, const std::string& fileName)
{
	OutputHandle = nullptr;
	OutputFile = nullptr;
	WriteToLog = true;
	WriteTimestamp = true;

	// Open the standard out stream and show the window, then grab its handle so we can set the text color.

	AllocConsole();
	freopen_s(&OutputFile, "CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (WriteToLog)
	{
		LogFile.Create(directory, fileName);
	}

	Write(GetNameFormatted() + "Initialized!");
}

void ConsoleComponent::ToggleLogging(const bool& bWriteToLog)
{
	WriteToLog = bWriteToLog;
}

void ConsoleComponent::ToggleTimestamp(const bool& bWriteTimestamp)
{
	WriteTimestamp = bWriteTimestamp;
}

void ConsoleComponent::SetTextColor(const TextColors textColor)
{
	if (OutputHandle)
	{
		SetConsoleTextAttribute(OutputHandle, static_cast<uint16_t>(textColor));
	}
}

std::string ConsoleComponent::GetTimestamp(const bool& bWithSpace)
{
	std::tm local;
	std::time_t time = std::time(nullptr);
	localtime_s(&local, &time);

	std::string timestamp;
	timestamp = "[";

	if (local.tm_hour < 10) { timestamp += "0" + std::to_string(local.tm_hour); }
	else { timestamp += std::to_string(local.tm_hour); }

	timestamp += ":";

	if (local.tm_min < 10) { timestamp += "0" + std::to_string(local.tm_min); }
	else { timestamp += std::to_string(local.tm_min); }

	timestamp += ":";

	if (local.tm_sec < 10) { timestamp += "0" + std::to_string(local.tm_sec); }
	else { timestamp += std::to_string(local.tm_sec); }

	timestamp += "]";

	if (bWithSpace)
	{
		timestamp += " ";
	}

	return timestamp;
}

class ConsoleComponent Console;