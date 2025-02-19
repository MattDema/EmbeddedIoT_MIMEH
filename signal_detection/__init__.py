import cv2
import os


# Define the path to the logo relative to this script
logo_path = os.path.join(os.path.dirname(__file__), '..', 'Assets', 'Loghi', 'mini_logo.jpg')

# Load the logo
logo = cv2.imread(logo_path)

def draw_text_with_logo(frame, text, padding=10):
    # Set the text properties
    font = cv2.FONT_HERSHEY_DUPLEX
    font_scale = 1
    font_thickness = 2
    text_size, _ = cv2.getTextSize(text, font, font_scale, font_thickness)

    # Maximize logo size while maintaining aspect ratio
    max_logo_height = text_size[1] + 2 * padding
    max_logo_width = int(logo.shape[1] * (max_logo_height / logo.shape[0]))
    resized_logo = cv2.resize(logo, (max_logo_width, max_logo_height))

    # Calculate the position and size of the rectangle
    frame_height, frame_width, _ = frame.shape
    rect_width = text_size[0] + max_logo_width + 2 * padding  # Padding of 10 on each side of text and logo
    rect_height = max(text_size[1], max_logo_height) + 2 * padding  # Padding of 10 above and below
    rect_x = frame_width - rect_width - padding
    rect_y = padding

    # Draw the rectangle
    cv2.rectangle(frame, (rect_x, rect_y), (rect_x + rect_width, rect_y + rect_height), (255, 255, 255), -1)

    # Place the text inside the rectangle and center it vertically
    text_x = rect_x + padding
    text_y = rect_y + ((rect_height - text_size[1]) // 2) + text_size[1]
    cv2.putText(frame, text, (text_x, text_y), font, font_scale, (225, 105, 65), font_thickness)

    # Place the resized logo inside the rectangle, to the right of the text
    logo_x = text_x + text_size[0] + padding
    logo_y = rect_y + ((rect_height - max_logo_height) // 2)
    frame[logo_y:logo_y + max_logo_height, logo_x:logo_x + max_logo_width] = resized_logo
