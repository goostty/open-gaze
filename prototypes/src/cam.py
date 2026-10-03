# This File Initializes camera and captures
# the frame

import cv2
import mediapipe as mp
from mediapipe.tasks import python

base_options = python.BaseOptions(
    model_asset_path="../models/face_landmarker.task",
    delegate=mp.tasks.BaseOptions.Delegate.CPU,
)
options = python.vision.FaceLandmarkerOptions(
    base_options=base_options,
    num_faces=1,
    min_face_detection_confidence=0.5,
    running_mode=python.vision.FaceLandmarkerOptions.running_mode.VIDEO,  # or IMAGE
)
face_landmarker = python.vision.FaceLandmarker.create_from_options(options)
cap = cv2.VideoCapture(0)


def cam_loop():
    timestamp = 0
    # Open webcam
    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            break

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)

        # VIDEO mode requires monotonically increasing timestamps
        result = face_landmarker.detect_for_video(mp_image, timestamp)
        timestamp += 1  # or use time.time_ns() // 1_000_000

        if result.face_landmarks:
            landmarks = result.face_landmarks[0]
            # 478 landmarks per face (468 + 10 iris)
            for lm in landmarks:
                x = int(lm.x * frame.shape[1])
                y = int(lm.y * frame.shape[0])
                # draw, process, etc.
                cv2.circle(frame, (x, y), 1, (0, 255, 0), -1)

        cv2.imshow("Face Mesh", cv2.flip(frame, 1))
        if cv2.waitKey(5) & 0xFF == 27:
            break


cam_loop()
cap.release()
face_landmarker.close()
