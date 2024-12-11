#include "Time.hpp"
#include "Formatting.hpp"

namespace CodeRed::Time
{
	Time::Time() :
		Second(0),
		Minute(0),
		Hour(0),
		Day(0),
		Month(0),
		Year(0),
		WeekDay(0),
		YearDay(0),
		bDaylightSavings(false)
	{
	
	}

	Time::Time(uint64_t epochTimestamp)
	{
		FromEpoch(epochTimestamp);
	}

	Time::Time(const std::tm& tm)
	{
		FromTM(tm);
	}

	Time::Time(const Time& time) :
		Second(time.Second),
		Minute(time.Minute),
		Hour(time.Hour),
		Day(time.Day),
		Month(time.Month),
		Year(time.Year),
		WeekDay(time.WeekDay),
		YearDay(time.YearDay),
		bDaylightSavings(time.bDaylightSavings)
	{
	
	}

	Time::~Time() {}

	Time& Time::FromEpoch(uint64_t epochTimestamp)
	{
		std::tm tm;
		ZeroMemory(&tm, sizeof(tm));
		localtime_s(&tm, reinterpret_cast<std::time_t*>(&epochTimestamp));
		return FromTM(tm);
	}


	Time& Time::FromTM(const std::tm& tm)
	{
		Second = tm.tm_sec;
		Minute = tm.tm_min;
		Hour = tm.tm_hour;
		Day = tm.tm_mday;
		Month = tm.tm_mon;
		Year = tm.tm_year;
		WeekDay = tm.tm_wday;
		YearDay = tm.tm_year;
		bDaylightSavings = (tm.tm_isdst > 0);
		return *this;
	}

	Time& Time::Create()
	{
		std::tm local;
		ZeroMemory(&local, sizeof(local));
		std::time_t time = std::time(nullptr);
		localtime_s(&local, &time);
		return FromTM(local);
	}

	Time& Time::operator=(const std::tm& tm)
	{
		return FromTM(tm);
	}

	Time& Time::operator=(const Time& time)
	{
		Second = time.Second;
		Minute = time.Minute;
		Hour = time.Hour;
		Day = time.Day;
		Month = time.Month;
		Year = time.Year;
		WeekDay = time.WeekDay;
		YearDay = time.YearDay;
		bDaylightSavings = time.bDaylightSavings;
		return *this;
	}

	std::vector<std::string> Timestamp::m_days = {
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
		"Sun"
	};

	std::vector<std::string> Timestamp::m_months = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};

	Timestamp::Timestamp() : b24Hours(false) {}

	Timestamp::Timestamp(const Time& time, bool bIs24Hours) : tTime(time), b24Hours(bIs24Hours) {}

	Timestamp::Timestamp(const Timestamp& timestamp) : tTime(timestamp.tTime), b24Hours(timestamp.b24Hours) {}

	Timestamp::~Timestamp() {}

	Timestamp Timestamp::Create(bool bIs24Hours)
	{
		return Timestamp(Time().Create(), bIs24Hours);
	}

	std::string Timestamp::FormatClock(bool bBrackets, bool bSkipSeconds) const
	{
		// 4:31:42 PM
		std::ostringstream timeStream;
		if (bBrackets) { timeStream << "["; }

		uint32_t hours = tTime.Hour;
		bool isAM = (hours < 12);

		if (!b24Hours && !isAM) { hours -= 12; }
		if (b24Hours) { Format::FillRight(timeStream, '0', 2); }
		timeStream << hours << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Minute;

		if (!bSkipSeconds)
		{
			timeStream << ":";
			Format::FillRight(timeStream, '0', 2);
			timeStream << tTime.Second;
		}

		if (!b24Hours) { timeStream << (isAM ? " AM" : " PM"); }
		if (bBrackets) { timeStream << "]"; }
		return timeStream.str();
	}

	std::string Timestamp::FormatISO8601() const
	{
		// 2024-10-29T22:41:46+00:00
		std::ostringstream timeStream;
		timeStream << (tTime.Year + 1900) << "-";

		Format::FillRight(timeStream, '0', 2);
		timeStream << (tTime.Month + 1) << "-";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Day << "T";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Hour << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Minute << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Second;

		int32_t offset = UTCOffset();
		bool negative = (offset < 0);
		offset = std::abs(offset);

		timeStream << (negative ? "-" : "+");
		if (offset < 10) { timeStream << "0"; }
		timeStream << std::to_string(offset);

		return timeStream.str();
	}

	std::string Timestamp::FormatUTCSimple() const
	{
		// 10/29/2024 10:41pm
		std::ostringstream timeStream;
		uint32_t hours = tTime.Hour;
		uint32_t year = (tTime.Year > 100 ? (tTime.Year - 100) : tTime.Year);
		bool isAM = (hours < 12);

		Format::FillRight(timeStream, '0', 2);
		timeStream << (tTime.Month + 1) << "/";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Day << "/";
		timeStream << year << " ";

		if (!isAM) { hours -= 12; }
		Format::FillRight(timeStream, '0', 2);
		timeStream << hours << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Minute << (isAM ? "am" : "pm");

		return timeStream.str();
	}

	std::string Timestamp::FormatUTCExtended() const
	{
		// Tue Oct 29 22:46:59 2024 UTC
		std::ostringstream timeStream;

		timeStream << m_days[(tTime.Day < m_days.size()) ? tTime.Day : 0] << " ";
		timeStream << m_months[(tTime.Month < m_months.size()) ? tTime.Month : 0] << " ";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Hour << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Minute << ":";

		Format::FillRight(timeStream, '0', 2);
		timeStream << tTime.Second << " ";
		timeStream << (tTime.Year + 1900) << " UTC";

		return timeStream.str();
	}

	Timestamp& Timestamp::operator=(const Timestamp& timestamp)
	{
		tTime = timestamp.tTime;
		b24Hours = timestamp.b24Hours;
		return *this;
	}

	std::chrono::system_clock::duration EpochDuration()
	{
		return std::chrono::system_clock().now().time_since_epoch();
	}

	uint64_t EpochHours()
	{
		return std::chrono::duration_cast<std::chrono::hours>(EpochDuration()).count();
	}

	uint64_t EpochMinutes()
	{
		return std::chrono::duration_cast<std::chrono::minutes>(EpochDuration()).count();
	}

	uint64_t EpochSeconds()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(EpochDuration()).count();
	}

	uint64_t EpochMilli()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(EpochDuration()).count();
	}

	uint64_t EpochNano()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(EpochDuration()).count();
	}

	int32_t UTCOffset()
	{
		return std::chrono::duration_cast<std::chrono::hours>(std::chrono::current_zone()->get_info(std::chrono::system_clock::now()).offset).count();
	}
}