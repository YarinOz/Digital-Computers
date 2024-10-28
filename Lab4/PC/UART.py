import serial as ser
import time

state = '8'

def receive_data(serial_comm):
    global state
    enableTX = False
    if serial_comm.in_waiting > 0:
        received_char = serial_comm.read_until(b'$').decode("ascii").strip()
        print(received_char)
        enableTX = True  # Enable TX after receiving data
        time.sleep(0.25)
    return enableTX

def transmit_data(serial_comm):
    global state
    global PB
    if state:
        if state=='7':
            time.sleep(5)
            state = '8'
            return False
        state = input("Enter option: ")
        if 0 <= int(state) <= 9:
            serial_comm.write(bytes(state, 'ascii'))
            time.sleep(0.25)
            if state == '4':
                x = input("Enter new delay: ")
                serial_comm.write(bytes(x + '\n', 'ascii'))  # Send delay with newline
                time.sleep(0.25)  # Delay for accurate read/write operations on both ends
        else:
            print("Invalid input")
    return False  # Always return False to prevent indefinite looping

def start_communication():
    serial_comm = ser.Serial('COM19', baudrate=9600, bytesize=ser.EIGHTBITS,
                             parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                             timeout=1)
    serial_comm.reset_input_buffer()
    serial_comm.reset_output_buffer()

    while True:
        # RX
        if receive_data(serial_comm):
            continue

        # TX
        if transmit_data(serial_comm):
            continue

def main():
    start_communication()

if __name__ == "__main__":
    main()
