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
    print("Calibrating stepper motor...")
    calibration_window.deiconify()
    main_window.withdraw()

def button_4_func():
    print("Button 4 pressed")

def start_calibration():
    print("Calibration started")

def stop_calibration():
    print("Calibration stopped")

def back_to_main():
    calibration_window.withdraw()
    main_window.deiconify()

def ComGUI():
    global main_window, calibration_window

    main_window = tk.Tk()
    main_window.title("Serial Communication GUI")
    main_window.geometry("500x400")
    main_window.configure(bg='#ADD8E6')  # Light blue

    title = tk.Label(main_window, text="DCS Final Project", bg='#ADD8E6', fg='black', font=('Helvetica', 16, 'bold'))
    title.pack(pady=20)

    button_width = 30
    button_padx = 30

    button_1 = tk.Button(main_window, text="Stepper Motor Manual Control", command=button_1_func, bg='#FF6347', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Tomato red
    button_1.pack(pady=10)

    button_2 = tk.Button(main_window, text="Analog Joystick \"Paint\"", command=button_2_func, bg='#32CD32', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Lime green
    button_2.pack(pady=10)

    button_3 = tk.Button(main_window, text="Stepper Motor Calibration", command=button_3_func, bg='#FFD700', fg='black', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Gold
    button_3.pack(pady=10)

    button_4 = tk.Button(main_window, text="Script Mode", command=button_4_func, bg='#1E90FF', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Dodger blue
    button_4.pack(pady=10)

    title = tk.Label(main_window, text="Yarin Oziel & Omer Kariti", bg='#ADD8E6', fg='black', font=('Helvetica', 12, 'bold'))
    title.pack(pady=20)

    calibration_window = tk.Toplevel(main_window)
    calibration_window.title("Stepper Motor Calibration")
    calibration_window.geometry("500x400")
    calibration_window.configure(bg='#ADD8E6')
    calibration_window.withdraw()

    start_button = tk.Button(calibration_window, text="Start", command=start_calibration, bg='#32CD32', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Lime green
    start_button.pack(pady=20)

    stop_button = tk.Button(calibration_window, text="Stop", command=stop_calibration, bg='#FF6347', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Tomato red
    stop_button.pack(pady=20)

    back_button = tk.Button(calibration_window, text="Back to Main Menu", command=back_to_main, bg='#1E90FF', fg='white', font=('Helvetica', 12, 'bold'), width=button_width, padx=button_padx)  # Dodger blue
    back_button.pack(pady=20)

    main_window.mainloop()

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
