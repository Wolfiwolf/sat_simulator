
from sim_data_stream import SimDataType


STRING_TO_SIM_DATA_TYPE = {
    "position": SimDataType.POSITION,
    "velocity": SimDataType.VELOCITY,
    "orientation_euler": SimDataType.ORIENTATION_EULER,
}


class ConfigFile:
    def __init__(self, filepath):
        self.values_to_plot = []
        self.__parse(filepath)

    def __parse(self, filepath):
        file = open(filepath, "r")

        key, values = self.__get_next_key_val(file)
        if key == "plot":
            self.__parse_plot(values)

        file.close()

    def __get_next_key_val(self, file):
        line = file.readline()
        key = line.split(": ")[0]
        raw_values = line.split(": ")[1][0:-1]
        values = raw_values.split(" ")[0:-1]

        additional_values = "\\"
        while additional_values[-1] == "\\":
            line = file.readline()[0:-1]
            additional_values = line.split(" ")
            if additional_values[-1] == "\\":
                values.extend(additional_values[0:-1])
            else:
                values.extend(additional_values)

        clean_values = []
        for val in values:
            if val != "" and val != "\\":
                clean_values.append(val)
        values = clean_values

        return key, values

    def __parse_plot(self, values):
        for val in values:
            if val.lower() in STRING_TO_SIM_DATA_TYPE:
                self.values_to_plot.append(STRING_TO_SIM_DATA_TYPE[val.lower()])
        pass
