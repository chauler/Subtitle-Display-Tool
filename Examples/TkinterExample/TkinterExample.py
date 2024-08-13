import socket
from tkinter import END, ttk, Frame, IntVar, Tk, Label, LabelFrame, LEFT, Entry, StringVar, Button, filedialog
import subprocess
import json
import os
import signal
import time

HOST = "localhost"
PORT = 9999
EXE_PATH = '..\\..\\bin\\Debug\\Subtitle-Display-Tool.exe'
FONT_DIR = "C:\\Windows\\Fonts"
FONT_EXTENSIONS = ('.ttf', '.otf')
DEFAULT_FONT_SIZE = "54"
DEFAULT_POSITION = "250"
DEFAULT_FONT_COLOR = "255"
DEFAULT_BG_COLOR = "0"
DEFAULT_OUTLINE_SIZE = "2.0"
DEFAULT_OUTLINE_COLOR = "0.0"
DEFAULT_DROP_SHADOW_COLOR = "0.0"
DEFAULT_DROP_SHADOW_OFFSET = "1"
DEFAULT_DROP_SHADOW_BLUR_STRENGTH = "1"

proc = subprocess.Popen([EXE_PATH, str(PORT)])

app = Tk()
app.title("Subtitle Display Tool")
app.geometry('450x850')

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

def limit_rate( delay=1.0 ):
    """ produces a decorator that will call a function only once per `delay` """
    def wrapper( func ): # the actual decorator
        cache = dict( next = 0 ) # cache the result and time
        def limited( *args, **kwargs):
            if time.time() > cache['next']: # is it time to call again
                cache['result'] = func( *args, **kwargs) # do the function
                cache['next'] = time.time() + delay # dont call before this time
            return cache['result']
        return limited
    return wrapper

@limit_rate(0.007)
def onResize(event):
    global sock
    if event.widget is not app:
        return
    data = {
        "mode": "command",
        "data": {
            "command": "UpdateWindowTransform",
            "x": event.x,
            "y": event.y,
            "width": event.width,
            "height": event.height
            }
        }
    try:
        sock.sendall(bytes(json.dumps(data), encoding="utf-8"))
    except ConnectionResetError:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        sock.sendall(bytes(json.dumps(data), encoding="utf-8"))
    finally:
        print("Sent: {}".format(data))

def clicked(data):
    global sock
    try:
        sock.sendall(bytes(json.dumps(data), encoding="utf-8"))
    except ConnectionResetError:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        sock.sendall(bytes(json.dumps(data), encoding="utf-8"))
    finally:
        print("Sent: {}".format(data))

def restart_connection():
    global proc
    if proc:
        os.kill(proc.pid, signal.SIGTERM)
    proc = subprocess.Popen([EXE_PATH, str(PORT)])
    print("Connection restarted")

def on_closing():
    if proc:
        os.kill(proc.pid, signal.SIGTERM)
    app.destroy()

def update_format_options(*args):
    selected_format = format_var.get()
    simple_frame.grid_remove()
    advanced_frame.grid_remove()
    file_frame.grid_remove()
    if selected_format == "Simple":
        simple_frame.grid(row=2, column=0, padx=10, pady=10, sticky='ew')
    elif selected_format == "Advanced":
        advanced_frame.grid(row=2, column=0, padx=10, pady=10, sticky='ew')
    elif selected_format == "File":
        file_frame.grid(row=2, column=0, padx=10, pady=10, sticky='ew')

class CollapsibleFrame(Frame):
    def __init__(self, parent, title="", *args, **kwargs):
        super().__init__(parent, *args, **kwargs)
        self.show = IntVar()
        self.show.set(0)
        self.title_frame = Frame(self)
        self.title_frame.pack(fill="x", expand=1)
        Label(self.title_frame, text=title).pack(side="left", fill="x", expand=1)
        self.toggle_button = ttk.Checkbutton(self.title_frame, width=2, text='+', command=self.toggle, variable=self.show, style='Toolbutton')
        self.toggle_button.pack(side="right")
        self.sub_frame = Frame(self, relief="sunken", borderwidth=1)

    def toggle(self):
        if self.show.get():
            self.sub_frame.pack(fill="x", expand=1, pady=2, padx=2)
            self.toggle_button.configure(text='-')
        else:
            self.sub_frame.forget()
            self.toggle_button.configure(text='+')

def create_label_entry(parent, text, default_value=""):
    frame = Frame(parent)
    frame.pack(fill="x", pady=5)
    Label(frame, text=text, width=20, anchor='w').pack(side=LEFT, padx=5)
    entry = Entry(frame)
    entry.pack(side=LEFT, fill="x", expand="yes", padx=5)
    entry.insert(0, default_value)
    return entry

