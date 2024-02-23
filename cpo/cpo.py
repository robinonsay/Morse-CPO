import serial
from multiprocessing.connection import Connection

def device_handler(conn: Connection, port: str, baud: int):
    text = ""
    with serial.Serial(port, baud, timeout=10E-3) as device:
        run_status = True
        while run_status:
            line = device.readline()
            if line:
                line = line.decode()
                if line == "\r\n":
                    text += "\n"
                else:
                    text +=  line
                conn.send(text)
            if conn.poll():
                line:str = conn.recv()
                device.write(line.encode())
