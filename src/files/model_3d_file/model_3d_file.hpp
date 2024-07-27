#pragma once

#include <string>
#include <vector>

#include "./model_object.hpp"

namespace sat_sim
{
namespace files
{
namespace models_3d
{

class Model3dFile
{
protected:
	std::string _file_path;
	std::vector<ModelObject> _objects;

public:
	Model3dFile(const std::string &file_path);
	virtual ~Model3dFile();

	virtual bool load() = 0;

	const std::string &get_file_path() const;

	const std::vector<ModelObject> &get_objects() const;
};

} // namespace models_3d
} // namespace files
} // namespace sat_sim
