#include "utils.hpp"

namespace sat_sim
{
namespace math
{

double Utils::deg_to_rad(double deg)
{
	return deg * 0.0174532925;
}

double Utils::rad_to_deg(double rad)
{
	return rad * 57.2957795;
}

} // namespace math
} // namespace sat_sim
