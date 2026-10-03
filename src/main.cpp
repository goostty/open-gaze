#include "vision/vision.hpp"

int main() {
  vision::VisionSystem vs;
  if (!vs.init("../models/"))
    return -1;

  while (vs.step()) {
  }

  vs.shutdown();
  return 0;
}
