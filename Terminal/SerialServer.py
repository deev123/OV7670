# # Python 3 server example
# from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import serial
import sys
from flask import Flask, request, jsonify
import argparse
from collections import deque
from flask_cors import CORS, cross_origin


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
    init_port(com_port)
    return "request '/serial' to receive serial data"

@app.route('/send', methods=['POST'])
def send_serial():
    #print(request.data)
    data = request.data.decode('utf-8')
    # for key, value in data.items():
    #     print(f"Argument '{key}': {value}")
    print("sending: \033[92m" + data + "\033[0m")
    serial_port.write(request.data)
    return jsonify({"message": "Data sent successfully"}), 200
    #pass

@app.route("/serial")
def get_serial():
    size = serial_port.in_waiting
    if size > 0:
        rx = serial_port.readline(size)
        return rx.decode("Ascii")
        try:
            global buffer
            buffer += rx.decode("Ascii")
            return buffer
        except:
            return "error"
    else: return "error"
    

def init_port(port):
    global serial_port
    if serial_port != None:
        serial_port.close()
    try:
        serial_port = serial.Serial(port=com_port, baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
        serial_port.set_buffer_size(rx_size = 12800, tx_size = 50)
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