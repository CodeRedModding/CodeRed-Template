#pragma once
#include "../pch.hpp"

static constexpr uint64_t SECONDS_IN_MINUTE = 60;
static constexpr uint64_t SECONDS_IN_HOUR = 3600;
static constexpr uint64_t SECONDS_IN_DAY = 86400;
static constexpr uint64_t SECONDS_IN_MONTH = 2629743;
static constexpr uint64_t SECONDS_IN_YEAR = 31556926;

namespace Time
{
	class Time
	{
	public:
		uint32_t Second;		// Seconds after the minute - [0, 60] including leap second
		uint32_t Minute;		// Minutes after the hour - [0, 59]
		uint32_t Hour;			// Hours since midnight - [0, 23]
		uint32_t Day;			// Day of the month - [1, 31]
		uint32_t Month;			// Months since January - [0, 11]
		uint32_t Year;			// Years since 1900
		uint32_t WeekDay;		// Days since Sunday - [0, 6]
		uint32_t YearDay;		// Days since January 1 - [0, 365]
		bool bDaylightSavings;

	public:
		Time();
		Time(const std::tm& tm);
		Time(const Time& time);
		~Time();

	public:
		Time& FromTM(const std::tm& tm);
		Time& Create();

	public:
		Time& operator=(const std::tm& tm);
		Time& operator=(const Time& time);
	};

	class Timestamp
	{
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
		std::string FormatStandard(bool bBrackets, bool bSkipSeconds);
		std::string FormatISO8601(bool bBrackets);

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