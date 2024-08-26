import tkinter as tk
from tkinter import colorchooser, filedialog, messagebox, ttk
import os
import serial as ser
import serial.tools.list_ports
import binascii
import threading

serial_comm = None

# Instruction mapping with their corresponding opcode
instruction_map = {
    "inc_lcd": "01",
    "dec_lcd": "02",
    "rra_lcd": "03",
    "set_delay": "04",
    "clear_lcd": "05",
    "stepper_deg": "06",
    "stepper_scan": "07",
    "sleep": "08"
}


def translate_line(line):
    parts = line.strip().split()
    instruction = parts[0]
    operands = parts[1:]  # Capture operands after the instruction

    # Fetch the opcode for the instruction
    opcode = instruction_map[instruction]

    # Handle the specific operand requirements
    if instruction == "rra_lcd" and operands:
        operand = f'{ord(operands[0]):02X}'  # Convert to uppercase
    elif instruction == "stepper_scan" and operands:
        # Split the operands on the comma to get two separate values
        left_operand, right_operand = operands[0].split(',')
        operand = f'{int(left_operand):02X}{int(right_operand):02X}'  # Convert to uppercase
    elif operands:
        operand = f'{int(operands[0]):02X}'  # Convert to uppercase
    else:
        operand = ""

    # Combine opcode and operand
    return opcode + operand


def translate_script(input_file):
    translated_content = []
    with open(input_file, 'r') as infile:
        for line in infile:
            encoded_line = translate_line(line)
            translated_content.append(encoded_line)
    return '\n'.join(translated_content)


class Paint:
    DEFAULT_PEN_SIZE = 5.0
    DEFAULT_COLOR = 'black'
    STATE_PEN = 0
    STATE_ERASE = 1
    STATE_NEUTRAL = 2

    def __init__(self, master):
        self.master = master
        # self.serial_comm = serial_comm  # Pass the serial communication object
        self.top = tk.Toplevel(master)
        self.top.title("Painter")
        self.top.geometry("800x600")

        self.center_window(self.top, 800, 600)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TScale', font=('Helvetica', 12))
        self.style.configure('TLabel', font=('Helvetica', 12))

        self.choose_size_button = ttk.Scale(self.top, from_=1, to=20, orient=tk.HORIZONTAL)
        self.choose_size_button.set(5)
        self.choose_size_button.grid(row=0, column=0, padx=5, pady=5)

        self.c = tk.Canvas(self.top, bg='white', width=600, height=400)
        self.c.grid(row=1, columnspan=5, padx=5, pady=5)

        PaintActive = 1
        # send_state('P')  # Painter

        self.setup()
        # self.read_state_from_serial()

    def setup(self):
        self.old_x = None
        self.old_y = None
        self.line_width = self.choose_size_button.get()
        self.color = self.DEFAULT_COLOR
        self.eraser_on = False

        # Bind the mouse movement to the mouse_move method
        self.c.bind('<Motion>', self.mouse_move)

        self.update_state()

    def update_state(self):
        global pstate
        # pstate = self.read_state_from_serial()  # Read the state from the serial port
        pstate=0
        if pstate == self.STATE_PEN:
            self.eraser_on = False
            self.color = self.DEFAULT_COLOR
        elif pstate == self.STATE_ERASE:
            self.eraser_on = True
            self.color = 'white'
        else:
            self.eraser_on = False
        self.master.after(100, self.update_state)  # Check every 100ms

    def read_state_from_serial(self):
        # Read state from serial communication
        def worker():
            try:
                while True:
                    if serial_comm.in_waiting > 0:
                        pstate = serial_comm.readline().decode('utf-8').strip()
                        return int(pstate)
            except Exception as e:
                print(f"Error reading from serial: {e}")
                return self.STATE_NEUTRAL
        thread = threading.Thread(target=worker, daemon=True)
        thread.start()

    def mouse_move(self, event):
        if pstate != 2 and self.old_x is not None and self.old_y is not None:
            paint_color = 'white' if self.eraser_on else self.color
            self.c.create_line(self.old_x, self.old_y, event.x, event.y,
                               width=self.line_width, fill=paint_color,
                               capstyle=tk.ROUND, smooth=tk.TRUE)
        self.old_x = event.x
        self.old_y = event.y

    def reset(self, event):
        self.old_x, self.old_y = None, None

    def close_painter(self):
        self.top.destroy()

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


