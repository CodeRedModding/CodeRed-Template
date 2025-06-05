#include "Console.hpp"

namespace CodeRed
{
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

	bool ConsoleComponent::Initialize()
	{
		if (!IsInitialized())
		{
#ifdef CONSOLE_WINDOW
			AllocConsole();
			freopen_s(&m_outputFile, "CONOUT$", "w", stdout);
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#ifdef WRITE_TO_FILE
			std::filesystem::path directory = std::filesystem::current_path();

			if (std::filesystem::exists(directory))
			{
				m_logFile.open(directory / "CodeRed.log");
			}
			else
			{
				Error(GetNameFormatted() + "Directory does not exist, cannot create log file!");
				return false;
			}
#endif

			Success(GetNameFormatted() + "Initialized!");
			SetInitialized(true);
		}

		return IsInitialized();
	}

	void ConsoleComponent::SetClockStyle(bool bIs24Hours)
	{
		m_24hourClock = bIs24Hours;
	}

	std::string ConsoleComponent::CreateTimestamp(bool bBrackets) const
	{
		return Time::Timestamp::Create(m_24hourClock).FormatClock(bBrackets, false);
	}

	std::string ConsoleComponent::CreateTimestampISO8601() const
	{
		return Time::Timestamp::Create(m_24hourClock).FormatISO8601();
	}

	std::string ConsoleComponent::CreateTimestampUTCSimple() const
	{
		return Time::Timestamp::Create(m_24hourClock).FormatUTCSimple();
	}

	std::string ConsoleComponent::CreateTimestampUTCExtended() const
	{
		return Time::Timestamp::Create(m_24hourClock).FormatUTCExtended();
	}

	void ConsoleComponent::ColorConsole(TextColors textColor)
	{
		if (m_outputHandle)
		{
#ifdef STANDARD_OUTPUT
			SetConsoleTextAttribute(m_outputHandle, static_cast<uint16_t>(textColor));
#endif
		}
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
		ColorConsole(textColor);
		std::cout << str << std::endl;
		ColorConsole(TextColors::White);
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

	class ConsoleComponent Console;
}