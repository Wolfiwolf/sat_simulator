#include "model_3d_file.hpp"

namespace sat_sim
{
namespace files
{
namespace models_3d
{

Model3dFile::Model3dFile(const std::string &file_path)
    : _file_path(file_path)
{
}

Model3dFile::~Model3dFile()
{
}

const std::vector<ModelObject> &Model3dFile::get_objects() const
{
	return _objects;
}

const std::string &Model3dFile::get_file_path() const
{
	return _file_path;
}

} // namespace models_3d
} // namespace files
} // namespace sat_sim
