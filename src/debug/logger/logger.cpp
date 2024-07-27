#include <chrono>

#include "logger.hpp"

namespace sat_sim
{
namespace debug
{

void Logger::Log(const std::string &msg, LogLevel log_level)
{
	const std::time_t now = std::time(nullptr);
	const std::tm calendar_time = *std::localtime(std::addressof(now));

	switch (log_level) {
	case LogLevel::NORMAL:
		std::cout << "\033[0m";
		break;
	case LogLevel::SUCCESS:
		std::cout << "\033[32m";
		break;
	case LogLevel::WARNING:
		std::cout << "\033[33m";
		break;
	case LogLevel::DANGER:
		std::cout << "\033[31m";
		break;
	}

	std::cout << "[";
	std::cout << calendar_time.tm_hour << ":";
	std::cout << calendar_time.tm_min << ":";
	std::cout << calendar_time.tm_sec << "] ";

	std::cout << msg << "\033[0m\n";
}

} // namespace debug
} // namespace sat_sim
