import tkinter as tk
from PIL import Image, ImageTk  # For handling and displaying images in the GUI
import threading  # For running background tasks without freezing the GUI
import serial  # For communication with Arduino via serial port
import os  # For handling file paths
import time  # For adding delays where necessary
from Processing.video_processing import process_video  # Function to handle webcam processing
from Arduino.controller import initialize_serial  # Function to initialize serial communication in controller.py

# Serial Configuration
ARDUINO_PORT = "COM5"  # Define the port to which the Arduino is connected
BAUD_RATE = 9600  # Set the baud rate for serial communication

arduino = None  # Initialize the Arduino serial instance

# Try connecting to Arduino
try:
    arduino = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)  # Establish serial connection
    time.sleep(2)  # Wait for the connection to stabilize
    print("Connected to Arduino")
except serial.SerialException:
    print("Error: Could not open serial port.")

# Pass the serial instance to controller.py for use in other modules
initialize_serial(arduino)

# Initialize GUI
root = tk.Tk()
root.title("M.I.M.E.H")  # Set the window title

# Define window size and position it at the center of the screen
window_width, window_height = 800, 600
screen_width, screen_height = root.winfo_screenwidth(), root.winfo_screenheight()
x_position, y_position = (screen_width - window_width) // 2, (screen_height - window_height) // 2
root.geometry(f"{window_width}x{window_height}+{x_position}+{y_position}")
root.configure(bg='#FFFFFF')  # Set background color

# Load Logo
current_dir = os.path.dirname(os.path.abspath(__file__))  # Get the current script directory
logo_path = os.path.join(current_dir, 'Assets', 'Loghi', 'mini_logo.jpg')  # Construct the path to the logo
logo = Image.open(logo_path)  # Open the image
logo = logo.resize((50, 50), Image.LANCZOS)  # Resize it for display
logo_photo = ImageTk.PhotoImage(logo)  # Convert it to a format usable by Tkinter

# Header Frame (Title + Logo)
header_frame = tk.Frame(root, bg='#FFFFFF')  # Create a frame for the header
header_frame.place(relx=0.5, rely=0.1, anchor="center")  # Position it at the top-center

# Application title label
app_name_label = tk.Label(header_frame, text="M.I.M.E.H", font=("Segoe UI", 40, "bold"), fg="#4169E1", bg="#FFFFFF")
app_name_label.pack(side="left")  # Pack it to the left inside the frame

# Logo label
logo_label = tk.Label(header_frame, image=logo_photo, bg="#FFFFFF")
logo_label.image = logo_photo  # Keep a reference to prevent garbage collection
logo_label.pack(side="left")

# Status Label for displaying messages to the user
status_label = tk.Label(root, text="", font=("Segoe UI", 20, "bold"), fg="#4169E1", bg="#FFFFFF")
status_label.place(relx=0.5, rely=0.3, anchor="center")

# Function to start video processing when triggered by Arduino
def handle_webcam_processing():
    global arduino_thread_running
    arduino_thread_running = False  # Stop listening for Arduino signals
    process_video()  # Start the video processing function

# Function to continuously listen for Arduino commands
def listen_arduino():
    global arduino_thread_running
    while arduino_thread_running:
        try:
            if arduino and arduino.in_waiting > 0:  # Check if data is available in the serial buffer
                command = arduino.readline().decode('utf-8').strip()  # Read and decode the command
                if command == "1":  # If command is "1", start video processing
                    root.after(0, handle_webcam_processing)  # Run function in the main thread
                    break  # Exit loop
                else: # Any other result, update the status label
                    root.after(0, lambda: status_label.config(text="Please look and use the MSP432 for using the hand"))
                    break  # Exit loop
        except:
            pass  # Ignore any exceptions (could improve error handling here)

# Function to initialize the interface and start listening to Arduino
def create_interface():
    if arduino and arduino.is_open:  # Check if the Arduino is properly connected
        status_label.config(text="Waiting for mode chosen by Arduino...")  # Update status message
        global arduino_thread_running
        arduino_thread_running = True  # Set the flag to keep listening
        threading.Thread(target=listen_arduino, daemon=True).start()  # Start listening in a background thread
    else:
        status_label.config(text="Arduino not connected. \n Please connect the device and restart the program")  # Display error message

# Start the interface
create_interface()

# Run the Tkinter event loop to keep the GUI active
root.mainloop()
