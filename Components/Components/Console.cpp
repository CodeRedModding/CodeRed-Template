#include "Console.hpp"

ConsoleComponent::ConsoleComponent() : Component("Console", "Displays and manages the standard out stream.") { OnCreate(); }

ConsoleComponent::~ConsoleComponent() { OnDestroy(); }

void ConsoleComponent::OnCreate()
{
	OutputHandle = nullptr;
	OutputFile = nullptr;
	WriteToLog = true;
	WriteTimestamp = false;
}

void ConsoleComponent::OnDestroy()
{
	LogFile.close();

	if (OutputFile)
	{
		FreeConsole();
	}
}

void ConsoleComponent::Write(const std::string& text, TextColors textColor)
{
	WriteInternal(text, textColor);
}

void ConsoleComponent::WriteInternal(std::string text, TextColors textColor)
{
	SetTextColor(textColor);

	if (WriteTimestamp)
	{
		text = GetTimestamp(true) + text;
	}

	std::cout << text << std::endl;

	if (WriteToLog)
	{
		LogFile << text;
		LogFile.flush();
	}

	SetTextColor(TextColors::BrightWhite);
}

void ConsoleComponent::Warning(const std::string& text)
{
	WriteInternal(text, TextColors::Yellow);
}

void ConsoleComponent::Error(const std::string& text)
{
	WriteInternal(text, TextColors::Red);
}

void ConsoleComponent::Success(const std::string& text)
{
	WriteInternal(text, TextColors::LightGreen);
}

void ConsoleComponent::Notify(const std::string& text)
{
	WriteInternal(text, TextColors::LightBlue);
}

void ConsoleComponent::Initialize(const std::filesystem::path& directory, const std::string& fileName)
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
		LogFile.open(directory / fileName);
	}

	Write(GetNameFormatted() + "Initialized!");
}

void ConsoleComponent::ToggleLogging(bool bWriteToLog)
{
	WriteToLog = bWriteToLog;
}

void ConsoleComponent::ToggleTimestamp(bool bWriteTimestamp)
{
	WriteTimestamp = bWriteTimestamp;
}

void ConsoleComponent::SetTextColor(TextColors textColor)
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

	std::ostringstream timeStream;
	timeStream << "[";
	timeStream << std::setfill('0') << std::setw(2) << std::right << std::to_string(local.tm_hour);
	timeStream << ":";
	timeStream << std::setfill('0') << std::setw(2) << std::right << std::to_string(local.tm_min);
	timeStream << ":";
	timeStream << std::setfill('0') << std::setw(2) << std::right << std::to_string(local.tm_sec);
	timeStream << "]" << (bWithSpace ? " " : "");

	return timeStream.str();
}

class ConsoleComponent Console{};