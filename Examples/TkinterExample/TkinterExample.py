import socket
from tkinter import *
import subprocess
import json
import os
import signal

HOST = "localhost"
PORT = 9999

args = ['..\\..\\bin\\Debug\\Subtitle-Display-Tool.exe', str(PORT)]
proc = subprocess.Popen(args)

app = Tk()
app.title("Example Tkinter Host App")
app.geometry('350x800')

def clicked(data):
    #Send JSON to our program
    try:
        # Connect to server and send data
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        sock.sendall(bytes(json.dumps(data),encoding="utf-8"))

    finally:
        sock.close()
        
    print("Sent:     {}".format(data))

def restart_connection():
    global proc
    if(proc):
        # Terminates current process
        os.kill(proc.pid, signal.SIGTERM)
    # Starts a new process to establish a fresh connection
    proc = subprocess.Popen(args)
    print("Connection restated")

def on_closing():
    if proc:
        os.kill(proc.pid, signal.SIGTERM)
    app.destroy()
 
btn = Button(app, text = "Create window using simple mode", fg = "black", command=lambda: clicked({
        "mode": "simple",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App"
        }
    }))

btn2 = Button(app, text = "Create a stylized window with our API", fg = "black", command=lambda: clicked({
        "mode": "advanced",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App",
            "styles": {
                "fontSize": 36,
                "position": [250, 250],
			    "fontColor": [51, 255, 255, 255],
			    "bgColor": [102, 0, 51, 255],
                "lifetime": 5
            }
        }
    }))

btn3 = Button(app, text = "Create a transparent window with a custom font", fg = "black", command=lambda: clicked({
        "mode": "advanced",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App",
            "styles": {
                "fontSize": 500,
                "position": [250, 250],
			    "fontColor": [255, 255, 255, 255],
			    "bgColor": [102, 0, 51, 0],
			    "fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
                "lifetime": 5.0
            }
        }
    }))

btn4 = Button(app, text = "Create a window which lasts 0.5 seconds", fg = "black", command=lambda: clicked({
        "mode": "advanced",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App",
            "styles": {
                "fontSize": 36,
                "position": [250, 250],
			    "fontColor": [255, 255, 255, 255],
			    "bgColor": [102, 0, 51, 0],
			    "fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
                "lifetime": 0.5
            }
        }
    }))

btn5 = Button(app, text = "Create a window showcasing the outline and dropshadow shaders", fg = "black", command=lambda: clicked({
        "mode": "advanced",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App",
            "styles": {
                "fontSize": 500,
                "position": [250, 250],
			    "fontColor": [0, 255, 0, 200],
			    "bgColor": [0, 0, 255, 255],
			    "fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
                "lifetime": 5.0,
                "outline": {
                    "size": 2,
                    "color": [0.0, 0.4, 0.3, 1.0]
                    }
            }
        }
    }))

# Temporary workaround to reset the socket connection
btn_restart = Button(app, text="Restart Connection", fg="black", command=restart_connection)

btn.grid(column=0, row=0)
btn2.grid(column=0, row=1)
btn3.grid(column=0, row=2)
btn4.grid(column=0, row=3)
btn5.grid(column=0, row=4)

btn_restart.grid(column=0, row=5)

app.protocol("WM_DELETE_WINDOW", on_closing)
app.mainloop()
