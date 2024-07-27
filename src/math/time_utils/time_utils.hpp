#pragma once

#include "cstdint"

namespace sat_sim
{
namespace math
{

class TimeUtils
{
public:
	static void get_date_from_timestamp(uint64_t timestamp_unix, uint16_t *day_in_year = nullptr, uint16_t *year = nullptr,
	                                    uint8_t *month = nullptr, uint8_t *day = nullptr, uint8_t *hour = nullptr,
	                                    uint8_t *min = nullptr, uint8_t *sec = nullptr, uint16_t *millis = nullptr);
};

} // namespace math
} // namespace sat_sim
