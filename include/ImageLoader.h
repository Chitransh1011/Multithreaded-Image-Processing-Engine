#pragma once

#include <opencv2/core/mat.hpp>

#include <string>

namespace imaging {

class ImageLoader {
public:
    // Loads original image data without applying transformations.
    // Throws std::runtime_error if the path cannot be decoded as an image.
    cv::Mat load(const std::string& imagePath) const;
};

}  // namespace imaging
