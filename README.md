# About
Our project is a standalone application that can be included in or called on by other applications to display subtitles. Relevant functionalities of the application include displaying a window, displaying subtitle text, communicating with the host application, accepting multiple subtitle formats, and providing custom API for the host application. 

# Usage
At this point in time, users must build the program themselves. Clone the repo and build in Visual Studio using the provided solution file. Within your own application, execute the previously built executable, providing a port number. Then, open a TCP socket and send a JSON message using our API (To be documented here later). 

Example (Python):
```
args = ['path\\to\\Subtitle-Display-Tool.exe', '9999']
subprocess.Popen(args)
```

# Release Notes
## Milestone 1
* Program currently successfully creates a window which displays text. The window is transparent, has no surrounding titlebar/decorations, is always drawn in the foreground, and passes inputs through to windows behind it.

## Milestone 2
* Program was updated to display multiple windows; windows are stored in a vector which can be modified in the main loop. An Input Manager class has been added to handle opening and listening on a port for messages from the Host app. Program accepts a port number through the command line and listens on that port. Currently only accepts a single message, with plans to properly handle listening over the lifetime of the program by Milestone 3. Now offer a JSON API which allows the Host app to send just a string or optionally specify styling information. Subtitle windows support custom RGBA font color and background color, font face, font size, and window lifetimes.
