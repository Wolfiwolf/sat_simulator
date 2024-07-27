#include <fstream>
#include <regex>
#include <string>

#include "config_file.hpp"
#include "debug/logger/logger.hpp"

namespace sat_sim
{
namespace file_processing
{

bool ConfigFile::load(const std::string &filepath)
{
	try {
		std::ifstream config_file("conf.txt");
		std::regex key_val_rgx("^(.+):\\s(.+)$");
		std::regex array_val_rgx("\\s?(\\w+)[\\s\\|]?+");
		std::string line;

		while (std::getline(config_file, line)) {
			std::smatch m;

			if (line.size() == 0 || line[0] == '#')
				continue;

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
					std::regex pattern("([\\w]+)");
					std::sregex_iterator begin(val.begin(), val.end(), pattern), end;

					while (begin != end) {
						std::smatch match = *begin;

						this->enabled_ouputs.push_back(match.str());
						++begin;
					}
				} else if (key == "model_file_path") {
					this->model_file_path = val;
				} else if (key == "orbit_mode") {
					if (val == "physics" || val == "orbit_mode")
						this->orbit_mode = val;
				}

			} else {
				return false;
			}
		}
	} catch (const std::exception &e) {
		return false;
	}

	// Print out the settings
	debug::Logger::Log("data_stream: " + data_stream_type, debug::LogLevel::SUCCESS);
	debug::Logger::Log("data_stream_port: " + std::to_string(this->data_stream_port), debug::LogLevel::SUCCESS);

	for (const std::string &enabled_output : this->enabled_ouputs) {
		if (enabled_output == "file")
			sat_sim::debug::Logger::Log("output_file: " + this->output_file, sat_sim::debug::LogLevel::SUCCESS);
	}

	debug::Logger::Log("model_file_path: " + this->model_file_path, debug::LogLevel::SUCCESS);
	debug::Logger::Log("model_file_path: " + orbit_mode, debug::LogLevel::SUCCESS);

	return true;
}

} // namespace file_processing
} // namespace sat_sim
