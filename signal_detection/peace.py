import Arduino.controller
from . import draw_text_with_logo


def peace(hand, cv2, frame, recorder):
    """
    Detects the "peace" hand signal and handles recording the corresponding signal.

    Args:
        hand (Hand): The Hand object containing hand landmarks and detection results.
        cv2 (cv2): The OpenCV library used to handle image processing.
        frame (numpy.ndarray): The current video frame.
        recorder (FoulRecorder): The recorder used to save the video if a foul is detected.
    """
    # Checks if the hand is in a "peace" gesture (two fingers raised, no other fingers bent)
    if (hand.is_two and not hand.is_hand_closed
            and not hand.is_hand_opened and not hand.is_one and not hand.is_three):

        # If not recording or the current foul type isn't "peace", start a new recording
        if not recorder.is_recording or recorder.current_foul_type != "peace":
            # If already recording, stop the current recording
            if recorder.is_recording:
                recorder.stop_recording()
            # Start recording for the "peace" foul type
            recorder.start_recording("peace")

        # Display the message "Peace!" on the frame with a logo
        draw_text_with_logo(frame, "Peace!")
    else:
        # If recording and the current foul type is "peace", stop the recording
        if recorder.is_recording and recorder.current_foul_type == "peace":
            recorder.stop_recording()
