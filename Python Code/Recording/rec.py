import cv2
import os
import time


class SignalRecorder:
    def __init__(self, buffer_size=30, fps=20.0):
        # Initializes the recorder with empty buffers, buffer size, and frames per second (fps)
        self.buffers = {}
        self.buffer_size = buffer_size
        self.fps = fps
        self.current_signal_type = None
        self.is_recording = False

    def start_recording(self, signal_type):
        """
        Starts recording a specific signal.

        Args:
            signal_type (str): The type of signal to be recorded.
        """
        # Sets the current signal type
        self.current_signal_type = signal_type
        # If the signal type is not in the buffers, creates a new list for it
        if signal_type not in self.buffers:
            self.buffers[signal_type] = []
        # Starts the recording process
        self.is_recording = True

    def stop_recording(self):
        """
        Stops the current recording and saves the signal video.
        """
        # If there's a current signal type and it exists in the buffers
        if self.current_signal_type and self.current_signal_type in self.buffers:
            # Saves the signal video to the storage
            self.save_signal(self.current_signal_type)
            # Clears the buffer of the current signal type
            self.buffers[self.current_signal_type].clear()
        # Stops recording and resets the current signal type
        self.is_recording = False
        self.current_signal_type = None

    def update_buffer(self, frame):
        """
        Adds a new frame to the buffer if recording is ongoing.

        Args:
            frame (numpy.ndarray): The current frame to be added to the buffer.
        """
        # If recording is ongoing and a foul type is set
        if self.is_recording and self.current_signal_type:
            buffer = self.buffers[self.current_signal_type]
            # If the buffer has reached its maximum size, removes the first frame
            if len(buffer) >= self.buffer_size:
                buffer.pop(0)
            # Adds the new frame to the buffer
            buffer.append(frame)

    def save_signal(self, signal_type):
        """
        Saves the frames of the current signal type into a video file.

        Args:
            signal_type (str): The type of signal whose video is to be saved.
        """
        # If the signal type is not in the buffers or the buffer is empty, exits
        if signal_type not in self.buffers or not self.buffers[signal_type]:
            return

        # Defines the directory and the file name for the saved video
        directory = f"Recording/{signal_type}"
        if not os.path.exists(directory):
            os.makedirs(directory)
        file_name = f"{directory}/{signal_type}_{int(time.time())}.mp4"

        # Defines the video writer with the appropriate settings
        height, width, layers = self.buffers[signal_type][0].shape
        fourcc = cv2.VideoWriter_fourcc(*"mp4v")
        out = cv2.VideoWriter(file_name, fourcc, self.fps, (width, height))

        # Writes the frames in the buffer into the video file
        for frame in self.buffers[signal_type]:
            out.write(frame.astype('uint8'))

        out.release()
        print(f"Signal video saved: {file_name}")
