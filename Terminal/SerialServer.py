# # Python 3 server example
# from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import serial
import sys
from flask import Flask, request, jsonify
import argparse
from collections import deque
from flask_cors import CORS, cross_origin
import time


app = Flask(__name__)
serial_port = None  # global variable
buffer = deque()    # variable size unlimited buffer for serial port data to push to and pop from
CORS(app, resources={r"/*": {"origins": "http://localhost:*"}})

# cors = CORS(app)
# app.config['CORS_HEADERS'] = 'Content-Type'


def write_to_serial(string):
    '''Send a string through the COM port in utf-8 encoding'''
    # Send data
    serial_port.write(string.encode('utf-8'))

def update_buffer():
    '''Read any bytes from the uart buffer and add to the serial deque buffer'''
    pass

@app.route("/")
def index():
    print("\033[93mRecived GET / request. Restarting serial\033[0m")
    init_port(com_port)
    return "request '/serial' to receive serial data"

@app.route('/send', methods=['POST'])
def send_serial():
    print("\033[93mRecived POST /send request.\nSending back empty 200 response.\033[0m")
    #print(request.data)
    data = request.data.decode('utf-8')
    # for key, value in data.items():
    #     print(f"Argument '{key}': {value}")
    print("\033[93mWriting to serial: \033[92m" + data + "\033[0m")
    serial_port.write(request.data)
    return "", 200
    #pass

import struct

def get_response():
    length = 0
    start_time = time.time()

    delimiter = b'\xFF\xFF\x00\x00\xF8\x00\x07\xE0\x00\x1F'
    delimiter_buffer = deque(maxlen=10)


    skipped_bytes = b''
    # skip until delimiter is seen
    official_timeout = serial_port.timeout
    serial_port.timeout = 1
    skippedbytes = b''
    while ((time.time() - start_time < 1000) and (bytearray(delimiter_buffer) != bytearray(delimiter))):
        rx = serial_port.read(1)
        if not rx: continue
        skippedbytes += rx
        delimiter_buffer.append(rx[0])
        #print(delimiter_buffer)
        #if(bytearray(delimiter_buffer)): break
    serial_port.timeout = official_timeout
    print("\033[93mSkipped leftover bytes: \033[96m" + str(skippedbytes) + "\033[0m")
    #TODO: Make handle errors better
    rx = serial_port.read(4)
    length = int.from_bytes(rx)
    #print(str(length))
    print("\033[93mRead serial packet size \033[96m" + str(rx) + "\033[93m as: \033[96m" + str(length) + "\033[0m")
    rx = bytearray()
    #time.sleep(1)

    half = int(length/2)
    #for i in range(length):
    rx += serial_port.read(half)
    rx += serial_port.read(length - half)
    
    #rx += serial_port.read(1)  # the \r character

    hex_representation = ''.join(struct.pack('B', x).hex() for x in rx)  #''.join(format(byte, '02x') for byte in rx)
    print("\033[93mSending back response:\033[0m")
    #print("\033[96m" + hex_representation + "\033[0m")
    return hex_representation
    return rx


    while True:

        rx += serial_port.read(1)
        data += rx
        if rx == '\r' or (time.time() - start_time > 8):
            return data

@app.route("/serial")
def get_serial():
    print("\033[93mRecived GET /serial request")

    return get_response()   # flask assumes 200 when sending something back


    # data = ""
    # size = 1 #serial_port.in_waiting
    # start_time = time.time()

    # while True:
    #     rx = serial_port.read().decode("Ascii")
    #     data += rx
    #     if rx == '\r' or (time.time() - start_time > 5):
    #         return data

    # old
    # if size > 0:
    #     rx = serial_port.readline()
    #     return rx.decode("Ascii")
    #     try:
    #         global buffer
    #         buffer += rx.decode("Ascii")
    #         return buffer
    #     except:
    #         return "error"
    # else: return "error"



def init_port(port):
    global serial_port
    if serial_port != None:
        serial_port.close()
    try:
        serial_port = serial.Serial(port=com_port, baudrate=115200, bytesize=8, timeout=5, stopbits=serial.STOPBITS_ONE)
        serial_port.set_buffer_size(rx_size = 128000, tx_size = 50)
        print("Listening to " + str(com_port))
    except:
        print("error: Could not open port " + str(com_port))
        exit()
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some arguments.')
    parser.add_argument('--port', type=str, default='COM4', help='Specify the port')
    args = parser.parse_args()

    com_port = args.port #sys.argv[1]
    #init_port(com_port)
        
    app.run(debug=True, host="localhost")