import time

# Set up the serial communication to Arduino
arduino = None  # Initialize Arduino serial instance as None

def initialize_serial(serial_instance):
    """Attach an existing serial instance instead of opening a new one."""
    global arduino
    arduino = serial_instance

def send_command(command):
    """Send a command to the Arduino via serial communication."""
    if arduino and arduino.is_open:
        arduino.write(command.encode())  # Convert string to bytes before sending
        time.sleep(0.1)  # Small delay to allow Arduino to process the command
    else:
        print("Error: Serial connection not available.")

def set_servo_angles(angles):
    """Send servo angles to Arduino for controlling the robotic hand."""
    if arduino and arduino.is_open:
        print(f"Sending angles: {angles}")  # Debugging output to verify data
        command = ' '.join(str(angles[servo]) for servo in ['thumb', 'index', 'middle', 'ring', 'pinky']) + '\n'
        send_command(command)
    else:
        print("Error: Serial connection not available.")
