#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace vision {

// Model input is a 256x256 RGB square; output is 478 (x, y, z) points.
constexpr int kLandmarkInputSize = 256;
constexpr int kNumLandmarks = 478;

// The crop is the face box's longer side times this. 1.0 cuts off the chin and
// forehead, 1.75 shrinks the face too much; 1.25 matched MediaPipe best.
constexpr float kCropScale = 1.25f;

class Landmarker {
public:
  // Loads face_landmarks_detector.tflite. Returns false if it can't be read.
  bool load(const std::string &modelPath);

  // Returns kNumLandmarks points in full-frame pixel coordinates, or an empty
  // vector if the frame or box is empty, the box is too small, or the model
  // isn't loaded.
  std::vector<cv::Point3f> detect(const cv::Mat &bgrFrame,
                                  const cv::Rect &faceBox);

private:
  cv::dnn::Net net_;
};

} // namespace vision
