#pragma once

#include <string>

#include "../isystem_info.hpp"

namespace sat_sim
{
namespace system
{

class LinuxSystemInfo : public ISystemInfo
{
private:
	std::string _user_name;

	void _get_user_name();

public:
	void init();

	std::string get_user_name();
};

} // namespace system
} // namespace sat_sim