def create_font_path_combobox(parent, text):
    frame = Frame(parent)
    frame.pack(fill="x", pady=5)
    Label(frame, text=text, width=20, anchor='w').pack(side=LEFT, padx=5)
    font_paths = [os.path.join(root, file) for root, dirs, files in os.walk(FONT_DIR) for file in files if file.endswith(FONT_EXTENSIONS)]
    combobox = ttk.Combobox(frame, values=font_paths)
    combobox.pack(side=LEFT, fill="x", expand="yes", padx=5)
    return combobox

def select_file(entry):
    file_path = filedialog.askopenfilename(filetypes=[("Subtitle Files(*.vtt)", "*.vtt"), ("Subtitle Files(*.ssa)", "*.ssa"), ("Subtitle Files(*.ass)", "*.ass"), ("Subtitle Files(*.srt)", "*.srt")])#;*.srt;*.ssa")])
    if file_path:
        entry.delete(0, END)
        entry.insert(0, file_path)

def get_style_data(fields):
    return {
        "fontSize": int(fields['font_size'].get()),
        "position": [
            int(fields['position_x'].get()), 
            int(fields['position_y'].get())
            ],
        "fontColor": [
            int(fields['font_color_r'].get()),
            int(fields['font_color_g'].get()),
            int(fields['font_color_b'].get()),
            int(fields['font_color_a'].get())
            ],
        "bgColor": [
            int(fields['bg_color_r'].get()),
            int(fields['bg_color_g'].get()),
            int(fields['bg_color_b'].get()),
            int(fields['bg_color_a'].get())
            ],
        "fontPath": fields['font_path'].get(),
        "outline": {
            "size": float(fields['outline_size'].get()),
            "color": [
                float(fields['outline_color_r'].get()),
                float(fields['outline_color_g'].get()),
                float(fields['outline_color_b'].get()),
                float(fields['outline_color_a'].get())
                ]
        },
        "shadow": {
            "color": [
                float(fields['drop_shadow_color_r'].get()),
                float(fields['drop_shadow_color_g'].get()),
                float(fields['drop_shadow_color_b'].get())
            ],
            "offset": [
                int(fields['drop_shadow_offset_x'].get()),
                int(fields['drop_shadow_offset_y'].get())
            ],
            "blurStrength": int(fields['drop_shadow_blur_strength'].get())
        }
    }

def create_style_fields(parent):
    fields = {}
    position_frame = CollapsibleFrame(parent, title="Position")
    fields['position_x'] = create_label_entry(position_frame.sub_frame, "Position X:", DEFAULT_POSITION)
    fields['position_y'] = create_label_entry(position_frame.sub_frame, "Position Y:", DEFAULT_POSITION)
    position_frame.pack(fill="x", padx=10, pady=5)

    font_frame = CollapsibleFrame(parent, title="Font")
    fields['font_size'] = create_label_entry(font_frame.sub_frame, "Font Size:", DEFAULT_FONT_SIZE)
    fields['font_path'] = create_font_path_combobox(font_frame.sub_frame, "Font Path:")
    font_frame.pack(fill="x", padx=10, pady=5)

    font_color_frame = CollapsibleFrame(parent, title="Font Color")
    fields['font_color_r'] = create_label_entry(font_color_frame.sub_frame, "Font Color R:", DEFAULT_FONT_COLOR)
    fields['font_color_g'] = create_label_entry(font_color_frame.sub_frame, "Font Color G:", DEFAULT_FONT_COLOR)
    fields['font_color_b'] = create_label_entry(font_color_frame.sub_frame, "Font Color B:", DEFAULT_FONT_COLOR)
    fields['font_color_a'] = create_label_entry(font_color_frame.sub_frame, "Font Color A:", DEFAULT_FONT_COLOR)
    font_color_frame.pack(fill="x", padx=10, pady=5)

    bg_color_frame = CollapsibleFrame(parent, title="Background Color")
    fields['bg_color_r'] = create_label_entry(bg_color_frame.sub_frame, "Background Color R:", DEFAULT_BG_COLOR)
    fields['bg_color_g'] = create_label_entry(bg_color_frame.sub_frame, "Background Color G:", DEFAULT_BG_COLOR)
    fields['bg_color_b'] = create_label_entry(bg_color_frame.sub_frame, "Background Color B:", DEFAULT_BG_COLOR)
    fields['bg_color_a'] = create_label_entry(bg_color_frame.sub_frame, "Background Color A:", DEFAULT_BG_COLOR)
    bg_color_frame.pack(fill="x", padx=10, pady=5)

    outline_frame = CollapsibleFrame(parent, title="Outline")
    fields['outline_size'] = create_label_entry(outline_frame.sub_frame, "Outline Size:", DEFAULT_OUTLINE_SIZE)
    fields['outline_color_r'] = create_label_entry(outline_frame.sub_frame, "Outline Color R:", DEFAULT_OUTLINE_COLOR)
    fields['outline_color_g'] = create_label_entry(outline_frame.sub_frame, "Outline Color G:", DEFAULT_OUTLINE_COLOR)
    fields['outline_color_b'] = create_label_entry(outline_frame.sub_frame, "Outline Color B:", DEFAULT_OUTLINE_COLOR)
    fields['outline_color_a'] = create_label_entry(outline_frame.sub_frame, "Outline Color A:", "1.0")
    outline_frame.pack(fill="x", padx=10, pady=5)

    drop_shadow_frame = CollapsibleFrame(parent, title="Drop Shadow")
    fields['drop_shadow_color_r'] = create_label_entry(drop_shadow_frame.sub_frame, "Shadow Color R:", DEFAULT_DROP_SHADOW_COLOR)
    fields['drop_shadow_color_g'] = create_label_entry(drop_shadow_frame.sub_frame, "Shadow Color G:", DEFAULT_DROP_SHADOW_COLOR)
    fields['drop_shadow_color_b'] = create_label_entry(drop_shadow_frame.sub_frame, "Shadow Color B:", DEFAULT_DROP_SHADOW_COLOR)
    fields['drop_shadow_offset_x'] = create_label_entry(drop_shadow_frame.sub_frame, "Shadow Offset X:", DEFAULT_DROP_SHADOW_OFFSET)
    fields['drop_shadow_offset_y'] = create_label_entry(drop_shadow_frame.sub_frame, "Shadow Offset Y:", DEFAULT_DROP_SHADOW_OFFSET)
    fields['drop_shadow_blur_strength'] = create_label_entry(drop_shadow_frame.sub_frame, "Blur Strength:", DEFAULT_DROP_SHADOW_BLUR_STRENGTH)
    drop_shadow_frame.pack(fill="x", padx=10, pady=5)

    return fields

