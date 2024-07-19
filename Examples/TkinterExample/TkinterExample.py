import socket
from tkinter import *
import subprocess
import json
HOST = "localhost"
PORT = 9999

args = ['..\\..\\bin\\Debug\\Subtitle-Display-Tool.exe', str(PORT)]
subprocess.Popen(args)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

app = Tk()
app.title("Example Tkinter Host App")
app.geometry('350x800')

def clicked(data):
    #Send JSON to our program
    try:
        # Connect to server and send data
        sock.connect((HOST, PORT))
        sock.sendall(bytes(json.dumps(data),encoding="utf-8"))

    finally:
        sock.close()
        
    print("Sent:     {}".format(data))
 
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
                "lifetime": 0
            }
        }
    }))

btn3 = Button(app, text = "Create a transparent window with a custom font", fg = "black", command=lambda: clicked({
        "mode": "advanced",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App",
            "styles": {
                "fontSize": 36,
                "position": [250, 250],
			    "fontColor": [255, 255, 255, 255],
			    "bgColor": [102, 0, 51, 0],
			    "fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
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

btn.grid(column=0, row=0)
btn2.grid(column=0, row=1)
btn3.grid(column=0, row=2)
btn4.grid(column=0, row=3)

app.mainloop()
