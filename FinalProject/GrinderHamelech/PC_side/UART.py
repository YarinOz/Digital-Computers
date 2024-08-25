import tkinter as tk
from tkinter import colorchooser, filedialog, messagebox, ttk
import os


class Paint:
    DEFAULT_PEN_SIZE = 5.0
    DEFAULT_COLOR = 'black'

    def __init__(self, master):
        self.master = master
        self.top = tk.Toplevel(master)
        self.top.title("Painter")
        self.top.geometry("800x600")

        self.center_window(self.top, 800, 600)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Arial', 12), padding=6, background='#4CAF50', foreground='white')
        self.style.configure('TScale', font=('Arial', 12))
        self.style.configure('TLabel', font=('Arial', 12))

        # Use standard tk.Button instead of ttk.Button
        self.pen_button = tk.Button(self.top, text='Pen', command=self.use_pen, relief=tk.RAISED)
        self.pen_button.grid(row=0, column=0, padx=5, pady=5)

        self.eraser_button = tk.Button(self.top, text='Erase', command=self.use_eraser, relief=tk.RAISED)
        self.eraser_button.grid(row=0, column=1, padx=5, pady=5)

        self.brush_button = tk.Button(self.top, text='Back', command=self.close_painter, relief=tk.RAISED)
        self.brush_button.grid(row=0, column=3, padx=5, pady=5)

        self.choose_size_button = ttk.Scale(self.top, from_=1, to=20, orient=tk.HORIZONTAL)
        self.choose_size_button.set(5)
        self.choose_size_button.grid(row=0, column=4, padx=5, pady=5)

        self.c = tk.Canvas(self.top, bg='white', width=600, height=400)
        self.c.grid(row=1, columnspan=5, padx=5, pady=5)

        self.setup()

    def setup(self):
        self.old_x = None
        self.old_y = None
        self.line_width = self.choose_size_button.get()
        self.color = self.DEFAULT_COLOR
        self.eraser_on = False
        self.active_button = self.pen_button
        self.c.bind('<B1-Motion>', self.paint)
        self.c.bind('<ButtonRelease-1>', self.reset)

    def use_pen(self):
        self.activate_button(self.pen_button)

    def use_eraser(self):
        self.activate_button(self.eraser_button, eraser_mode=True)

    def activate_button(self, some_button, eraser_mode=False):
        self.active_button.config(relief=tk.RAISED)
        some_button.config(relief=tk.SUNKEN)
        self.active_button = some_button
        self.eraser_on = eraser_mode

    def paint(self, event):
        paint_color = 'white' if self.eraser_on else self.color
        if self.old_x and self.old_y:
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
        self.style.configure('TButton', font=('Arial', 12), padding=6, background='#4CAF50', foreground='white')
        self.style.configure('TLabel', font=('Arial', 12))

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
        self.style.configure('TButton', font=('Arial', 12), padding=6, background='#4CAF50', foreground='white')
        self.style.configure('TLabel', font=('Arial', 12))

        # Layout
        self.label = ttk.Label(self.top, text="Stepper Motor Calibration", font=('Arial', 14))
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
        self.style.configure('TButton', font=('Arial', 12), padding=6, background='#4CAF50', foreground='white')
        self.style.configure('TLabel', font=('Arial', 12))

        # Layout
        self.label = ttk.Label(self.top, text="Manual Control of Motor-Based Machine", font=('Arial', 14))
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


class MainApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Control System of Motor-Based Machine")
        self.root.geometry("400x300")

        self.center_window(self.root, 400, 300)

        # Style
        self.style = ttk.Style()
        self.style.configure('TButton', font=('Arial', 12), padding=6, background='#4CAF50', foreground='white')
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


if __name__ == '__main__':
    root = tk.Tk()
    app = MainApp(root)
    root.mainloop()
