#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <string>

namespace vision {

class VisionSystem {
public:
  // Opens the camera and loads the face detector from modelDir.
  // Returns false if either fails.
  bool init(const std::string &modelDir);

  // Grabs one frame, detects faces, draws and shows it.
  // Returns false when the loop should stop (camera ended or 'q' pressed).
  bool step();

  // Releases the camera and closes windows.
  void shutdown();

private:
  cv::VideoCapture cap_;
  cv::dnn::Net net_;
};

} // namespace vision
