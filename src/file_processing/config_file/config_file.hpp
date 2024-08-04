#pragma once

#include <string>
#include <vector>

namespace sat_sim
{
namespace file_processing
{

class ConfigFile
{
public:
	std::string data_stream_type;
	int data_stream_port;
	std::vector<std::string> enabled_ouputs;
	std::string output_file;
	std::string model_file_path;
	std::string orbit_mode;

	bool load(const std::string &filepath);

private:
	void _process_line(const std::string &line);

	static void _value_to_vector(const std::string &val, std::vector<std::string> &vec);

	static void _value_to_vector(const std::string &val, std::vector<double> &vec);

	static std::string _value_to_string(const std::string &val);
};

} // namespace file_processing
} // namespace sat_sim
