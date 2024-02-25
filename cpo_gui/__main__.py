import dearpygui.dearpygui as dpg
from cpo_gui import cpo
from cpo_gui import audio
from serial.tools import list_ports
from multiprocessing import Process, Pipe
from multiprocessing.connection import Connection
import argparse

dpg.create_context()

def send(sender, app_data, conn: Connection):
    line = dpg.get_value("cw_out").upper()
    conn.send(line)
    dpg.set_value("cw_out", "")

def clear_log(sender, app_data):
    dpg.set_value("cw_in", "")


def main(conn: Connection):
    cw_text = [""]
    with dpg.value_registry():
        dpg.add_string_value(tag="cw_out")
        dpg.add_string_value(tag="cw_in")
    with dpg.window(tag="Primary Window"):
        dpg.add_input_text(source="cw_out", multiline=True)
        dpg.add_button(label="Send", callback=send, user_data=conn)
        dpg.add_button(label="Clear", callback=clear_log)
        with dpg.child_window(tag="CW Display"):
            dpg.add_text(source="cw_in")
    dpg.create_viewport(title='Morse CPO', width=600, height=400)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window("Primary Window", True)
    while dpg.is_dearpygui_running():
        if parent_conn.poll():
            letter = parent_conn.recv()
            if letter == "\n" and len(cw_text[-1]) < 50:
                cw_text[-1] += " "
            elif letter == "\n":
                cw_text.append("")
            else:
                cw_text[-1] += letter
            dpg.set_value("cw_in", "\n".join(cw_text))
        dpg.render_dearpygui_frame()

if __name__ == "__main__":
    try:
        hdlr = None
        audio_hdlr = None
        parser = argparse.ArgumentParser()
        parser.add_argument("--port", help="The Serial (COM) port to use", type=str)
        parser.add_argument("--baud", help="The Baud rate (default = 9600)", default=9600, type=int)
        parser.add_argument("--list", help="List COM ports", action="store_true")
        parser.add_argument("--audio", help="Use Computer Audio", action="store_true")
        args = parser.parse_args()
        if args.list:
            for port in list_ports.comports():
                print(port)
            exit()
        port = args.port if args.port.isupper() else args.port.upper()
        baud = args.baud
        if args.audio:
            parent_conn, child_conn = Pipe()
            parent_audio_conn, child_audio_conn = Pipe()
            hdlr = Process(target=cpo.device_handler, args=[child_conn, port, baud, parent_audio_conn])
            audio_hdlr = Process(target=audio.audio_handler, args=[child_audio_conn])
            audio_hdlr.start()
            hdlr.start()
        else:
            parent_conn, child_conn = Pipe()
            hdlr = Process(target=cpo.device_handler, args=[child_conn, port, baud])
            hdlr.start()
        main(parent_conn)
    finally:
        if hdlr:
            hdlr.kill()
        if audio_hdlr:
            audio_hdlr.kill()
        dpg.destroy_context()
