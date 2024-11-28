import tkinter as tk

import serial
from PIL import Image, ImageTk

# Set up serial communication
interface = "rfcomm0"
ser = serial.Serial(f"/dev/{interface}", 9600, timeout=1)

# Initialize Tkinter window
root = tk.Tk()
root.title("Serial Image Display")

# Load images once and store in a dictionary
image_paths = {
    "idle": "Images/walking.jpg",
    "waiting": "Images/Blog-Image-Are-You-Ok.jpg",
    "ok": "Images/its_ok.png",
    "danger": "Images/RAZ_Emeregency_Service.png",
}

images = {
    key: ImageTk.PhotoImage(Image.open(path)) for key, path in image_paths.items()
}

# Create a label to display images
image_label = tk.Label(root)
image_label.pack()


# Function to update the displayed image
def update_image(status):
    if status in images:
        image_label.config(image=images[status])
        image_label.image = images[status]  # Keep reference to avoid garbage collection
    else:
        print(f"Unknown status: {status}")


# Main loop to read from the serial port and update the GUI
def read_serial():
    if ser.in_waiting > 0:
        line = ser.readline().decode("utf-8").strip()
        update_image(line)  # Update image based on received status
    root.after(100, read_serial)  # Schedule next read


# Start reading serial data
root.after(100, read_serial)

# Run the Tkinter event loop
try:
    root.mainloop()
except KeyboardInterrupt:
    print("Exiting program.")
    ser.close()
