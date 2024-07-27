#pragma once

#include <iostream>
#include <string>

namespace sat_sim
{
namespace debug
{

enum class LogLevel {
	NORMAL = 0,
	SUCCESS,
	WARNING,
	DANGER
};

class Logger
{
public:
	static void Log(const std::string &msg, LogLevel log_level);
};
} // namespace debug
} // namespace sat_sim
