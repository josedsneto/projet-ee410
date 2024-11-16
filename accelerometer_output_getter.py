import csv

import serial

# Set up the serial connection
ser = serial.Serial(
    "/dev/rfcomm0", 9600
)  # Replace 'COM3' with your actual Arduino port

# Create a CSV file to store the data
with open("arduino_float_data.csv", mode="w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["X (g)", "Y (g)", "Z (g)", "Chute"])  # Write the header row

    x, y, z, chute = None, None, None, None  # Initialize variables to store X, Y, Z

    while True:
        line = ser.readline().decode("utf-8").strip()  # Read and decode the serial data

        # Extract X, Y, Z float values from the lines containing "X=", "Y=", "Z=" and Chute
        if line.startswith("X="):
            x = float(line.split("X=")[1].split("g")[0].strip())
        elif line.startswith("Y="):
            y = float(line.split("Y=")[1].split("g")[0].strip())
        elif line.startswith("Z="):
            z = float(line.split("Z=")[1].split("g")[0].strip())
        elif line.startswith("FallLabel="):
            chute = float(line.split("FallLabel=")[1])

        # When all three values (X, Y, Z) are captured, write them to the CSV
        if x is not None and y is not None and z is not None and chute is not None:
            # Print the float data (optional, just to confirm)
            print(f"X={x}g, Y={y}g, Z={z}g, Chute={chute}")

            # Write the float data to the CSV file
            writer.writerow([x, y, z, chute])

            # Reset X, Y, Z to capture the next set
            x, y, z, chute = None, None, None, None
