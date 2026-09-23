#include <opencv2/opencv.hpp>

int main() {
  cv::VideoCapture cap(0); // 0 = default camera
  if (!cap.isOpened()) {
    std::cerr << "Could not open camera" << std::endl;
    return -1;
  }

  cv::Mat frame;
  cv::Mat gray;
  while (true) {
    cap >> frame; // grab a frame
    if (frame.empty())
      break;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("FaceBreak - Camera Feed", frame);
    cv::imshow("FaceBreak - Camera Feed", gray);
    if (cv::waitKey(1) == 'q')
      break; // press q to quit
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
