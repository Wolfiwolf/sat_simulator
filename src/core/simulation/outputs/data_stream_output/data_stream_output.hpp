#include "../ioutput.hpp"
#include "core/simulation_models/simulation_model.hpp"
#include "data_streams/data_streams/idata_stream.hpp"

namespace sat_sim
{
namespace outputs
{

class DataStreamOutput : public IOutput
{
private:
	data_stream::IDataStream *_data_stream;
	int _server_id;

public:
	DataStreamOutput(core::SimulationModel *sim_model, data_stream::IDataStream *data_stream);

	~DataStreamOutput();

	void output(uint64_t t) override;
};

} // namespace outputs
} // namespace sat_sim
