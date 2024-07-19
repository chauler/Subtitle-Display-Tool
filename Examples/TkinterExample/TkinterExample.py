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

def clicked():
    data = {
        "mode": "simple",
        "data": {
            "dialogue": "This string was sent from the Tkinter Host App"
        }
    }
    #Send JSON to our program
    try:
        # Connect to server and send data
        sock.connect((HOST, PORT))
        sock.sendall(bytes(data,encoding="utf-8"))


        # Receive data from the server and shut down
        sock.settimeout(1)
        received = sock.recv(1024)
        received = received.decode("utf-8")

    finally:
        sock.close()
        
    print("Sent:     {}".format(data))
    if received:
        print("Received: {}".format(received))
 
btn = Button(app, text = "Create window using simple mode", fg = "black", command=clicked)

btn.grid(column=0, row=0)

app.mainloop()