class ScriptMode:
    def __init__(self, master):
        self.master = master
        self.top = tk.Toplevel(master)
        self.top.title("Script Mode")
        self.top.geometry("800x600")

        self.center_window(self.top, 600, 600)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TLabel', font=('Helvetica', 12))

        # Layout
        self.file_label = ttk.Label(self.top, text="No file selected")
        self.file_label.grid(row=0, column=0, padx=10, pady=10, columnspan=2)

        # Original Script Text Widget
        self.original_text_label = ttk.Label(self.top, text="Original Script")
        self.original_text_label.grid(row=1, column=0, padx=10, pady=5)
        self.original_text = tk.Text(self.top, width=30, height=20)
        self.original_text.grid(row=2, column=0, padx=10, pady=10)

        # Translated Script Text Widget
        self.translated_text_label = ttk.Label(self.top, text="Translated Script")
        self.translated_text_label.grid(row=1, column=1, padx=10, pady=5)
        self.translated_text = tk.Text(self.top, width=30, height=20)
        self.translated_text.grid(row=2, column=1, padx=10, pady=10)

        # Buttons
        self.select_button = ttk.Button(self.top, text="Select File", command=self.select_file)
        self.select_button.grid(row=3, column=0, padx=10, pady=10)

        self.execute_button = ttk.Button(self.top, text="Execute Script", command=self.execute_script)
        self.execute_button.grid(row=3, column=1, padx=10, pady=10)

        # Add a "Back" button
        self.back_button = ttk.Button(self.top, text="Back", command=self.close_script_mode)
        self.back_button.grid(row=4, column=0, columnspan=2, padx=10, pady=10)
        
        burn_index = 0
        self.execute_serial_command("s")  # Send script state

    def execute_serial_command(self, command):
        thread = threading.Thread(target=serial_write, args=(bytes(command, 'ascii'),))
        thread.start()

    def select_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
        if file_path:
            self.load_file(file_path)

    def load_file(self, file_path):
        self.file_label.config(text=os.path.basename(file_path))

        # Load and display the original script
        with open(file_path, 'r') as file:
            original_content = file.read()
            self.original_text.delete(1.0, tk.END)
            self.original_text.insert(tk.END, original_content)

        # Translate the script and display the translated content
        translated_content = translate_script(file_path)
        self.translated_text.delete(1.0, tk.END)
        self.translated_text.insert(tk.END, translated_content)

    def execute_script(self):
        script_content = self.content_text.get(1.0, tk.END).strip()
        if script_content:
            print("Executing script:\n\n" + script_content)
        else:
            print("No script content to execute.")

    def close_script_mode(self):
        self.top.destroy()

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


class CalibrationMode:
    def __init__(self, master):
        self.master = master
        self.top = tk.Toplevel(master)
        self.top.title("Stepper Motor Calibration")
        self.top.geometry("400x300")

        self.center_window(self.top, 400, 300)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TLabel', font=('Helvetica', 12))

        # Layout
        self.label = ttk.Label(self.top, text="Stepper Motor Calibration", font=('Helvetica', 14))
        self.label.pack(pady=10)

        self.start_button = ttk.Button(self.top, text="Start Calibration", command=self.start_calibration)
        self.start_button.pack(pady=5)

        self.stop_button = ttk.Button(self.top, text="Stop Calibration", command=self.stop_calibration)
        self.stop_button.pack(pady=5)

        self.counter_label = ttk.Label(self.top, text="Counter: N/A")
        self.counter_label.pack(pady=5)

        self.phi_label = ttk.Label(self.top, text="Phi: N/A [deg]")
        self.phi_label.pack(pady=5)

        self.back_button = ttk.Button(self.top, text="Back", command=self.close_calibration)
        self.back_button.pack(pady=5)

        self.execute_serial_command("C")  # Send calibration state

    def execute_serial_command(self, command):
        thread = threading.Thread(target=serial_write, args=(bytes(command, 'ascii'),))
        thread.start()

    def start_calibration(self):
        # Placeholder for actual calibration logic
        print("Starting calibration...")
        self.execute_serial_command("A")

    def stop_calibration(self):
        # Placeholder for actual calibration logic
        print("Stopping calibration...")
        self.execute_serial_command("M")
        threading.Thread(target=self.read_counter_value, daemon=True).start()

    def read_counter_value(self):
        while True:
            if serial_comm.in_waiting > 0:
                try:
                    # Read the line and strip any unwanted characters
                    raw_data = serial_comm.readline().decode('utf-8').strip()

                    # Remove non-numeric characters and null bytes
                    cleaned_data = ''.join(filter(str.isdigit, raw_data))

                    if cleaned_data:
                        # Convert to integer and compute phi
                        counter_int = int(cleaned_data)
                        phi = counter_int / 360

                        # Update the labels on the GUI
                        self.master.after(0, self.update_labels, cleaned_data, round(phi, 4))
                    break
                except ValueError as e:
                    print(f"Error converting counter value: {e}")
                except Exception as e:
                    print(f"Error reading counter value: {e}")
                break  # Exit the loop if there's an error

    def update_labels(self, counter, phi):
        self.counter_label.config(text=f"Counter: {counter}")
        self.phi_label.config(text=f"Phi: {phi} [deg]")

    def close_calibration(self):
        self.top.destroy()

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


