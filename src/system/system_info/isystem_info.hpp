#pragma once

#include <string>

namespace sat_sim
{
namespace system
{

class ISystemInfo
{
public:
	virtual void init() = 0;
	virtual std::string get_user_name() = 0;
};

} // namespace system
} // namespace sat_sim
