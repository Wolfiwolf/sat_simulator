#pragma once

#include <iostream>
#include <string>

namespace sat_sim
{
namespace debug
{

class ProgressBar
{
private:
	int _max_val;
	int _len;

public:
	ProgressBar(int max_val, int len);
	~ProgressBar();

	void update(int current_val);

	void end();
};

} // namespace debug
} // namespace sat_sim
