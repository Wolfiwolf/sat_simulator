#include <fstream>
#include <sstream>

#include "files/model_3d_file/model_3d_file.hpp"
#include "obj_file.hpp"

namespace sat_sim
{
namespace files
{
namespace models_3d
{

ObjFile::ObjFile(const std::string &file_path)
    : Model3dFile(file_path)
{
}

ObjFile::~ObjFile()
{
}

bool ObjFile::load()
{
	return _proccess_file();
}

bool ObjFile::_proccess_file()
{
	std::ifstream model_file(_file_path);
	std::vector<ModelVertex> vertecies;
	std::vector<std::vector<uint32_t[3]>> _object_triangles;
	std::string line;

	while (std::getline(model_file, line)) {
		if (line[0] == 'o') {
			ModelObject mo;
			std::stringstream ss(line);

			std::getline(ss, mo.name, ' ');
			std::getline(ss, mo.name, ' ');
			_objects.push_back(mo);

			break;
		}
	}
	if (_objects.size() == 0)
		return false;

	while (true) {
		bool object_found = false;

		while (std::getline(model_file, line)) {
			if (line[0] == 'o') {
				ModelObject mo;
				std::stringstream ss(line);

				std::getline(ss, mo.name, ' ');
				std::getline(ss, mo.name, ' ');
				_objects.push_back(mo);

				object_found = true;

				break;
			}

			if (line[0] == 'v' && line[1] == ' ') {
				ModelVertex vert;
				std::stringstream ss(line);
				std::vector<std::string> tokens;
				std::string token;

				while (std::getline(ss, token, ' '))
					tokens.push_back(token);

				if (tokens.size() != 4)
					return false;

				vert.x = std::stod(tokens[1]);
				vert.y = std::stod(tokens[2]);
				vert.z = std::stod(tokens[3]);

				vertecies.push_back(vert);

			} else if (line[0] == 'f' && line[1] == ' ') {
				std::stringstream ss(line);
				std::vector<uint32_t> vertex_indexes;
				std::string token;
				int last_pol;

				std::getline(ss, token, ' ');
				while (std::getline(ss, token, ' ')) {
					std::stringstream ss2(token);
					std::string token2;

					std::getline(ss2, token2, '/');

					vertex_indexes.push_back(std::stoi(token2));
				}

				_objects[_objects.size() - 1].polygons.push_back({});
				last_pol = _objects[_objects.size() - 1].polygons.size() - 1;
				for (int v_i : vertex_indexes) {
					_objects[_objects.size() - 1].polygons[last_pol].push_back(vertecies[v_i - 1]);
				}
			}
		}

		if (!object_found)
			break;
	}

	return true;
}

} // namespace models_3d
} // namespace files
} // namespace sat_sim
