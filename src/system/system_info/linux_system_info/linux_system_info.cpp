#include <iostream>
#include <unistd.h>

#include "linux_system_info.hpp"

namespace sat_sim
{
namespace system
{

void LinuxSystemInfo::init()
{
	_get_user_name();
}

std::string LinuxSystemInfo::get_user_name()
{
	return _user_name;
}

void LinuxSystemInfo::_get_user_name()
{
	char name[40];

	getlogin_r(name, 40);
	_user_name = std::string(name);
}

} // namespace system
} // namespace sat_sim
