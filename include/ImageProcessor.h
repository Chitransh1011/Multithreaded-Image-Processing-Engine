#pragma once

#include <opencv2/core/mat.hpp>

namespace imaging {

struct ProcessingConfig {
    double resizeScale = 1.0;
    int blurKernelSize = 5;
    double thresholdValue = 100.0;
    double cannyLowerThreshold = 50.0;
    double cannyUpperThreshold = 150.0;
};

class ImageProcessor {
public:
    explicit ImageProcessor(ProcessingConfig config = {});

    // Produces a single-channel edge image from one independently processable region.
    cv::Mat process(const cv::Mat& input) const;

private:
    ProcessingConfig config_;

    void validateConfig() const;
};

}  // namespace imaging
