import pyaudio
from multiprocessing.connection import Connection
import numpy as np

FREQ = 500
SAMPLE_RATE = 44100
CHUNK = 2
VOLUME = 0.5  # [0.0, 1.0]

phase = 0.0
# Function to play tone
def generate_tone(frequency, frame_count, phase_acc):
    global phase
    t = (np.arange(frame_count) + phase_acc) / SAMPLE_RATE
    tone = np.sin(frequency * 2 * np.pi * t)
    phase = (phase_acc + frame_count) % SAMPLE_RATE
    return (tone * 32767).astype(np.int16)

def callback(in_data, frame_count, time_info, status):
    global phase
    data = generate_tone(FREQ, frame_count, phase)
    return (data.tobytes(), pyaudio.paContinue)

def audio_handler(conn: Connection):
    t = 0
    p = pyaudio.PyAudio()
    stream = None
    try:
        stream = p.open(format=pyaudio.paInt16,
                    channels=1,
                    rate=SAMPLE_RATE,
                    output=True,
                    frames_per_buffer=CHUNK,
                    stream_callback=callback
                    )
        stream.stop_stream()
        isPlaying = False
        shouldPlay = False
        while True:
            if conn.poll():
                shouldPlay = conn.recv()
                print(shouldPlay)
            if shouldPlay:
                stream.start_stream()
                isPlaying = True
            else:
                if isPlaying:
                    stream.stop_stream()
                    isPlaying = False
    finally:
        if stream:
            stream.stop_stream()
            stream.close()
        p.terminate()