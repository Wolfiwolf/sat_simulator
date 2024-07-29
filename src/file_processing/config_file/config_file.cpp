#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include "config_file.hpp"
#include "debug/logger/logger.hpp"

namespace sat_sim
{
namespace file_processing
{

void ConfigFile::_value_to_vector(const std::string &val, std::vector<std::string> &vec)
{
	std::regex pattern("([\\w]+)");
	std::sregex_iterator begin(val.begin(), val.end(), pattern), end;

	vec.clear();

	while (begin != end) {
		std::smatch match = *begin;

		vec.push_back(match.str());
		++begin;
	}
}

void ConfigFile::_process_line(const std::string &line)
{
	std::smatch m;
	std::regex key_val_rgx("^(.+):\\s(.+)$");

	if (line.size() == 0 || line[0] == '#')
		return;

	if (std::regex_match(line, m, key_val_rgx)) {
		std::string key = m[1];
		std::string val = m[2];

		if (key == "data_stream_type") {
			if (val == "tcp_ip" || val == "web_socket")
				this->data_stream_type = val;
		} else if (key == "data_stream_port") {
			this->data_stream_port = std::stoi(val);
		} else if (key == "output_file") {
			this->output_file = val;
		} else if (key == "outputs") {
			_value_to_vector(val, this->enabled_ouputs);
		} else if (key == "model_file_path") {
			this->model_file_path = val;
		} else if (key == "orbit_mode") {
			if (val == "sgp4")
				this->orbit_mode = val;
		}
	}
}

bool ConfigFile::load(const std::string &filepath)
{
	try {
		std::ifstream config_file(filepath);
		std::string line;

		while (std::getline(config_file, line))
			_process_line(line);

	} catch (const std::exception &e) {
		return false;
	}

	debug::Logger::Log("model_file_path: " + this->model_file_path, debug::LogLevel::SUCCESS);
	debug::Logger::Log("model_file_path: " + orbit_mode, debug::LogLevel::SUCCESS);

	debug::Logger::Log("data_stream: " + this->data_stream_type, debug::LogLevel::SUCCESS);
	if (this->data_stream_port)
		debug::Logger::Log("  - port: " + std::to_string(this->data_stream_port), debug::LogLevel::SUCCESS);

	debug::Logger::Log("outputs: ", debug::LogLevel::SUCCESS);
	for (const std::string &enabled_output : this->enabled_ouputs) {
		debug::Logger::Log("  - " + enabled_output, debug::LogLevel::SUCCESS);
		if (enabled_output == "file")
			sat_sim::debug::Logger::Log("    * file: " + this->output_file, sat_sim::debug::LogLevel::SUCCESS);
		if (enabled_output == "data_stream")
			debug::Logger::Log("    * port: " + std::to_string(this->data_stream_port), debug::LogLevel::SUCCESS);
	}

	return true;
}

} // namespace file_processing
} // namespace sat_sim
