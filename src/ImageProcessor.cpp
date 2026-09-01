#include "ImageProcessor.h"

#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace imaging {

ImageProcessor::ImageProcessor(ProcessingConfig config) : config_(config) {
    validateConfig();
}

cv::Mat ImageProcessor::process(const cv::Mat& input) const {
    if (input.empty()) {
        throw std::invalid_argument("Cannot process an empty image.");
    }

    cv::Mat resized;
    if (config_.resizeScale == 1.0) {
        resized = input;
    } else {
        cv::resize(input, resized, cv::Size(), config_.resizeScale, config_.resizeScale, cv::INTER_AREA);
    }

    cv::Mat grayscale;
    if (resized.channels() == 1) {
        grayscale = resized;
    } else {
        cv::cvtColor(resized, grayscale, cv::COLOR_BGR2GRAY);
    }

    cv::Mat blurred;
    cv::GaussianBlur(grayscale, blurred, cv::Size(config_.blurKernelSize, config_.blurKernelSize), 0.0);

    cv::Mat thresholded;
    cv::threshold(blurred, thresholded, config_.thresholdValue, 255.0, cv::THRESH_BINARY);

    cv::Mat edges;
    cv::Canny(thresholded, edges, config_.cannyLowerThreshold, config_.cannyUpperThreshold);
    return edges;
}

void ImageProcessor::validateConfig() const {
    if (config_.resizeScale <= 0.0) {
        throw std::invalid_argument("Resize scale must be positive.");
    }
    if (config_.blurKernelSize <= 0 || config_.blurKernelSize % 2 == 0) {
        throw std::invalid_argument("Blur kernel size must be a positive odd number.");
    }
    if (config_.thresholdValue < 0.0 || config_.thresholdValue > 255.0) {
        throw std::invalid_argument("Threshold value must be between 0 and 255.");
    }
    if (config_.cannyLowerThreshold < 0.0 ||
        config_.cannyUpperThreshold < config_.cannyLowerThreshold) {
        throw std::invalid_argument("Canny thresholds must be non-negative and ordered.");
    }
}

}  // namespace imaging
