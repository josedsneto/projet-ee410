import serial
import csv

# Set up the serial connection
ser = serial.Serial('/dev/ttyACM0', 9600)  # Replace 'COM3' with your actual Arduino port

# Create a CSV file to store the data
with open('arduino_float_data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["X (g)", "Y (g)", "Z (g)"])  # Write the header row
    
    x, y, z = None, None, None  # Initialize variables to store X, Y, Z

    while True:
        line = ser.readline().decode('utf-8').strip()  # Read and decode the serial data
        
        # Extract X, Y, Z float values from the lines containing "X=", "Y=", and "Z="
        if line.startswith("X="):
            x = float(line.split("X=")[1].split("g")[0].strip())
        elif line.startswith("Y="):
            y = float(line.split("Y=")[1].split("g")[0].strip())
        elif line.startswith("Z="):
            z = float(line.split("Z=")[1].split("g")[0].strip())
        
        # When all three values (X, Y, Z) are captured, write them to the CSV
        if x is not None and y is not None and z is not None:
            # Print the float data (optional, just to confirm)
            print(f"X={x}g, Y={y}g, Z={z}g")
            
            # Write the float data to the CSV file
            writer.writerow([x, y, z])
            
            # Reset X, Y, Z to capture the next set
            x, y, z = None, None, None

