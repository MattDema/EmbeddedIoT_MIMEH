import cv2
import mediapipe as mp
import importlib
import os

from Dynamics.hand import Hand
from Recording.rec import SignalRecorder

def load_signal_detectors():
    """
    Loads signal detection functions dynamically from the 'signal_detection' package.

    Returns:
        dict: A dictionary containing the signal detection functions mapped by their names.
    """
    signal_detectors = {}
    signal_detection_folder = os.path.join(os.path.dirname(__file__), '..', 'signal_detection')

    print(f"Looking for signal detectors in folder: {signal_detection_folder}")

    # Iterates through the files in the signal detection folder
    for file in os.listdir(signal_detection_folder):
        # Checks if the file is a Python script and not the package initializer
        if file.endswith('.py') and file != '__init__.py':
            # Extracts the module name from the file name (removes .py)
            module_name = file[:-3]
            print(f"Found module: {module_name}")

            # Dynamically imports the module using importlib
            module = importlib.import_module(f'signal_detection.{module_name}')
            print(f"Imported module: {module}")

            # Gets the signal detection function from the module
            detector_function = getattr(module, f'{module_name}', None)
            if detector_function:
                print(f"Loaded detector function: {module_name}")
                # Adds the detector function to the dictionary
                signal_detectors[module_name] = detector_function
            else:
                print(f"Warning: No {module_name} function found in module {module}")

    return signal_detectors


def process_stream(video_source):
    """
    Processes the video stream from the webcam or file for signal detection.

    Args:
        video_source (int or str): Video source, either webcam (0) or file path.
    """

    # Initializes MediaPipe Hands for hand detection
    mp_hands = mp.solutions.hands
    hands = mp_hands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.5, min_tracking_confidence=0.5)

    # Initializes MediaPipe drawing utilities
    mp_drawing = mp.solutions.drawing_utils

    # Loads signal detection functions
    signal_detectors = load_signal_detectors()

    # Initializes the foul recorder
    recorder = SignalRecorder(buffer_size=120, fps=10.0)

    # Opens the video source (0 for webcam or file path)
    cap = cv2.VideoCapture(video_source)

    if not cap.isOpened():
        print(f"Error: Unable to open video source {video_source}")
        return

    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            break

        # Converts the frame from BGR to RGB for MediaPipe
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Processes the frame for hand detection
        hands_detected = hands.process(frame_rgb)

        # If hands are detected, draw the landmarks and connections on the frame
        if hands_detected.multi_hand_landmarks:
            handsLandmarks = hands_detected.multi_hand_landmarks[0]

            for hand_landmarks in hands_detected.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    frame,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing.DrawingSpec(color=(176, 132, 255), thickness=2, circle_radius=2),
                    mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2)
                )

            # Creates instances of the Hand and Body classes using the detected landmarks
            hand = Hand(handsLandmarks)

            # Updates the buffer of the recorder with the current frame
            recorder.update_buffer(frame)

            hand.send_servo_angles()
            # Calls signal detection functions
            for detector_name, detector_function in signal_detectors.items():
                detector_function(hand, cv2, frame, recorder)

        # Displays the annotated frame in a window
        cv2.imshow('M.I.M.E.H - Stream Analysis', frame)

        # Exits the loop if the 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Releases the video source and closes all windows
    cap.release()
    cv2.destroyAllWindows()


def process_video(video_path=None):
    """
    Processes a video file for signal detection.

    Args:
        video_path (str): If None, uses the webcam.
    """
    process_stream(0)  # Uses the webcam
