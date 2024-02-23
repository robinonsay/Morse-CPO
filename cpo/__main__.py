import dearpygui.dearpygui as dpg
from cpo import cpo
from serial.tools import list_ports
from multiprocessing import Process, Pipe
from multiprocessing.connection import Connection
import argparse

dpg.create_context()

def send(sender, app_data, conn: Connection):
    line = dpg.get_value("cw_out")
    conn.send(line)
    dpg.set_value("cw_out", "")

def main():
    with dpg.value_registry():
        dpg.add_string_value(tag="cw_out")
        dpg.add_string_value(tag="cw_in")
    with dpg.window(tag="Primary Window"):
        dpg.add_input_text(label="CW Out", source="cw_out")
        dpg.add_button(label="Send")
        dpg.add_text(source="cw_in")
    dpg.create_viewport(title='Morse CPO', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window("Primary Window", True)
    while dpg.is_dearpygui_running():
        if parent_conn.poll():
            dpg.set_value("cw_in", parent_conn.recv())
        dpg.render_dearpygui_frame()

if __name__ == "__main__":
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("--port", help="The Serial (COM) port to use", type=str)
        parser.add_argument("--baud", help="The Baud rate (default = 9600)", default=9600, type=int)
        parser.add_argument("--list", help="List COM ports", action="store_true")
        args = parser.parse_args()
        if args.list:
            for port in list_ports.comports():
                print(port)
            exit()
        port = args.port if args.port.isupper() else args.port.upper()
        baud = args.baud
        parent_conn, child_conn = Pipe()
        hdlr = Process(target=cpo.device_handler, args=[child_conn, port, baud])
        hdlr.start()
        main()
    finally:
        hdlr.kill()
        dpg.destroy_context()
