#pragma once

#include <string>
#include <vector>

#include "files/model_3d_file/model_3d_file.hpp"

namespace sat_sim
{
namespace files
{
namespace models_3d
{

class ObjFile : public Model3dFile
{
private:
	bool _proccess_file();

public:
	ObjFile(const std::string &file_path);
	~ObjFile();

	bool load();
};

} // namespace models_3d
} // namespace files
} // namespace sat_sim
