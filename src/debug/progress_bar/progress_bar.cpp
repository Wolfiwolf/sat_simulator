#include "progress_bar.hpp"

namespace sat_sim
{
namespace debug
{

ProgressBar::ProgressBar(int max_val, int len)
    : _max_val(max_val), _len(len)
{
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::update(int current_val)
{
	int i;

	std::cout << "[";
	for (i = 0; i < _len; ++i) {
		int limit = (current_val / (float)_max_val) * _len;

		std::cout << (i < limit ? "=" : " ");
	}
	std::cout << "]";

	printf(" %.2f%%\r", (current_val / (float)_max_val) * 100);

	std::cout.flush();
}

void ProgressBar::end()
{
	int i;

	std::cout << " ";
	for (i = 0; i < _len; ++i) {
		int limit = (_max_val / (float)_max_val) * _len;

		std::cout << " ";
	}

	std::cout << "          \r";

	std::cout.flush();
}

} // namespace debug
} // namespace sat_sim
