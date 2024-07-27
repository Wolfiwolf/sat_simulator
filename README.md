# Cubesat Simulator Engine

## Description
This is an expandable satellite simulator for earth bound orbits. It simulates the position, attitude of the satellite also taking into account air resistance and gravity gradient.

It contains <b>sensors</b> like magnetometers, sun sensors and gyros and <b>actuators</b> like magnetorquers and reaction wheels for testing out <b>control and estimation algorithms</b>. You can implement your own sensor mathematical models in the <b>sat_simulator_sensor_models</b> and import them into this project as .so files.

It takes in a <b>3D file</b> which is used for air resistance, sensor and actuator calculations. You can specify the sensor and actuator locations in the model.

It has a expandable architecture which allows adding new sensors, actuators, data streams and more to serve specific needs of the organization.

## Supported platforms
Currently only Linux is supported. Windows support will probably be added later on.

## Quick start

### Installing GeographicLib
Install geographic lib on your system. The project expects that a libGeographicLib.so file is in the LD_PATH.

### Importing sensor models
Before starting you have to build the sat_simulator_sensor project which will generate sensor models .so files. You have to put these files into the ```sensor_models``` directory in the root of the project.

### Configuration file
In the root of the project create a ```conf.txt``` file. An example conf.txt file would look like this
```txt
data_stream_type: web_socket        // Use the web socket protocol for controlling the simulator
data_stream_port: 5050              // The port of the web socket server
outputs: file | data_stream         // The outputs of the simulator
output_file: log.bin                // Output file
model_file_path: cubesat.obj        // Path to the 3D model of the satellite
orbit_mode: sgp4                    // Orbit calculation mode
mass: 1                             // Mass of satellite in kg
inertia_matrix: 1 0 0 0 1 0 0 0 1   // The 3x3 inertia matrix specified row by row from left to right
```

### Building the project
To build the project simply run
```bash
make all
```

### Run the project
After building the project a ```sat_sim``` executable will be generated. Run
```bash
./sat_sim
```
When you want the simulation to stop press CTRL + C.

## Config file
The config specifies the starting state of the simulation and it's parameters. The parameters are the following.

Parameter        | Value                    | Description
-----------------|--------------------------|------------
data_stream_type | web_socket \| tcp_socket | The way of communication with the simulator
data_stream_port | port                     | If the data stream selected needs a port
outputs          | file \| data_stream      | Multiple or just one output type seperated by '\|'
output_file      | filepath                 | If the output selected is a file
model_file_path  | filepath                 | Filepath to the 3D model of the satellite
orbit_mode       | sgp4                     | Way of calculating the position of the satellite

## 3D model
The 3D model of the satellite can be any shape and size. The model must be exported as a obj file. When placing the sensors they must be created in the following way.

### Sensor and actuator objects
The sensor object must have a main body and 3 axis of direction (x, y and z). The naming for the different type of objects are the following.

#### Sensors
The format for sensor naming is <sensor_name><number>. The axis of the sensor is named as <sensor_name><X|Y|Z><number> currently supported sensors are:
- MagnetometerSensor
- SunSensor

#### Actuators
The format for sensor naming is <actuator_name><number>. The axis of the sensor is named as <actuator_name><X|Y|Z><number> currently supported sensors are:
- MagnetorquerActuator
    - Only has X axis which is the direction of magnet field generated
- ReactionWheelActuator
    - Only has X axis which is the normal of reaction wheel. The direction of spin is determined by the right thumb rule around the normal.