format_var = StringVar()
format_var.set("Simple")
format_var.trace("w", update_format_options)

format_label = Label(app, text="Select Subtitle Format:")
format_label.grid(row=0, column=0, padx=10, pady=10)
format_menu = ttk.Combobox(app, textvariable=format_var, values=["Simple", "Advanced", "File"])
format_menu.grid(row=1, column=0, padx=10, pady=10)

simple_frame = LabelFrame(app, text="Simple Format Options", padx=10, pady=10)
simple_dialogue_label = Label(simple_frame, text="Dialogue:")
simple_dialogue_label.pack(fill="x")
simple_dialogue_entry = Entry(simple_frame)
simple_dialogue_entry.pack(fill="x", pady=5)
simple_btn = Button(simple_frame, text="Create Simple Window", command=lambda: clicked({
    "mode": "simple",
    "data": {"dialogue": simple_dialogue_entry.get()}
}))
simple_btn.pack(pady=10)

advanced_frame = LabelFrame(app, text="Advanced Format Options", padx=10, pady=10)
advanced_dialogue_label = Label(advanced_frame, text="Dialogue:")
advanced_dialogue_label.pack(fill="x")
advanced_dialogue_entry = Entry(advanced_frame)
advanced_dialogue_entry.pack(fill="x", pady=5)
advanced_fields = create_style_fields(advanced_frame)
advanced_lifetime_label = Label(advanced_frame, text="Lifetime:")
advanced_lifetime_label.pack(fill="x")
advanced_lifetime_entry = Entry(advanced_frame)
advanced_lifetime_entry.pack(fill="x", pady=5)
advanced_btn = Button(advanced_frame, text="Create Advanced Window", command=lambda: clicked({
    "mode": "advanced",
    "data": {
        "dialogue": advanced_dialogue_entry.get(),
        "styles": {**get_style_data(advanced_fields), "lifetime": float(advanced_lifetime_entry.get())}
    }
}))
advanced_btn.pack(pady=10)

file_frame = LabelFrame(app, text="File Format Options", padx=10, pady=10)
file_path_frame = Frame(file_frame)
file_path_frame.pack(fill="x", pady=5)
file_path_label = Label(file_path_frame, text="File Path:", width=20, anchor='w')
file_path_label.pack(side=LEFT, padx=5)
file_path_entry = Entry(file_path_frame)
file_path_entry.pack(side=LEFT, fill="x", expand="yes", padx=5)
file_path_button = Button(file_path_frame, text="Browse", command=lambda: select_file(file_path_entry))
file_path_button.pack(side=LEFT, padx=5)
file_fields = create_style_fields(file_frame)
file_btn = Button(file_frame, text="Load Subtitles from File", command=lambda: clicked({
    "mode": "file",
    "data": {"filePath": file_path_entry.get(), "styles": get_style_data(file_fields)}
}))
file_btn.pack(pady=10)
btn6 = Button(file_frame, text="Create Window showcasing WebVTT file support", command=lambda: clicked({
    "mode": "file",
    "data": {"filePath": "./test_files/example.vtt", "styles": get_style_data(file_fields)}
}))
btn6.pack(pady=10)

btn_restart = Button(app, text="Restart Connection", command=restart_connection)
btn_restart.grid(column=0, row=3, padx=10, pady=10)

simple_frame.grid(column=0, row=2, padx=10, pady=10, sticky='ew')

app.bind("<Configure>", onResize)

app.protocol("WM_DELETE_WINDOW", on_closing)

app.mainloop()
