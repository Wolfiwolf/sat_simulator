#pragma once

#include <iostream>

#include "core/simulation_models/simulation_model.hpp"

namespace sat_sim
{
namespace outputs
{

class IOutput
{
protected:
	core::SimulationModel *_sim_model;

public:
	inline IOutput(core::SimulationModel *model)
	    : _sim_model(model)
	{
	}

	inline virtual ~IOutput()
	{
	}

	virtual void output(uint64_t t) = 0;
};

} // namespace outputs
} // namespace sat_sim
