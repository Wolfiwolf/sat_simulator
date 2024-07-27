EXE = sat_sim
SRC_DIR = ./src 
BLD_DIR = ./build
SENSOR_MODELS_DIR = ./sensor_models

MAIN_SRC = src/main.cpp
ALL_INCS = $(shell find $(SRC_DIR) -wholename "*.hpp")

EXTERNAL_DIR = src/external

SGP4_DIR = src/external/sgp4
SGP4_SRC = $(shell find $(SGP4_DIR) -wholename "*.cpp")
SGP4_OBJS = $(SGP4_SRC:src/%.cpp=$(BLD_DIR)/%.o)
SGP4_LIB = libsgp4.so

CUBESAT_SIM_DEBUG_DIR = src/debug
CUBESAT_SIM_DEBUG_SRC = $(shell find $(CUBESAT_SIM_DEBUG_DIR) -wholename "*.cpp")
CUBESAT_SIM_DEBUG_OBJS = $(CUBESAT_SIM_DEBUG_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_DEBUG_LIB = libcubesat_sim_debug.so

CUBESAT_SIM_SYSTEM_DIR = src/system
CUBESAT_SIM_SYSTEM_SRC = $(shell find $(CUBESAT_SIM_SYSTEM_DIR) -wholename "*.cpp")
CUBESAT_SIM_SYSTEM_OBJS = $(CUBESAT_SIM_SYSTEM_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_SYSTEM_LIB = libcubesat_sim_system.so

CUBESAT_SIM_STORAGE_DIR = src/storage
CUBESAT_SIM_STORAGE_SRC = $(shell find $(CUBESAT_SIM_STORAGE_DIR) -wholename "*.cpp")
CUBESAT_SIM_STORAGE_OBJS = $(CUBESAT_SIM_STORAGE_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_STORAGE_LIB = libcubesat_sim_storage.so

CUBESAT_SIM_FILE_PROCESSING_DIR = src/file_processing
CUBESAT_SIM_FILE_PROCESSING_SRC = $(shell find $(CUBESAT_SIM_FILE_PROCESSING_DIR) -wholename "*.cpp")
CUBESAT_SIM_FILE_PROCESSING_OBJS = $(CUBESAT_SIM_FILE_PROCESSING_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_FILE_PROCESSING_LIB = libcubesat_sim_file_processing.so

CUBESAT_SIM_FILES_DIR = src/files
CUBESAT_SIM_FILES_SRC = $(shell find $(CUBESAT_SIM_FILES_DIR) -wholename "*.cpp")
CUBESAT_SIM_FILES_OBJS = $(CUBESAT_SIM_FILES_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_FILES_LIB = libcubesat_sim_files.so

CUBESAT_SIM_MATH_DIR = src/math
CUBESAT_SIM_MATH_SRC = $(shell find $(CUBESAT_SIM_MATH_DIR) -wholename "*.cpp")
CUBESAT_SIM_MATH_OBJS = $(CUBESAT_SIM_MATH_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_MATH_LIB = libcubesat_sim_math.so

CUBESAT_SIM_DATA_STREAMS_DIR = src/data_streams
CUBESAT_SIM_DATA_STREAMS_SRC = $(shell find $(CUBESAT_SIM_DATA_STREAMS_DIR) -wholename "*.cpp")
CUBESAT_SIM_DATA_STREAMS_OBJS = $(CUBESAT_SIM_DATA_STREAMS_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_DATA_STREAMS_LIB = libcubesat_sim_data_streams.so

CUBESAT_SIM_CORE_DIR = src/core
CUBESAT_SIM_CORE_SRC = $(shell find $(CUBESAT_SIM_CORE_DIR) -wholename "*.cpp")
CUBESAT_SIM_CORE_OBJS = $(CUBESAT_SIM_CORE_SRC:src/%.cpp=$(BLD_DIR)/%.o)
CUBESAT_SIM_CORE_LIB = libcubesat_sim_core.so

all: $(EXE)

$(SGP4_LIB): $(SGP4_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $^

$(CUBESAT_SIM_DEBUG_LIB): $(CUBESAT_SIM_DEBUG_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $^

$(CUBESAT_SIM_FILES_LIB): $(CUBESAT_SIM_FILES_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_FILES_OBJS)

$(CUBESAT_SIM_STORAGE_LIB): $(CUBESAT_SIM_STORAGE_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_STORAGE_OBJS) \
		-L$(BLD_DIR) \
		-lcubesat_sim_debug

$(CUBESAT_SIM_MATH_LIB): $(CUBESAT_SIM_MATH_OBJS) $(SGP4_LIB)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_MATH_OBJS) \
		-L$(BLD_DIR) \
		-lGeographicLib -lsgp4 \
		-Wl,-rpath,'./build'

$(CUBESAT_SIM_DATA_STREAMS_LIB): $(CUBESAT_SIM_DATA_STREAMS_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_DATA_STREAMS_OBJS)

$(CUBESAT_SIM_FILE_PROCESSING_LIB): $(CUBESAT_SIM_FILE_PROCESSING_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_FILE_PROCESSING_OBJS)

$(CUBESAT_SIM_SYSTEM_LIB): $(CUBESAT_SIM_SYSTEM_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $(CUBESAT_SIM_SYSTEM_OBJS) 

$(CUBESAT_SIM_CORE_LIB): $(CUBESAT_SIM_CORE_OBJS)
	@mkdir -p $(BLD_DIR)/
	g++ -shared -o $(BLD_DIR)/$@ $^

$(EXE): $(MAIN_SRC) $(ALL_INCS) $(CUBESAT_SIM_CORE_LIB) $(CUBESAT_SIM_DEBUG_LIB) $(CUBESAT_SIM_SYSTEM_LIB) $(CUBESAT_SIM_STORAGE_LIB) \
	$(CUBESAT_SIM_FILES_LIB) $(CUBESAT_SIM_MATH_LIB) $(CUBESAT_SIM_DATA_STREAMS_LIB) \
	$(CUBESAT_SIM_FILE_PROCESSING_LIB)
	g++ -o $(EXE) $(MAIN_SRC) -std=c++17 -Isrc -I$(EXTERNAL_DIR) \
		-L$(BLD_DIR) -L$(SENSOR_MODELS_DIR) \
		-lsgp4 -lGeographicLib -lcubesat_sim_core -lcubesat_sim_math -lcubesat_sim_debug -lcubesat_sim_storage -lcubesat_sim_data_streams \
		-lcubesat_sim_system -lcubesat_sim_files -lcubesat_sim_file_processing -lsun_sensor_model -lmagnetometer_sensor_model \
		-Wl,-rpath,'./build',-rpath,'./sensor_models',-rpath,'/usr/local/lib'

$(BLD_DIR)/%.o: */%.cpp 
	@mkdir -p $(@D)
	@g++ -fPIC -c $< -o $@ -std=c++17  -Isrc -I$(EXTERNAL_DIR) 

clean:
	@rm -rf $(BLD_DIR) $(EXE) tags

test:
	@./test.sh