class ManualControl:
    def __init__(self, master):
        self.master = master
        self.top = tk.Toplevel(master)
        self.top.title("Manual Control")
        self.top.geometry("400x300")

        self.center_window(self.top, 400, 300)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TLabel', font=('Helvetica', 12))

        # Layout
        self.label = ttk.Label(self.top, text="Manual Control of Motor-Based Machine", font=('Helvetica', 14))
        self.label.pack(pady=10)

        self.start_button = ttk.Button(self.top, text="Start Motor", command=self.start_motor)
        self.start_button.pack(pady=5)

        self.stop_button = ttk.Button(self.top, text="Stop Motor", command=self.stop_motor)
        self.stop_button.pack(pady=5)

        self.joystick_button = ttk.Button(self.top, text="Joystick Mode", command=self.toggle_joystick_mode)
        self.joystick_button.pack(pady=5)

        self.back_button = ttk.Button(self.top, text="Back", command=self.close_manual_control)
        self.back_button.pack(pady=5)

        self.joystick_mode = False

        self.execute_serial_command("m")  # Send manual stepper state

    def start_motor(self):
        # Placeholder for starting motor logic
        print("Motor started.")
        self.execute_serial_command("A")

    def stop_motor(self):
        # Placeholder for stopping motor logic
        print("Motor stopped.")
        self.execute_serial_command("M")

    def toggle_joystick_mode(self):
        # Toggle joystick mode
        self.joystick_mode = not self.joystick_mode
        print(f"Joystick Mode {'enabled' if self.joystick_mode else 'disabled'}.")
        mode = "J" if self.joystick_mode else "M"
        self.execute_serial_command(mode)

    def execute_serial_command(self, command):
        thread = threading.Thread(target=serial_write, args=(bytes(command, 'ascii'),))
        thread.start()

    def close_manual_control(self):
        self.top.destroy()

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


def serial_write(message):
    global serial_comm
    try:
        if serial_comm and serial_comm.is_open:
            serial_comm.write(message)
        else:
            print("Serial port not open.")
    except Exception as e:
        print(f"Error writing to serial port: {e}")

# Communication
def send_state(message=None, file_option=False):
    serial_comm.reset_output_buffer()
    if file_option:
        bytesMenu = message
    else:
        bytesMenu = bytes(message, 'ascii')
    serial_comm.write(bytesMenu)


def read_from_MSP(state, size):
    n = 4
    while True:
        while serial_comm.in_waiting > 0:  # while the input buffer isn't empty
            if state == 'Painter':
                message = serial_comm.read(size=size)  # at Painter size = 6
                message = binascii.hexlify(message).decode('utf-8')
                message_split = "".join([message[x:x + 2] for x in range(0, len(message), 2)][::-1])
                final_message = [message_split[i:i + n] for i in range(0, len(message_split), n)]
            elif state == 'script':
                #        try:
                final_message = serial_comm.read(size=size).decode('utf-8')  # at Painter size = 4
            #        except:
            #           final_message = s.read()
            #           print(final_message)
            #           print("error")
            # final_message = s.readline().decode('utf-8')
            else:
                final_message = serial_comm.readline().decode('utf-8')
                # final_message = s.readline()
                # int(binascii.hexlify(message), 16)
                # int((final_message[0]), 16) - convert hex to int

            #  print(final_message)
            return final_message
    # except:
    #
    # return "Error"


