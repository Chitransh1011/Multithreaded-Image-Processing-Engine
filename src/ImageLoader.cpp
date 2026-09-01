#include "ImageLoader.h"

#include <opencv2/imgcodecs.hpp>

#include <stdexcept>

namespace imaging {

cv::Mat ImageLoader::load(const std::string& imagePath) const {
    if (imagePath.empty()) {
        throw std::runtime_error("Input image path must not be empty.");
    }

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error(
            "Could not load image '" + imagePath + "'. Check that the file exists and is a supported image format.");
    }

    return image;
}

}  // namespace imaging
