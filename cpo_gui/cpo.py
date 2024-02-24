import serial
from multiprocessing.connection import Connection

ON = b'1'

def device_handler(conn: Connection, port: str, baud: int, audio_conn: Connection=None):
    with serial.Serial(port, baud) as device:
        run_status = True
        while run_status:
            val = device.read()
            key =  val == ON
            if audio_conn:
                audio_conn.send(key)

