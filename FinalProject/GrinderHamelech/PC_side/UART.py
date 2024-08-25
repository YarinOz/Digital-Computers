import tkinter as tk
from tkinter import colorchooser, filedialog, messagebox, ttk
import os
import serial as ser
import serial.tools.list_ports
import binascii


class Paint:
    DEFAULT_PEN_SIZE = 5.0
    DEFAULT_COLOR = 'black'
    STATE_PEN = 0
    STATE_ERASE = 1
    STATE_NEUTRAL = 2

    def __init__(self, master):
        self.master = master
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

        self.setup()

    def setup(self):
        self.old_x = None
        self.old_y = None
        self.line_width = self.choose_size_button.get()
        self.color = self.DEFAULT_COLOR
        self.eraser_on = False

        # Bind the mouse movement to the mouse_move method
        self.c.bind('<Motion>', self.mouse_move)

        # Periodically check the state
        self.update_state()

    def update_state(self):
        global pstate
        # pstate = self.read_state_from_serial()  # Read the state from the serial port
        pstate = 2
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
        try:
            if serial_comm.in_waiting > 0:
                pstate = serial_comm.readline().decode('utf-8').strip()
                return pstate
        except Exception as e:
            print(f"Error reading from serial: {e}")
        return self.STATE_NEUTRAL

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

        self.center_window(self.top, 800, 600)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Helvetica', 12), padding=6, background='#4CAF50', foreground='black')
        self.style.configure('TLabel', font=('Helvetica', 12))

        # Layout
        self.file_listbox = tk.Listbox(self.top, width=50)
        self.file_listbox.grid(row=0, column=0, padx=10, pady=10, rowspan=4)

        self.select_button = ttk.Button(self.top, text="Select File", command=self.select_file)
        self.select_button.grid(row=0, column=1, padx=10, pady=10)

        self.execute_button = ttk.Button(self.top, text="Execute Script", command=self.execute_script)
        self.execute_button.grid(row=1, column=1, padx=10, pady=10)

        self.file_label = ttk.Label(self.top, text="No file selected")
        self.file_label.grid(row=2, column=1, padx=10, pady=10)

        self.content_text = tk.Text(self.top, width=60, height=20)
        self.content_text.grid(row=3, column=1, padx=10, pady=10)

        # Add a "Back" button
        self.back_button = ttk.Button(self.top, text="Back", command=self.close_script_mode)
        self.back_button.grid(row=4, column=1, padx=10, pady=10)

    def select_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
        if file_path:
            self.load_file(file_path)

    def load_file(self, file_path):
        self.file_label.config(text=os.path.basename(file_path))
        with open(file_path, 'r') as file:
            content = file.read()
            self.content_text.delete(1.0, tk.END)
            self.content_text.insert(tk.END, content)

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

        self.back_button = ttk.Button(self.top, text="Back", command=self.close_calibration)
        self.back_button.pack(pady=5)

    def start_calibration(self):
        # Placeholder for actual calibration logic
        print("Starting calibration...")

    def stop_calibration(self):
        # Placeholder for actual calibration logic
        print("Stopping calibration...")

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

    def start_motor(self):
        # Placeholder for starting motor logic
        print("Motor started.")

    def stop_motor(self):
        # Placeholder for stopping motor logic
        print("Motor stopped.")

    def toggle_joystick_mode(self):
        # Toggle joystick mode
        self.joystick_mode = not self.joystick_mode
        print(f"Joystick Mode {'enabled' if self.joystick_mode else 'disabled'}.")

    def close_manual_control(self):
        self.top.destroy()

    def center_window(self, window, width, height):
        screen_width = window.winfo_screenwidth()
        screen_height = window.winfo_screenheight()
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        window.geometry(f"{width}x{height}+{x}+{y}")


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


# Automatic PORT search
class PortError(Exception):
    """Raised when the port not found"""
    pass


def port_search(between=None):
    # find the right com that connect between the pc and controller
    ports = serial.tools.list_ports.comports()
    for desc in sorted(ports):
        if "MSP430" in desc.description:
            return desc.device
    raise PortError


if __name__ == '__main__':
    # Automatic port search
    port = port_search()
    serial_comm = ser.Serial(port, baudrate=9600, bytesize=ser.EIGHTBITS,
                             parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                             timeout=1)
    serial_comm.flush() # Flush input/output buffer
    serial_comm.set_buffer_size(rx_size=1024, tx_size=1024)
    serial_comm.reset_input_buffer()
    serial_comm.reset_output_buffer()

    root = tk.Tk()
    app = MainApp(root)
    root.mainloop()
