#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

// File System Setup

int main() {
  std::string modelDir = "../models/";
  cv::VideoCapture cap(0); // 0 = default camera
  if (!cap.isOpened()) {
    std::cerr << "Could not open camera" << std::endl;
    return -1;
  }

  // Load DNN face detector model (comes with OpenCV)
  cv::dnn::Net net =
      cv::dnn::readNetFromTensorflow(modelDir + "opencv_face_detector_uint8.pb",
                                     modelDir + "opencv_face_detector.pbtxt");

  // Check if model loaded successfully
  if (net.empty()) {
    std::cerr << "Failed to load face detection model." << std::endl;
    std::cerr << "Make sure 'opencv_face_detector_uint8.pb' and "
                 "'opencv_face_detector.pbtxt' are in your working directory."
              << std::endl;
    std::cerr << "These files are typically found in OpenCV's "
                 "'samples/dnn/face_detector' directory."
              << std::endl;
    return -1;
  }

  // Set preferable backend and target (CPU by default)
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

  cv::Mat frame, gray;
  while (true) {
    cap >> frame; // grab a frame
    if (frame.empty())
      break;

    // Face detection using DNN
    cv::Mat blob =
        cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300),
                               cv::Scalar(104, 177, 123), false, false);
    net.setInput(blob);
    cv::Mat detections = net.forward();

    // Process detections
    cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F,
                         detections.ptr<float>());
    for (int i = 0; i < detectionMat.rows; i++) {
      float confidence = detectionMat.at<float>(i, 2);
      if (confidence > 0.7) { // Confidence threshold
        int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
        int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
        int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
        int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

        // Draw rectangle around face
        cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2),
                      cv::Scalar(0, 255, 0), 2);

        // Draw confidence label
        std::string label = cv::format("Face: %.2f", confidence);
        int baseLine;
        cv::Size labelSize =
            cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        cv::rectangle(frame, cv::Point(x1, y1 - labelSize.height),
                      cv::Point(x1 + labelSize.width, y1 + baseLine),
                      cv::Scalar(0, 255, 0), cv::FILLED);
        cv::putText(frame, label, cv::Point(x1, y1), cv::FONT_HERSHEY_SIMPLEX,
                    0.5, cv::Scalar(0, 0, 0));
      }
    }

    cv::imshow("FaceBreak - Camera Feed", frame);
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    if (cv::waitKey(1) == 'q')
      break; // press q to quit
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
