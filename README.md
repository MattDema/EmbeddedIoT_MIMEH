# EmbeddedIoT_MIMEH
M.I.M.E.H Project for Embedded Software for the Internet of Things

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
- PyCharm
- Media pipe
- ...

## Project Layout
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

### Setting up the Software

## User guide
<!--=========================================================================-->


