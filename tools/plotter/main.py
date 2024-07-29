#!/bin/python3

if __name__ != "__main__":
    exit(1)

import math
from config_file import ConfigFile
from plotter import Plotter
from sim_data_stream import SIM_DATA_TYPE_TO_NAME, SIM_DATA_TYPE_TO_VALUES_COUNT, SimDataStream, SimDataType
import signal

stop_flag = False
new_data_stop_flag = False

def signal_handler(sig, frame):
    global stop_flag, new_data_stop_flag
    if not new_data_stop_flag:
        new_data_stop_flag = True
        return
    stop_flag = True

signal.signal(signal.SIGINT, signal_handler)


config_file = ConfigFile("config.txt")


plotter = Plotter(2)
sim_data_stream = SimDataStream()


num_of_graphs = 0
for value_type in SimDataType:
    if value_type not in config_file.values_to_plot:
        continue
    plotter.add_graph(SIM_DATA_TYPE_TO_NAME[value_type], "Samples", "Value")
    for _ in range(0, SIM_DATA_TYPE_TO_VALUES_COUNT[value_type]):
        plotter.add_plot(num_of_graphs)
    num_of_graphs += 1


if not sim_data_stream.start():
    print("Can't connect to simulator!")
    exit(1)
plotter.start()

plot_on_every = 3
plot_wait_counter = 0

while not new_data_stop_flag:
    if not sim_data_stream.listen_for_data():
        break

    new_data = sim_data_stream.get_new_data()

    # Add data to plotter
    for chunk in new_data:
        if chunk.type == SimDataType.TIME:
            t = chunk.data[0];
            plotter.add_value_x(t)

        if chunk.type not in config_file.values_to_plot:
            continue

        if chunk.type == SimDataType.POSITION:
            x = chunk.data[0];
            y = chunk.data[1];
            z = chunk.data[2];
            plotter.add_values(0, [x, y, z])
        elif chunk.type == SimDataType.VELOCITY:
            x = chunk.data[0];
            y = chunk.data[1];
            z = chunk.data[2];
            plotter.add_values(1, [x, y, z])
        elif chunk.type == SimDataType.ORIENTATION_Q:
            q = chunk.data[0];
            x = chunk.data[1];
            y = chunk.data[2];
            z = chunk.data[3];
        elif chunk.type == SimDataType.ORIENTATION_EULER:
            phi = chunk.data[0];
            theta = chunk.data[1];
            psi = chunk.data[2];
            plotter.add_values(2, [phi, theta, psi])

    plot_wait_counter += 1
    if plot_wait_counter == plot_on_every:
        plotter.update()
        plot_wait_counter = 0

while not stop_flag:
    plotter.update()

sim_data_stream.close()


