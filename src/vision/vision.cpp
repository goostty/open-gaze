#include "vision/vision.hpp"

#include <iostream>

namespace vision {

bool VisionSystem::init(const std::string &modelDir) {
  cap_.open(0); // 0 = default camera
  if (!cap_.isOpened()) {
    std::cerr << "Could not open camera" << std::endl;
    return false;
  }

  // Load DNN face detector model (comes with OpenCV)
  net_ = cv::dnn::readNetFromTensorflow(
      modelDir + "opencv_face_detector_uint8.pb",
      modelDir + "opencv_face_detector.pbtxt");

  if (net_.empty()) {
    std::cerr << "Failed to load face detection model." << std::endl;
    std::cerr << "Make sure 'opencv_face_detector_uint8.pb' and "
                 "'opencv_face_detector.pbtxt' are in "
              << modelDir << std::endl;
    return false;
  }

  // Set preferable backend and target (CPU by default)
  net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
  return true;
}

bool VisionSystem::step() {
  cv::Mat frame;
  cap_ >> frame; // grab a frame
  if (frame.empty())
    return false;

  // Face detection using DNN
  cv::Mat blob =
      cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300),
                             cv::Scalar(104, 177, 123), false, false);
  net_.setInput(blob);
  cv::Mat detections = net_.forward();

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
  return cv::waitKey(1) != 'q'; // press q to quit
}

void VisionSystem::shutdown() {
  cap_.release();
  cv::destroyAllWindows();
}

} // namespace vision
