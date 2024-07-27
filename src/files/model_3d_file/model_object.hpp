#pragma once

#include <string>
#include <vector>

#include "./model_vertex.hpp"

namespace sat_sim
{
namespace files
{
namespace models_3d
{

struct ModelObject {
	std::string name;
	std::vector<std::vector<ModelVertex>> polygons;
};

} // namespace models_3d
} // namespace files
} // namespace sat_sim
