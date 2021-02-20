import neurol
import neurol.connect_device
import neurol.BCI
import neurol.streams
import neurol.plot

import serial
import pylsl
import numpy as np

close_char = b'0'
open_char = b'1'
arduino_port = 'com7'

def close_hand(arduino_serial):
    arduino_serial.write(close_char)

def open_hand(arduino_serial):
    arduino_serial.write(open_char)


def classifier(emg, clb_info):
    '''classify if hand is closed'''
    return np.average(emg[-20:,0]) > 0.5

def classifier2(emg, clb_info):
    '''alternate classifier'''
    percent_above = np.average(emg[-20:0] > 0.5)
    return percent_above > 0.5

# TODO: try neurol's AutoMLBCI w/ TD-FD transformer

def action(state):
    '''sends commands to arduino to open or close prosthetic hand'''
    if state:
        close_hand(arduino)
        print('hand closed')
    else:
        open_hand(arduino)
        print('hand open')


if __name__ == '__main__':

    # connect to arduino via serial port
    print('Connecting Arduino')
    arduino = serial.Serial(arduino_port, baudrate=9600)

    print(arduino.readline()) # "arduino ready!"

    # make sure hand is open at start
    open_hand(arduino)


    # get EMG signal stream
    stream_info = neurol.connect_device.resolve_byprop('type', 'EMG')[0]
    inlet = pylsl.StreamInlet(stream_info)
    stream  = neurol.streams.lsl_stream(inlet, buffer_length=512)

    print(f'connected to stream \'{inlet.info().name()}\'')


    # create neurol BCI
    hand_bci = neurol.BCI.generic_BCI(classifier, action=action)

    # run prosthetic hand BCI
    try:
        hand_bci.run(stream)
    except KeyboardInterrupt:
        print('\n\n\nQUIT')
