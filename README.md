# EmbeddedIoT_MIMEH
M.I.M.E.H Project for Embedded Software for the Internet of Things

> **Note:** To view this file in preview mode, open it in a text editor like Visual Studio Code, Atom, or an online Markdown viewer such as [Dillinger](https://dillinger.io/). Otherwise, if the project is opened in your compiler, such as PyCharm, the preview will be displayed automatically.

![MIMEH Logo](./Assets/Loghi/logo.jpg)

<!--=========================================================================-->

## Idea of the project
<!--=========================================================================-->

The main idea of the project is to build a robotic hand, which moves using a dedicated servomotor for each finger.
The hand structure is composed of 3D-printed components, while movement is controlled through two different modes.
The first uses the mediapipe library OpenCV to track the movements of the user fingers, captured by the computer webcam.
The Manual Control Mode is used to move the fingers, singularly changing the angle value of the related servomotor using input joysticks.
Mode selection is controlled by a Texas Instrument board, which displays a menu on its integrated display. It's possible to move within the menu using the joystick and button sensors.
The Texas Instruments board send the inputs for the Arduino board which selects and moves the servomotors aaccordingly.

## Requirements
<!--=========================================================================-->

### Hardware Requirements

- Texas instrument Board
- Arduino UNO R4 Minima Board
- Full set of cables (male-to-male, male-to-female)
- 5x Servomotors
- 1x Breadborad

### Software Requirements

- Arduino IDE
- Code Composer Studio
#### Software Requirements - Libraries Python

For optimal program performance, it is recommended to use a machine with a dedicated and powerful graphics card. However, the program can also run on less powerful systems. The required libraries are:

- `cv2`
- `mediapipe`
- `importlib`
- `os`
- `tkinter`
- `POL`
- `sys`
- `time`
- `math`
- `collections`
- `pydantic`
- `numpy`
- `pyserial`
- `threading`

#### Library Installation

It is recommended to install all these libraries before running the program to avoid errors during runtime. Below are the commands to install the libraries via `pip`:

```bash
pip install opencv-python
pip install mediapipe
pip install importlib-metadata
pip install python-os
pip install tk
pip install python-polling
pip install sys
pip install time
pip install math
pip install collections
pip install pydantic
pip install numpy
pip install pyserial
pip install threading
```

## Project Layout

M.I.M.E.H/  
├── Assets/             # Contains logos and images  
│   └── Logos/  
│       └── mini_logo.jpg  
├── Arduino/            # Contains Arduino code  
│   └── controller.py   # Manages serial communication with Arduino  
├── Dynamics/           # Contains logic for hand management  
│   └── hand.py         # Defines the `Hand` class for landmark detection  
├── Processing/         # Contains modules for video processing  
│   └── video_processing.py  # Manages the video stream processing  
├── Recording/          # Contains modules for video recording  
│   └── rec.py          # Defines the `FoulRecorder` class for signal recording  
├── signal_detection/   # Contains modules for specific signal detection  
│   └── peace.py        # Example of "peace" signal detection  
├── main.py             # Main file to execute the program  
├── requirements.txt    # List of Python dependencies  
└── README.md           # This file

* **`Assets/`**: Contains resources like logos and images used in the graphical interface.  
* **`Arduino/`**: Contains scripts and code for communication with the Arduino board.  
    * `controller.py`: Manages serial communication with Arduino to send commands to the servos. Includes functions to initialize serial communication, send generic commands, and set servo angles [1, 2].  
* **`Dynamics/`**: Contains the logic for managing and calculating the dynamics of the hand.  
    * `hand.py`: Defines the `Hand` class, which receives hand landmark points from MediaPipe and calculates the necessary angles to control the servos. Also includes functions to determine if the hand is open, closed, or in specific positions [3-9].  
* **`Processing/`**: Contains scripts for processing the video stream.  
    * `video_processing.py`: Contains the `process_video` function which handles video stream acquisition from a webcam or file, detects hands using MediaPipe, and calls signal detection functions [1].  
* **`Recording/`**: Contains scripts for recording videos of detected signals.  
    * `rec.py`: Defines the `FoulRecorder` class, which handles video recording of detected signals. It allows starting, stopping, and saving recordings, keeping a buffer of recent frames [10-14].  
* **`signal_detection/`**: Contains scripts for detecting specific signals.  
    * `peace.py`: Contains the `peace` function, which detects the "peace" gesture (two fingers up) and starts/stops the corresponding video recording [15, 16].  
* **`main.py`**: The main file to execute the graphical interface and start the detection process [1, 17-24].  
* **`requirements.txt`**: List of Python libraries required to run the project.

## How It Works

1. **Initialization:**
    * The `main.py` program starts the graphical interface and attempts to connect to Arduino through the specified serial port [17, 18].  
    * A separate thread is started to listen for commands from Arduino [23].  
2. **Video Detection:**
    * The `process_video` function in `video_processing.py` acquires the video stream from the webcam or a file [25, 26].  
    * MediaPipe detects hands in the video stream and provides landmark points [26, 27].  
3. **Gesture Processing:**
    * The `Hand` class in `hand.py` uses the landmarks to calculate finger angles and determine the hand's position [4, 5].  
    * Functions in the `signal_detection` package (e.g., `peace.py`) use the information from the `Hand` class to detect specific gestures [15, 16].  
4. **Arduino Control:**
    * The `set_servo_angles` function in `controller.py` sends commands to Arduino to control the servos based on the detected gestures [28].  
5. **Video Recording:**
    * The `FoulRecorder` class in `rec.py` records videos of detected gestures, categorizing them by signal type [10, 14].  
6. **Interaction with Arduino:**
    * The program is designed to receive commands from Arduino, such as starting video processing or updating the displayed status in the interface [21, 22].  
    * Arduino sends the command "1" to start video processing and "2" to update the status label [22].

### Usage

1. **Run the `main.py` file:**

    ```bash
    python main.py
    ```

2. **Interact with the graphical interface:**

    The interface displays the system status and any messages.

3. **Position your hand in front of the webcam:**

    Ensure that the hand is clearly visible.

4. **Perform supported gestures:**

    The system will detect the gestures and control the robotic hand.

5. **Check video recordings:**

    The videos of the detected gestures will be saved in the `Recording` folder.

<!--=========================================================================-->

## Getting Started
<!--=========================================================================-->

### Setting up the Hand

The project begins with the construction of the hand components, which are 3D-printed using PLA material. Some parts require milling to reduce friction between the joints of the phalanges, improving overall movement performance.
The components are assembled using screws and hot glue:
- Screws are primarily used to connect the phalanges.
- Hot glue secures other parts, such as the forearm and the springs.

Finger movement is achieved through a combination of springs and fishing line:
- The springs provide a constant force that helps return the fingers to their default position.
- The fishing line connects the fingers to the servomotors, transmitting motion from the servos to the fingers.

### Setting up Wiring

The wiring process starts by connecting the servomotors to the Arduino board through a breadboard.
Each servo operates with three wires:
- Two wires for power (GND and 5V).
- One wire for the signal.

To supply power, the breadboard is first connected to the Arduino board using male-to-male wires, linking the GND and 5V pins (since the servos require 5V).
Finally, the signal wires of the servos are connected to the digital pins of the Arduino board, allowing for individual control of each motor.

MPS + ARDUINO...

### Setting up the Arduino Software

### Setting up the Texas Instrument 

### Setting up PyCharm
#### Project Layout

M.I.M.E.H/  
├── Assets/             # Contains logos and images  
│   └── Logos/  
│       └── mini_logo.jpg  
├── Arduino/            # Contains Arduino code  
│   └── controller.py   # Manages serial communication with Arduino  
├── Dynamics/           # Contains logic for hand management  
│   └── hand.py         # Defines the `Hand` class for landmark detection  
├── Processing/         # Contains modules for video processing  
│   └── video_processing.py  # Manages the video stream processing  
├── Recording/          # Contains modules for video recording  
│   └── rec.py          # Defines the `FoulRecorder` class for signal recording  
├── signal_detection/   # Contains modules for specific signal detection  
│   └── peace.py        # Example of "peace" signal detection  
├── main.py             # Main file to execute the program  
├── requirements.txt    # List of Python dependencies  
└── README.md           # This file

* **`Assets/`**: Contains resources like logos and images used in the graphical interface.  
* **`Arduino/`**: Contains scripts and code for communication with the Arduino board.  
    * `controller.py`: Manages serial communication with Arduino to send commands to the servos. Includes functions to initialize serial communication, send generic commands, and set servo angles [1, 2].  
* **`Dynamics/`**: Contains the logic for managing and calculating the dynamics of the hand.  
    * `hand.py`: Defines the `Hand` class, which receives hand landmark points from MediaPipe and calculates the necessary angles to control the servos. Also includes functions to determine if the hand is open, closed, or in specific positions [3-9].  
* **`Processing/`**: Contains scripts for processing the video stream.  
    * `video_processing.py`: Contains the `process_video` function which handles video stream acquisition from a webcam or file, detects hands using MediaPipe, and calls signal detection functions [1].  
* **`Recording/`**: Contains scripts for recording videos of detected signals.  
    * `rec.py`: Defines the `FoulRecorder` class, which handles video recording of detected signals. It allows starting, stopping, and saving recordings, keeping a buffer of recent frames [10-14].  
* **`signal_detection/`**: Contains scripts for detecting specific signals.  
    * `peace.py`: Contains the `peace` function, which detects the "peace" gesture (two fingers up) and starts/stops the corresponding video recording [15, 16].  
* **`main.py`**: The main file to execute the graphical interface and start the detection process [1, 17-24].  
* **`requirements.txt`**: List of Python libraries required to run the project.

#### How It Works

1. **Initialization:**
    * The `main.py` program starts the graphical interface and attempts to connect to Arduino through the specified serial port [17, 18].  
    * A separate thread is started to listen for commands from Arduino [23].  
2. **Video Detection:**
    * The `process_video` function in `video_processing.py` acquires the video stream from the webcam or a file [25, 26].  
    * MediaPipe detects hands in the video stream and provides landmark points [26, 27].  
3. **Gesture Processing:**
    * The `Hand` class in `hand.py` uses the landmarks to calculate finger angles and determine the hand's position [4, 5].  
    * Functions in the `signal_detection` package (e.g., `peace.py`) use the information from the `Hand` class to detect specific gestures [15, 16].  
4. **Arduino Control:**
    * The `set_servo_angles` function in `controller.py` sends commands to Arduino to control the servos based on the detected gestures [28].  
5. **Video Recording:**
    * The `FoulRecorder` class in `rec.py` records videos of detected gestures, categorizing them by signal type [10, 14].  
6. **Interaction with Arduino:**
    * The program is designed to receive commands from Arduino, such as starting video processing or updating the displayed status in the interface [21, 22].  
    * Arduino sends the command "1" to start video processing and "2" to update the status label [22].

#### Usage

1. **Run the `main.py` file:**

    ```bash
    python main.py
    ```

2. **Interact with the graphical interface:**

    The interface displays the system status and any messages.

3. **Position your hand in front of the webcam:**

    Ensure that the hand is clearly visible.

4. **Perform supported gestures:**

    The system will detect the gestures and control the robotic hand.

5. **Check video recordings:**

    The videos of the detected gestures will be saved in the `Recording` folder.

## User guide
<!--=========================================================================-->

The robotic hand operates in two different modes, which can be selected using the Texas Instruments board. Follow the steps below to use each mode:

1. Hand Tracking Mode (Mediapipe)
This mode allows the robotic hand to replicate the user's finger movements in real time using MediaPipe and a computer webcam.

  Steps:
  1. Connect the Texas Instruments board and the Arduino board to the computer.
  2. Open PyCharm and run the Python script responsible for hand tracking.
  3. The script establishes a serial communication with the Arduino.
  4. The MediaPipe library tracks the user’s hand movements through the webcam.
  5. The robotic hand mimics the movements detected by MediaPipe.
     
2. Manual Control Mode (Joystick)
   
  This mode allows for individual finger control using the joystick on the Texas Instruments     board.

  Steps:

  1. Select the Manual Control Mode from the menu on the Texas Instruments board.
  2. Use the joystick to navigate through the menu and select the finger to move.
  3. Adjust the joystick position to change the angle of the selected finger.
  4. To exit this mode, perform a left shift with the joystick.
  5. Once a mode is selected, the Texas Instruments board sends the commands to the Arduino,     which then controls the servomotors accordingly.

## Team Members
<!--=========================================================================-->

The project was developed as a team effort, with the code divided into different areas, including hand construction, Arduino programming, and Texas Instruments development.

- Matthew De Marco
  - Contributed to the Texas Instruments program, developing the operating menu and integrating     the Python configuration with the Texas Instruments configuration.
- Andrea Lo Iacono
  - Worked on hand construction and the Python-Arduino integration.
  - Remodeled 3D files for the 3D printing of hand components.
- Andrea Pezzo
  - Contributed to hand construction and Arduino implementation.
  - Assisted in both the Python integration and Texas Instruments development.
    