def joysticktelemetry():
    global state
    n = 4
    while True:
        while serial_comm.in_waiting > 0:  # while the input buffer isn't empty
            message = serial_comm.read(size=4)  # at Painter size = 6
            message = binascii.hexlify(message).decode('utf-8')
            message_split = "".join([message[x:x + 2] for x in range(0, len(message), 2)][::-1])
            telem = [message_split[i:i + n] for i in range(0, len(message_split), n)]
        break
    Vx = int((telem[0]), 16)
    Vy = int((telem[1]), 16)
    if Vx > 1024 or Vy > 1024:
        telem[0] = "".join([telem[0][x:x + 2] for x in range(0, len(telem[0]), 2)][::-1])
        telem[1] = "".join([telem[1][x:x + 2] for x in range(0, len(telem[1]), 2)][::-1])
        Vx = int((telem[0]), 16)
        Vy = int((telem[1]), 16)

    print("Vx: " + str(Vx) + ", Vy: " + str(Vy) + ", state: " + str(state))

    return Vx, Vy


def message_handler(message=None, FSM=False, file=False):
    serial_comm.reset_output_buffer()
    txChar = message  # enter key to transmit
    if FSM:
        serial_comm.write(b"\x7f")
    if not file:
        bytesChar = bytes(txChar, 'ascii')
    else:
        bytesChar = txChar
    serial_comm.write(bytesChar)
    print(f"cp send: {message}")
#----------------------------------------------------------------------------------------------------#
# Serial Communication UART initialization
class PortError(Exception):
    """Raised when the port not found"""
    pass

class MainApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Control System of Motor-Based Machine")
        self.root.geometry("400x300")

        self.center_window(self.root, 400, 300)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TLabel', font=('Verdana', 15))

        # Main layout
        self.main_frame = ttk.Frame(self.root, padding=10)
        self.main_frame.pack(fill=tk.BOTH, expand=True)

        self.label = ttk.Label(self.main_frame, text="Final Project by Yarin and Omer", font=('Verdana', 15))
        self.label.pack(pady=10)

        self.manual_button = ttk.Button(self.main_frame, text="Manual Control of Motor-Based Machine",
                                        command=self.show_manual)
        self.manual_button.pack(pady=5)

        self.painter_button = ttk.Button(self.main_frame, text="Joystick Based PC Painter", command=self.show_painter)
        self.painter_button.pack(pady=5)

        self.calib_button = ttk.Button(self.main_frame, text="Stepper Motor Calibration", command=self.show_calib)
        self.calib_button.pack(pady=5)

        self.script_button = ttk.Button(self.main_frame, text="Script Mode", command=self.show_script)
        self.script_button.pack(pady=5)
    
    def show_manual(self):
        ManualControl(self.root)

    def show_painter(self):
        Paint(self.root)

    def show_calib(self):
        CalibrationMode(self.root)

    def show_script(self):
        ScriptMode(self.root)

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


def port_search():
    # find the right com that connect between the pc and controller
    ports = serial.tools.list_ports.comports()
    for desc in sorted(ports):
        if "MSP430" in desc.description:
            return desc.device
    raise PortError


def initialize_serial_comm():
    global serial_comm
    try:
        port = port_search()
        serial_comm = ser.Serial(port, baudrate=9600, bytesize=ser.EIGHTBITS,
                                 parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                                 timeout=1)
        serial_comm.flush()
        serial_comm.set_buffer_size(rx_size=1024, tx_size=1024)
        serial_comm.reset_input_buffer()
        serial_comm.reset_output_buffer()
    except PortError:
        messagebox.showerror("Port Error", "MSP430 Port not found.")
        sys.exit()


if __name__ == '__main__':
    initialize_serial_comm()

    root = tk.Tk()
    app = MainApp(root)
    root.mainloop()
