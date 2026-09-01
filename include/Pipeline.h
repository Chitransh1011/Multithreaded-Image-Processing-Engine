#pragma once

#include "ImageProcessor.h"
#include "ImageTiler.h"

#include <cstddef>

#include <opencv2/core/mat.hpp>

namespace imaging {

class Pipeline {
public:
    explicit Pipeline(ImageProcessor processor = ImageProcessor{});

    cv::Mat processSequential(const cv::Mat& image,
                              int tileSize = ImageTiler::kDefaultTileSize) const;
    cv::Mat processParallel(const cv::Mat& image,
                            std::size_t workerCount,
                            int tileSize = ImageTiler::kDefaultTileSize) const;

private:
    struct ProcessedTile {
        Tile metadata;
        cv::Mat image;
    };

    cv::Mat createOutputImage(const cv::Mat& input) const;
    void copyTileToOutput(const ProcessedTile& tile, cv::Mat& output) const;

    ImageTiler tiler_;
    ImageProcessor processor_;
};

}  // namespace imaging
