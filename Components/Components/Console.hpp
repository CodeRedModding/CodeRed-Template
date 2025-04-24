#pragma once
#include "../Component.hpp"

namespace CodeRed
{
	// Manages the standard out stream, as well as logging to an offline file.
	class ConsoleComponent : public Component
	{
	private:
		std::ofstream m_logFile;
		HANDLE m_outputHandle;
		FILE* m_outputFile;
		bool m_24hourClock; // If you want timestamps to be in 24 hour format or 12 hour.

	public:
		ConsoleComponent();
		~ConsoleComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;

	public: // Utils
		void SetClockStyle(bool bIs24Hours);
		std::string CreateTimestamp(bool bBrackets = true) const; // 4:31:42 PM
		std::string CreateTimestampISO8601() const; // 2024-10-29T22:41:46+00:00
		std::string CreateTimestampUTCSimple() const; // 10/29/2024 10:41pm
		std::string CreateTimestampUTCExtended() const; // Tue Oct 29 22:46:59 2024 UTC

	public:
		void ColorConsole(TextColors textColor);
		void Write(const std::string& text, TextColors textColor = TextColors::White);
		void Warning(const std::string& text);
		void Error(const std::string& text);
		void Success(const std::string& text);
		void Notify(const std::string& text);
		void WriteDemo();

	public:
		void Initialize(const std::filesystem::path& directory, const std::string& fileName);
	};

	extern class ConsoleComponent Console;
}