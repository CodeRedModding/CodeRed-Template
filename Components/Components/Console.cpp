#include "Console.hpp"

#define CONSOLE_WINDOW 	// If you want to open a console window to display text.
#define WRITE_TO_FILE 	// If you want to write all console text to a local text file.
#define STANDARD_OUTPUT // If you want all console text to also write to "std::cout".

ConsoleComponent::ConsoleComponent() : Component("Console", "Displays and manages the standard out stream.") { OnCreate(); }

ConsoleComponent::~ConsoleComponent() { OnDestroy(); }

void ConsoleComponent::OnCreate()
{
	m_outputHandle = nullptr;
	m_outputFile = nullptr;
	m_24hourClock = false;
}

void ConsoleComponent::OnDestroy()
{
#ifdef WRITE_TO_FILE
	if (m_logFile.is_open())
	{
		m_logFile.flush();
		m_logFile.close();
	}
#endif

#ifdef CONSOLE_WINDOW
	m_outputHandle = nullptr;

	if (m_outputFile)
	{
		FreeConsole();
		m_outputFile = nullptr;
	}
#endif
}

void ConsoleComponent::SetClockStyle(bool bIs24Hours)
{
	m_24hourClock = bIs24Hours;
}

std::string ConsoleComponent::CreateTimestamp(bool bBrackets)
{
	return Time::Timestamp::Create(m_24hourClock).FormatStandard(bBrackets, false);
}

std::string ConsoleComponent::CreateTimestampISO8601(bool bBrackets)
{
	return Time::Timestamp::Create(m_24hourClock).FormatISO8601(bBrackets);
}

void ConsoleComponent::Write(const std::string& text, TextColors textColor)
{
	std::string str = (CreateTimestamp() + " " + text);
	
#ifdef WRITE_TO_FILE
	if (m_logFile.is_open())
	{
		m_logFile << str << std::endl;
	}
#endif

#ifdef STANDARD_OUTPUT
	std::cout << str << std::endl;
#endif
}

void ConsoleComponent::Warning(const std::string& text)
{
	Write(text, TextColors::Yellow);
}

void ConsoleComponent::Error(const std::string& text)
{
	Write(text, TextColors::Red);
}

void ConsoleComponent::Success(const std::string& text)
{
	Write(text, TextColors::Green);
}

void ConsoleComponent::Notify(const std::string& text)
{
	Write(text, TextColors::Blue);
}

void ConsoleComponent::WriteDemo()
{
	for (uint16_t i = 0; i < static_cast<uint16_t>(TextColors::END); i++)
	{
		Write("The Quick Brown Fox Jumps Over The Lazy Dog `~1234567890!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?", static_cast<TextColors>(i));
	}
}

void ConsoleComponent::Initialize(const std::filesystem::path& directory, const std::string& fileName)
{
#ifdef CONSOLE_WINDOW
	AllocConsole();
	freopen_s(&m_outputFile, "CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#ifdef WRITE_TO_FILE
	if (std::filesystem::exists(directory))
	{
		m_logFile.open(directory / fileName);
	}
#endif

	Write(GetNameFormatted() + "Initialized!");
}

class ConsoleComponent Console{};