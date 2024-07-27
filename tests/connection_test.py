#!/bin/python3

from websockets.sync.client import connect

print("Connecting..")
websocket = None
try:
    websocket = connect("ws://localhost:5050")
except:
    print("Connecting has failed!")
    exit(1)


print("Sending...")
websocket.send(bytearray([0, 10]))
print("Waiting for response...")
message = websocket.recv()
print("Response received!")

is_response_ok = False
if len(message) == 4:
    if message[0] == 1:
        if message[1] == 1 and message[2] == 2 and message[3] == 3:
            is_response_ok = True

websocket.close_socket()

if is_response_ok:
    exit(0)
else:
    exit(1)
