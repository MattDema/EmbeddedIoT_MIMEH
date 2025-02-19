import math
import numpy as np
from Arduino.controller import set_servo_angles


class Hand:
    MAX_THRESHOLD = 0  # Soglia massima per il rilevamento della distanza, da regolare secondo necessità
    MIN_THRESHOLD = 0  # Soglia minima per il rilevamento della distanza, da regolare secondo necessità

    def __init__(self, landmarks):
        # Inizializza la classe Hand con i punti di riferimento della mano
        self.landmarks = landmarks
        self.init_hand()  # Inizializza i punti di riferimento della mano
        self.update_th()  # Aggiorna le soglie di distanza
        self.prev_angles = {
            "thumb": 0,
            "index": 0,
            "middle": 0,
            "ring": 0,
            "pinky": 0
        }  # Initialize previous angles to 90 degrees

    def init_hand(self):
        # Imposta i punti di riferimento della mano utilizzando i landmark di MediaPipe
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
        # Definisci i punti
        palm_point = np.array([self.palm.x, self.palm.y, self.palm.z])
        index_base_point = np.array([self.index_base.x, self.index_base.y, self.index_base.z])
        pinky_base_point = np.array([self.pinky_base.x, self.pinky_base.y, self.pinky_base.z])

        # Calcola i vettori
        vector1 = index_base_point - palm_point
        vector2 = pinky_base_point - palm_point

        # Calcola il prodotto vettoriale
        palm_normal = np.cross(vector1, vector2)

        # Normalizza il vettore normale (opzionale, ma consigliato)
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
        # Clamp cosine value to avoid floating-point errors
        #cos_angle = max(-1.0, min(1.0, cos_angle))
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
        MIN_ANGLE = 30  # Closed hand
        MAX_ANGLE = 90  # Open hand

        def map_angle(angle, invert=False):
            """Map detected angle to servo range [0, 180], with optional inversion."""
            if invert:
                normalized_angle = (MAX_ANGLE - angle) / (MAX_ANGLE - MIN_ANGLE) * 180  # Inverted mapping
            else:
                normalized_angle = (angle - MIN_ANGLE) / (MAX_ANGLE - MIN_ANGLE) * 180  # Normal mapping
            return max(0, min(180, normalized_angle))  # Ensure within [0, 180]

        # Apply different mappings based on the finger type

        self.prev_angles = {
            "thumb": int(map_angle(angles["thumb"])),
            "index": int(map_angle(angles["index"], invert=True)),  # Invert mapping
            "middle": int(map_angle(angles["middle"], invert=True)),  # Invert mapping
            "ring": int(map_angle(angles["ring"])),
            "pinky": int(map_angle(angles["pinky"])),
        }

        # Convert angles and ensure they are integers
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
        # Aggiorna le soglie di distanza utilizzando la distanza tra il palmo e la punta del pollice
        self.MAX_THRESHOLD = self.distance(self.palm, self.thumb_tip) * 2
        self.MIN_THRESHOLD = self.distance(self.palm, self.thumb_tip) * 1.1

    def distance(self, point1, point2):
        # Calcola la distanza euclidea tra due punti di riferimento.
        distance = math.sqrt(
            (point1.x - point2.x) ** 2 +
            (point1.y - point2.y) ** 2
        )
        return distance

    @property
    def orientation(self):
        dx = self.middle_base.x - self.palm.x
        dy = self.middle_base.y - self.palm.y

        if abs(dx) < 0.05:  # Gestisce il caso quasi verticale
            if dy < 0:
                return "Up"
            else:
                return "Down"
        else:
            tan_theta = dy / dx

        if -1 <= tan_theta <= 1:
            if dx > 0:
                return "Right"
            else:
                return "Left"
        else:
            if dy < 0:
                return "Up"
            else:
                return "Down"

    def distance_th(self, point1, point2, threshold):
        # Calcola la distanza euclidea tra due punti con una soglia.
        return math.sqrt(
            (point1.x - (point2.x + threshold)) ** 2 +
            (point1.y - (point2.y + threshold)) ** 2
        )

    def is_finger_open(self, finger_tip, finger_ip, finger_base):
        # Determina se un dito è aperto basandosi sulla distanza dal palmo.
        tip_to_palm = self.distance(finger_tip, self.palm)
        ip_to_palm = self.distance(finger_ip, self.palm)
        base_to_palm = self.distance(finger_base, self.palm)
        return (
            tip_to_palm > ip_to_palm > base_to_palm
        )

    def is_finger_closed(self, finger_tip, finger_p):
        # Determina se un dito è chiuso basandosi sulla distanza dal palmo.
        tip_to_palm = self.distance(finger_tip, self.palm)
        p_to_palm = self.distance(finger_p, self.palm)
        return (
            tip_to_palm < p_to_palm and
            (tip_to_palm < self.MIN_THRESHOLD)
        )

    @property
    def is_thumb_in_position(self):
        # Determina se il pollice è in posizione rispetto all'indice e al mignolo.
        if self.thumb_tip.x > self.pinky_tip.x:
            thumb_inside = self.thumb_tip.x < self.index_tip.x
        else:
            thumb_inside = self.thumb_tip.x > self.index_tip.x
        return thumb_inside

    @property
    def is_hand_opened(self):
        # Determina se la mano è aperta controllando tutte le dita.
        return (
            not self.is_thumb_in_position and
            self.is_finger_open(self.thumb_tip, self.thumb_ip, self.palm) and
            self.is_finger_open(self.index_tip, self.index_ip, self.index_base) and
            self.is_finger_open(self.middle_tip, self.middle_ip, self.middle_base) and
            self.is_finger_open(self.ring_tip, self.ring_ip, self.ring_base) and
            self.is_finger_open(self.pinky_tip, self.pinky_ip, self.pinky_base)
        )

    @property
    def is_hand_closed(self):
        # Determina se la mano è chiusa controllando tutte le dita.
        return (
            self.is_thumb_in_position and
            self.is_finger_closed(self.thumb_tip, self.middle_p) and
            self.is_finger_closed(self.index_tip, self.index_p) and
            self.is_finger_closed(self.middle_tip, self.middle_p) and
            self.is_finger_closed(self.ring_tip, self.ring_p) and
            self.is_finger_closed(self.pinky_tip, self.pinky_p)
        )

    @property
    def is_thumb_up(self):
        # Determina se il gesto è 'okay' (dito pollice alzato).
        return (
                not self.is_thumb_in_position and
                self.is_finger_open(self.thumb_tip, self.thumb_ip, self.palm) and
                self.is_finger_closed(self.index_tip, self.index_p) and
                self.is_finger_closed(self.middle_tip, self.middle_p) and
                self.is_finger_closed(self.ring_tip, self.ring_p) and
                self.is_finger_closed(self.pinky_tip, self.pinky_p)
        )

    @property
    def is_one(self):
        # Determina se il gesto è 'uno' (dito indice alzato).
        return (
            self.is_thumb_in_position and
            self.is_finger_closed(self.thumb_tip, self.middle_p) and
            self.is_finger_open(self.index_tip, self.index_ip, self.index_base) and
            self.is_finger_closed(self.middle_tip, self.middle_p) and
            self.is_finger_closed(self.ring_tip, self.ring_p) and
            self.is_finger_closed(self.pinky_tip, self.pinky_p)
        )

    @property
    def is_two(self):
        # Determina se il gesto è 'due' (dita indice e medio alzati).
        return (
            self.is_thumb_in_position and
            self.is_finger_closed(self.thumb_tip, self.middle_p) and
            self.is_finger_open(self.index_tip, self.index_ip, self.index_base) and
            self.is_finger_open(self.middle_tip, self.middle_ip, self.middle_base) and
            self.is_finger_closed(self.ring_tip, self.ring_p) and
            self.is_finger_closed(self.pinky_tip, self.pinky_p)
        )

    @property
    def is_three(self):
        # Determina se il gesto è 'tre' (dita pollice, indice e medio alzati).
        return (
            not self.is_thumb_in_position and
            self.is_finger_open(self.thumb_tip, self.thumb_ip, self.palm) and
            self.is_finger_open(self.index_tip, self.index_ip, self.index_base) and
            self.is_finger_open(self.middle_tip, self.middle_ip, self.middle_base) and
            self.is_finger_closed(self.ring_tip, self.ring_p) and
            self.is_finger_closed(self.pinky_tip, self.pinky_p)
        )