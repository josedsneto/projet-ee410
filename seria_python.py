import serial
import time

# Specify the Bluetooth serial port and baud rate
bluetooth_port = '/dev/rfcomm0'  # Update this if necessary
baud_rate = 9600  # Should match the Arduino's baud rate

# Establish a serial connection
try:
    bluetooth = serial.Serial(bluetooth_port, baud_rate)
    print(f"Connected to {bluetooth_port} at {baud_rate} baud.")
except serial.SerialException as e:
    print(f"Could not open port {bluetooth_port}: {e}")
    exit()

# Give some time for the connection to establish
time.sleep(2)  # Arduino needs a moment to reset after connection

def send_message(message):
    """Send a message to the Arduino over Bluetooth."""
    try:
        bluetooth.write((message + '\n').encode())  # Add newline to indicate end of message
        print(f"Sent: {message}")
    except serial.SerialException as e:
        print(f"Error sending message: {e}")

def read_response():
    """Read the response from Arduino if available."""
    try:
        if bluetooth.in_waiting > 0:
            response = bluetooth.readline().decode().strip()
            print(f"Received: {response}")
    except serial.SerialException as e:
        print(f"Error reading response: {e}")

try:
    while True:
        # Get user input and send it to Arduino
        message = input("Enter message to send to Arduino (or type 'exit' to quit): ")
        
        if message.lower() == 'exit':
            print("Exiting...")
            break

        send_message(message)
        
        # Wait a bit and check if Arduino sent a response
        time.sleep(1)  # Small delay to give Arduino time to respond
        read_response()

except KeyboardInterrupt:
    print("\nDisconnected.")

finally:
    # Close the Bluetooth connection
    bluetooth.close()
    print("Bluetooth connection closed.")
