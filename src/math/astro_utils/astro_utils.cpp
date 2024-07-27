#include <iostream>
#include <math.h>
#include <stdio.h>

#include "GeographicLib/Geocentric.hpp"
#include "GeographicLib/MagneticModel.hpp"

#include "astro_utils.hpp"
#include "math/coordinate_systems/coordinate_systems.hpp"
#include "math/quaternion/quaternion.hpp"
#include "math/rotations/rotations.hpp"
#include "math/time_utils/time_utils.hpp"
#include "math/utils/utils.hpp"

namespace sat_sim
{
namespace math
{

const uint64_t AstroUtils::EARTH_R = 6371000;

static double _timestamp_to_days_in_year(uint64_t t_unix)
{
	uint16_t d_in_y;
	uint16_t y;
	uint8_t m;
	uint8_t day;
	uint8_t h;
	uint8_t min;
	uint8_t sec;
	uint16_t milli;
	TimeUtils::get_date_from_timestamp(t_unix, &d_in_y, &y, &m, &day, &h, &min, &sec, &milli);

	return d_in_y + (double)((((((h * 60) + min) * 60) + sec) * 1000) + milli) / (double)(24 * 60 * 60 * 1000);
}

Vector AstroUtils::get_sun_vec_eci(uint64_t t_unix, const Vector &pos_eci, bool with_shadow)
{
	Vector earth_vec, earth_vec_proj;

	double d = _timestamp_to_days_in_year(t_unix);

	double pi = 3.14;
	double L = 280.4606184 + ((36000.77005361 / 36525) * d);
	double g = 357.5277233 + ((35999.05034 / 36525) * d);
	double p = L + (1.914666471 * sin(g * pi / 180)) + (0.918994643 * sin(2 * g * pi / 180));
	double q = 23.43929 - ((46.8093 / 3600) * (d / 36525.0));

	double u_x = cos(p * pi / 180);
	double u_y = cos(q * pi / 180) * sin(p * pi / 180);
	double u_z = sin(q * pi / 180) * sin(p * pi / 180);

	if (!with_shadow)
		return {u_x, u_y, u_z};

	earth_vec = pos_eci * -1;

	// If sat is infront of the earth, there is sun
	if (Utils::rad_to_deg(earth_vec.angle_between({u_x, u_y, u_z})) > 90.0)
		return {u_x, u_y, u_z};

	earth_vec_proj = earth_vec.proj_onto_plane({u_x, u_y, u_z});

	if (earth_vec_proj.magnitude() <= EARTH_R)
		return {0, 0, 0};

	return {u_x, u_y, u_z};
}

Vector AstroUtils::get_mag_vec_eci(uint64_t t_unix, const Vector &pos_eci)
{
	GeographicLib::Geocentric geo_model(GeographicLib::Geocentric::WGS84());
	GeographicLib::MagneticModel mag_model("wmm2020");
	Vector ecef = CoordinateSystems::vec_eci_to_ecef(pos_eci, t_unix);
	Vector geodetic = CoordinateSystems::vec_ecef_to_geodetic(ecef);
	double mx, my, mz;

	mag_model(0, geodetic[0], geodetic[1], geodetic[2], mx, my, mz);

	return {mx, my, mz};
}

Vector AstroUtils::get_euler_angles_ZYX_from_telemetry(uint64_t t_unix, const Vector &eci_pos,
                                                       const Vector &sun_vec_body, const Vector &mag_vec_body)
{
	Vector sun_vec_eci, un_norm, axis_of_rot, mag_vec_eci,
	       target_mag_body, mag_vec_proj, mag_vec_eci_proj;
	Quaternion q;
	double degs_to_rot;

	// Create start orientation
	q = {1, 0, 0, 0};

	// First step: rotate base quaternion around [sun_vec_eci x
	// sun_vec_body] by degrees between sun_vec_eci and sun_vec_body
	sun_vec_eci = get_sun_vec_eci(t_unix, {0, 0, 0}, false);
	un_norm = sun_vec_body.cross(sun_vec_eci);
	axis_of_rot = un_norm * (1.0 / un_norm.magnitude());
	degs_to_rot = sun_vec_body.angle_between(sun_vec_eci);
	q.rot_around_axis(axis_of_rot, degs_to_rot);

	// Second step rotate around sun_vec_body so the projection on the
	// plane,defined by sun_vec_eci as a normal, of mag_vec_body fits the
	// projection of mag_vec_eci

	// Get magnetic field vector projections
	mag_vec_eci = {0, 1, 0};

	// Transform mag_eci to target_mag_body
	target_mag_body = CoordinateSystems::vec_inertial_to_body(Rotations::q_to_euler_ZYX(q), mag_vec_eci);

	mag_vec_proj = mag_vec_body.proj_onto_plane(sun_vec_body);
	mag_vec_eci_proj = target_mag_body.proj_onto_plane(sun_vec_body);

	// Calculate degrees between projections
	degs_to_rot = mag_vec_eci_proj.angle_between(mag_vec_proj);

	q.rot_around_axis(sun_vec_body, degs_to_rot);

	return Rotations::q_to_euler_ZYX(q);
}

} // namespace math
} // namespace sat_sim
