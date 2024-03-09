#include "Console.hpp"

ConsoleComponent::ConsoleComponent() : Component("Console", "Displays and manages the standard out stream.") { OnCreate(); }

ConsoleComponent::~ConsoleComponent() { OnDestroy(); }

void ConsoleComponent::OnCreate()
{
	m_outputHandle = nullptr;
	m_outputFile = nullptr;
}

void ConsoleComponent::OnDestroy()
{
	m_logFile.close();

	if (m_outputFile)
	{
		FreeConsole();
		m_outputFile = nullptr;
	}
}

void ConsoleComponent::Write(const std::string& text, TextColors textColor)
{
	WriteInternal(text, textColor);
}

void ConsoleComponent::WriteInternal(std::string text, TextColors textColor)
{
	SetTextColor(textColor);
	text = (GetTimestamp(true) + text);

	std::cout << text << std::endl;

	if (m_logFile.is_open())
	{
		m_logFile << text;
		m_logFile.flush();
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
	m_outputHandle = nullptr;

	// If you want to open the standard out stream and show the window, then grab its handle so we can set the text color.

	AllocConsole();
	freopen_s(&m_outputFile, "CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// If you want to write to a text file.

	if (std::filesystem::exists(directory))
	{
		m_logFile.open(directory / fileName);
	}

	Write(GetNameFormatted() + "Initialized!");
}

void ConsoleComponent::SetTextColor(TextColors textColor)
{
	if (m_outputHandle)
	{
		SetConsoleTextAttribute(m_outputHandle, static_cast<uint16_t>(textColor));
	}
}

std::string ConsoleComponent::GetTimestamp(bool bWithSpace)
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