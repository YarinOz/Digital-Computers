import serial as ser
import time
import tkinter as tk
from tkinter import simpledialog

state = '7'

def receive_data(serial_comm):
    global state
    enableTX = False
    if serial_comm.in_waiting > 0:
        received_char = serial_comm.read_until(b'$').decode("ascii").strip()
        print(f"Received: {received_char}")
        enableTX = True  # Enable TX after receiving data
        time.sleep(0.25)
    return enableTX

def transmit_data(serial_comm):
    global state
    if state:
        state = input("Enter option: ")
        if 0 <= int(state) <= 8:
            serial_comm.write(bytes(state, 'ascii'))
            time.sleep(0.25)
            if state == '4':
                x = input("Enter new delay: ")
                serial_comm.write(bytes(x + '\n', 'ascii'))  # Send delay with newline
                time.sleep(0.25)  # Delay for accurate read/write operations on both ends
        else:
            print("Invalid input")
    return False  # Always return False to prevent indefinite looping

def button_1_func():
    print("Button 1 pressed")

def button_2_func():
    print("Button 2 pressed")

def button_3_func():
    print("Button 3 pressed")

def button_4_func():
    print("Button 4 pressed")

def ComGUI():
    root = tk.Tk()
    root.title("Serial Communication GUI")

    # Set window size
    root.geometry("500x400")

    # Set background color
    root.configure(bg='#ADD8E6')  # Light blue

    # Add title
    title = tk.Label(root, text="DCS Final Project", bg='#ADD8E6', fg='black', font=('Helvetica', 16, 'bold'))
    title.pack(pady=20)

    # Button width and padding
    button_width = 30
    button_padx = 30

    button_1 = tk.Button(root, text="Stepper Motor Manual Control", command=button_1_func, bg='#FF6347', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Tomato red
    button_1.pack(pady=10)

    button_2 = tk.Button(root, text="Analog Joystick \"Paint\"", command=button_2_func, bg='#32CD32', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Lime green
    button_2.pack(pady=10)

    button_3 = tk.Button(root, text="Stepper Motor Calibration", command=button_3_func, bg='#FFD700', fg='black', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Gold
    button_3.pack(pady=10)

    button_4 = tk.Button(root, text="Script Mode", command=button_4_func, bg='#1E90FF', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Dodger blue
    button_4.pack(pady=10)

    title = tk.Label(root, text="Yarin Oziel & Omer Kariti", bg='#ADD8E6', fg='black', font=('Helvetica', 12, 'bold'))
    title.pack(pady=20)

    root.mainloop()

def start_communication():
    # serial_comm = ser.Serial('COM19', baudrate=9600, bytesize=ser.EIGHTBITS,
    #                          parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
    #                          timeout=1)
    # serial_comm.reset_input_buffer()
    # serial_comm.reset_output_buffer()

    # while True:
    #     # RX
    #     if receive_data(serial_comm):
    #         continue

    #     # TX
    #     if transmit_data(serial_comm):
    #         continue
    
    ComGUI()

def main():
    start_communication()

if __name__ == "__main__":
    main()
