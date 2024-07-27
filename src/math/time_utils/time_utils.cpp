#include "time_utils.hpp"

#include <ctime>

namespace sat_sim
{
namespace math
{

void TimeUtils::get_date_from_timestamp(uint64_t timestamp_unix, uint16_t *day_in_year, uint16_t *year, uint8_t *month, uint8_t *day,
                                        uint8_t *hour, uint8_t *min, uint8_t *sec, uint16_t *millis)
{

	uint64_t timestamp_unix_sec = timestamp_unix / 1000;
	uint16_t milliseconds = (timestamp_unix / 1000.0) - timestamp_unix_sec;

	struct tm *time_info = localtime((time_t *)&timestamp_unix_sec);

	if (day_in_year != nullptr)
		*day_in_year = time_info->tm_yday;
	if (year != nullptr)
		*year = time_info->tm_year + 1900;
	if (month != nullptr)
		*month = time_info->tm_mon + 1;
	if (day != nullptr)
		*day = time_info->tm_mday;
	if (hour != nullptr)
		*hour = time_info->tm_hour - (time_info->tm_gmtoff / 60) / 60;
	if (min != nullptr)
		*min = time_info->tm_min;
	if (sec != nullptr)
		*sec = time_info->tm_sec;
	if (millis != nullptr)
		*millis = milliseconds;
}
} // namespace math
} // namespace sat_sim
