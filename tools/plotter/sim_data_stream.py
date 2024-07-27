from enum import Enum
import typing
from websockets.sync.client import connect
import timeit
import struct


class SimDataType(Enum):
    NONE = 0,
    TIME = 1
    POSITION = 2,
    VELOCITY = 3,
    ORIENTATION_Q = 4
    ORIENTATION_EULER = 5

SIM_DATA_TYPE_TO_VALUES_COUNT = {
    SimDataType.POSITION: 3,
    SimDataType.VELOCITY: 3,
    SimDataType.ORIENTATION_EULER: 3,
}

SIM_DATA_TYPE_TO_NAME = {
    SimDataType.POSITION: "ECI Position",
    SimDataType.VELOCITY: "ECI Velocity",
    SimDataType.ORIENTATION_EULER: "ECI Orientation euler ZYX",
}


class SimData:
    def __init__(self):
        self.type = SimDataType.NONE
        self.data = []


class SimDataStream:
    def __init__(self):
        self.websocket = None
        self.received_datas = []

    def start(self):
        try:
            self.websocket = connect("ws://localhost:5050")
            return True
        except:
            return False

    def close(self):
        print("closing socket")
        if self.websocket:
            self.websocket.close_socket()

    def is_new_data(self) -> bool:
        return len(self.received_datas) != 0

    def get_new_data(self) -> typing.List[SimData]:
        ret_data = self.received_datas
        self.received_datas = []
        return ret_data

    def listen_for_data(self) -> bool:
        if not self.websocket:
            return False
        message = self.websocket.recv()
        self.__parse_bytes_to_data(message)
        return True

    def __parse_bytes_to_data(self, data):
        offset = 1
        t = struct.unpack("Q", bytearray(data[offset:offset + 8]))[0]
        offset += 8

        dt = SimData()
        dt.type = SimDataType.TIME
        dt.data = [t]
        self.received_datas.append(dt)

        x = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        y = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        z = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8

        dt = SimData()
        dt.type = SimDataType.POSITION
        dt.data = [x, y, z]
        self.received_datas.append(dt)

        xv = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        yv = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        zv = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8

        dt = SimData()
        dt.type = SimDataType.VELOCITY
        dt.data = [xv, yv, zv]
        self.received_datas.append(dt)

        orient_w = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        orient_x = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        orient_y = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        orient_z = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8

        dt = SimData()
        dt.type = SimDataType.ORIENTATION_Q
        dt.data = [orient_w, orient_x, orient_y, orient_z]
        self.received_datas.append(dt)

        phi = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        theta = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8
        psi = struct.unpack("d", bytearray(data[offset:offset + 8]))[0]
        offset += 8

        dt = SimData()
        dt.type = SimDataType.ORIENTATION_EULER
        dt.data = [phi, theta, psi]
        self.received_datas.append(dt)
