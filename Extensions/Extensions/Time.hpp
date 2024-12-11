#pragma once
#include "../pch.hpp"

namespace CodeRed::Time
{
	static constexpr uint64_t SECONDS_IN_MINUTE = 60;
	static constexpr uint64_t SECONDS_IN_HOUR = 3600;
	static constexpr uint64_t SECONDS_IN_DAY = 86400;
	static constexpr uint64_t SECONDS_IN_MONTH = 2629743;
	static constexpr uint64_t SECONDS_IN_YEAR = 31556926;

	class Time
	{
	public:
		uint32_t Second;		// Seconds after the minute, including leap second (0-60).
		uint32_t Minute;		// Minutes after the hour (0-59).
		uint32_t Hour;			// Hours since midnight (0-23).
		uint32_t Day;			// Day of the month (1-31).
		uint32_t Month;			// Months since January (0-11).
		uint32_t Year;			// Years since 1900.
		uint32_t WeekDay;		// Days since Sunday (0-6).
		uint32_t YearDay;		// Days since January 1st (0-365).
		bool bDaylightSavings;

	public:
		Time();
		explicit Time(uint64_t epochTimestamp);
		explicit Time(const std::tm& tm);
		Time(const Time& time);
		~Time();

	public:
		Time& FromEpoch(uint64_t epochTimestamp);
		Time& FromTM(const std::tm& tm);
		Time& Create();

	public:
		Time& operator=(const std::tm& tm);
		Time& operator=(const Time& time);
	};

	class Timestamp
	{
	private:
		static std::vector<std::string> m_days;
		static std::vector<std::string> m_months;

	public:
		Time tTime;
		bool b24Hours;

	public:
		Timestamp();
		Timestamp(const Time& time, bool bIs24Hours);
		Timestamp(const Timestamp& timestamp);
		~Timestamp();

	public:
		static Timestamp Create(bool bIs24Hours);
		std::string FormatClock(bool bBrackets, bool bSkipSeconds) const; // 4:31:42 PM
		std::string FormatISO8601() const; // 2024-10-29T22:41:46+00:00
		std::string FormatUTCSimple() const; // 10/29/2024 10:41pm
		std::string FormatUTCExtended() const; // Tue Oct 29 22:46:59 2024 UTC

	public:
		Timestamp& operator=(const Timestamp& timestamp);
	};

	std::chrono::system_clock::duration EpochDuration();
	uint64_t EpochHours();
	uint64_t EpochMinutes();
	uint64_t EpochSeconds();
	uint64_t EpochMilli();
	uint64_t EpochNano();
	int32_t UTCOffset();
}