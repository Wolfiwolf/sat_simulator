#pragma once

#include "math/vector/vector.hpp"

sat_sim::math::Vector magnetometer_sensor_model(double delta_t, const sat_sim::math::Vector &mag_vec);
