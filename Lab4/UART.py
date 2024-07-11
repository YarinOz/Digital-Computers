import sys
import serial as ser
import time

state = '7'
j = 0

def receive_data(serial_comm, enableTX, delay=0.25):
    global state
    # RX
    while (serial_comm.in_waiting > 0):  # while the input buffer isn't empty
        menu = serial_comm.read_until(expected ='\r').decode("ascii")
        print(menu)
        enableTX = True
        time.sleep(delay)  # delay for accurate read/write operations on both ends
    return enableTX


def transmit_data(serial_comm, enableTX, delay=0.25):
    while (serial_comm.out_waiting > 0 or enableTX):  # while the output buffer isn't empty
        global state
        state = input(" enter menu option: ")
        serial_comm.write(bytes(state, 'ascii'))
        time.sleep(delay)  # delay for accurate read/write operations on both
        enableTX = True

        if (state == '4'):
            x = input("enter new delay: ")
            serial_comm.write(bytes(x + '\0', 'ascii'))
            time.sleep(delay)  # delay for accurate read/write operations on both ends
        while (1):
            # RX
            enableTX = receive_data(serial_comm, enableTX)
            # TX
            enableTX = transmit_data(serial_comm, enableTX)
        if (serial_comm.out_waiting == 0):
            return False

    return enableTX


def start_communication():
    serial_comm = ser.Serial('COM37', baudrate=9600, bytesize=ser.EIGHTBITS,
                             parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                             timeout=1)
    serial_comm.reset_input_buffer()
    serial_comm.reset_output_buffer()
    enableTX = False
    while (1):
        # RX
        enableTX = receive_data(serial_comm, enableTX)
        # TX
        enableTX = transmit_data(serial_comm, enableTX)


def main():
    start_communication()


if __name__ == "__main__":
    main()