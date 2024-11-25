import time

import serial
from PIL import Image

interface = "ttyUSB0"
ser = serial.Serial(f"/dev/{interface}", 9600)


while True:
    line = ser.readline().decode("utf-8").strip()

    if line == "idle":
        image = Image.open("Images/walking.jpg")
        image.show()
        time.sleep(1000)
    elif line == "waiting":
        image = Image.open("Images/Blog-Image-Are-You-Ok.jpg")
        image.show()
        time.sleep(1000)
    elif line == "ok":
        image = Image.open("Images/its_ok.png")
        image.show()
        time.sleep(1000)
    elif line == "danger":
        image = Image.open("RAZ_Emeregency_Service.png")
        image.show()
        time.sleep(1000)
    else:
        print(f"receiving: {line}")
