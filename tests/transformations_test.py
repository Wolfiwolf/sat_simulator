#!/bin/python3

from websockets.sync.client import connect
import struct

def test_eci_2_ecef(ws):
    parameters = [4]
    parameters.extend([x for x in struct.pack("f", 5000)])
    parameters.extend([x for x in struct.pack("f", 6000)])
    parameters.extend([x for x in struct.pack("f", 7000)])

    ws.send(bytearray(parameters))
    message = ws.recv()
    if message[0] != 1:
        return False

    return True

def test_ecef_2_eci(ws):
    pass
def test_ecef_2_geodetic(ws):
    pass

print("Connecting..")
ws = None
try:
    ws = connect("ws://localhost:5050")
except:
    print("Connecting has failed!")
    exit(1)



ws.close_socket()


