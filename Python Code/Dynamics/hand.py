import math
import numpy as np
from Arduino.controller import set_servo_angles


class Hand:
    MAX_THRESHOLD = 0  # Maximum threshold for distance detection, adjust as needed
    MIN_THRESHOLD = 0  # Minimum threshold for distance detection, adjust as needed

    def __init__(self, landmarks):
        # Initialize the Hand class with hand landmarks
        self.landmarks = landmarks
        self.init_hand()  # Initialize hand landmarks
        self.update_th()  # Update distance thresholds
        self.prev_angles = {
            "thumb": 0,
            "index": 0,
            "middle": 0,
            "ring": 0,
            "pinky": 0
        }  # Initialize previous angles to 90 degrees

    def init_hand(self):
        # Set hand reference points using MediaPipe landmarks
        self.palm = self.landmarks.landmark[0]
        self.thumb_cmc = self.landmarks.landmark[1]
        self.thumb_base = self.landmarks.landmark[2]
        self.thumb_ip = self.landmarks.landmark[3]
        self.thumb_tip = self.landmarks.landmark[4]
        self.index_base = self.landmarks.landmark[5]
        self.index_p = self.landmarks.landmark[6]
        self.index_ip = self.landmarks.landmark[7]
        self.index_tip = self.landmarks.landmark[8]
        self.middle_base = self.landmarks.landmark[9]
        self.middle_p = self.landmarks.landmark[10]
        self.middle_ip = self.landmarks.landmark[11]
        self.middle_tip = self.landmarks.landmark[12]
        self.ring_base = self.landmarks.landmark[13]
        self.ring_p = self.landmarks.landmark[14]
        self.ring_ip = self.landmarks.landmark[15]
        self.ring_tip = self.landmarks.landmark[16]
        self.pinky_base = self.landmarks.landmark[17]
        self.pinky_p = self.landmarks.landmark[18]
        self.pinky_ip = self.landmarks.landmark[19]
        self.pinky_tip = self.landmarks.landmark[20]

    def calculate_finger_direction(self, finger_tip, finger_base):
        """Calculate the angle between the finger direction and the palm normal"""
        # Calculate vectors for the finger direction and palm normal
        finger_direction = (
            finger_tip.x - finger_base.x,
            finger_tip.y - finger_base.y,
            finger_tip.z - finger_base.z,
        )
        return finger_direction

    def calculate_palm_normal(self):
        # Define the points
        palm_point = np.array([self.palm.x, self.palm.y, self.palm.z])
        index_base_point = np.array([self.index_base.x, self.index_base.y, self.index_base.z])
        pinky_base_point = np.array([self.pinky_base.x, self.pinky_base.y, self.pinky_base.z])

        # Calculate the vectors
        vector1 = index_base_point - palm_point
        vector2 = pinky_base_point - palm_point

        # Calculate the cross product
        palm_normal = np.cross(vector1, vector2)

        # Normalize the normal vector (optional but recommended)
        palm_normal = palm_normal / np.linalg.norm(palm_normal)

        return palm_normal

    def calculate_angle(self, finger_direction, palm_normal):
        """Calculate the angle between the finger direction and the palm normal"""
        # Dot product
        dot_product = sum(f * n for f, n in zip(finger_direction, palm_normal))
        # Magnitudes
        finger_magnitude = math.sqrt(sum(f ** 2 for f in finger_direction))
        palm_magnitude = math.sqrt(sum(n ** 2 for n in palm_normal))
        # Cosine of the angle between the vectors
        cos_angle = dot_product / (finger_magnitude * palm_magnitude)
        # Convert to radians, then to degrees
        angle = math.acos(cos_angle)
        return math.degrees(angle)

    def get_servo_angles(self):
        """Calculate and normalize angles for servo control."""
        palm_normal = self.calculate_palm_normal()

        angles = {
            "thumb": self.calculate_angle(self.calculate_finger_direction(self.thumb_tip, self.thumb_cmc),
                                          self.calculate_finger_direction(self.thumb_cmc, self.palm)),
            "index": self.calculate_angle(self.calculate_finger_direction(self.index_tip, self.index_base),
                                          palm_normal),
            "middle": self.calculate_angle(self.calculate_finger_direction(self.middle_tip, self.middle_base),
                                           palm_normal),
            "ring": self.calculate_angle(self.calculate_finger_direction(self.ring_tip, self.ring_base),
                                         self.calculate_finger_direction(self.ring_base, self.palm)),
            "pinky": self.calculate_angle(self.calculate_finger_direction(self.pinky_tip, self.pinky_base),
                                          self.calculate_finger_direction(self.pinky_base, self.palm)),
        }

        # Define the min and max angle thresholds
        MIN_ANGLE = 10  # Closed hand
        MAX_ANGLE = 90  # Open hand

        def map_angle(angle, invert=False):
            """Map detected angle to servo range [0, 180], with optional inversion."""
            if invert:
                normalized_angle = (MAX_ANGLE - angle) / (MAX_ANGLE - MIN_ANGLE) * 180  # Inverted mapping
            else:
                normalized_angle = (angle - MIN_ANGLE) / (MAX_ANGLE - MIN_ANGLE) * 180  # Normal mapping
            return max(0, min(180, normalized_angle))  # Ensure within [0, 180]

        # Apply different mappings based on the finger type
        servo_angles = {
            "thumb": int(map_angle(angles["thumb"])),
            "index": int(map_angle(angles["index"], invert=True)),  # Invert mapping
            "middle": int(map_angle(angles["middle"], invert=True)),  # Invert mapping
            "ring": int(map_angle(angles["ring"])),
            "pinky": int(map_angle(angles["pinky"])),
        }

        print(f"Raw Angles: {angles}")  # Debugging
        print(f"Mapped Servo Angles: {servo_angles}")  # Debugging

        return servo_angles

    def send_servo_angles(self):
        """Send the calculated servo angles to the Arduino"""
        angles = self.get_servo_angles()  # This gets the servo angles as a dictionary
        # Convert the angles to integers
        int_angles = {key: int(angle) for key, angle in angles.items()}
        set_servo_angles(int_angles)

    def update_th(self):
        # Update distance thresholds using the distance between the palm and the thumb tip
        self.MAX_THRESHOLD = self.distance(self.palm, self.thumb_tip) * 2
        self.MIN_THRESHOLD = self.distance(self.palm, self.thumb_tip) * 1.1

    def distance(self, point1, point2):
        # Calculate the Euclidean distance between two reference points
        return math.sqrt((point1.x - point2.x) ** 2 + (point1.y - point2.y) ** 2)
