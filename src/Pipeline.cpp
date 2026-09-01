#include "Pipeline.h"

#include "ThreadPool.h"

#include <opencv2/core/types.hpp>

#include <future>
#include <stdexcept>
#include <utility>
#include <vector>

namespace imaging {

Pipeline::Pipeline(ImageProcessor processor) : processor_(std::move(processor)) {}

cv::Mat Pipeline::processSequential(const cv::Mat& image, int tileSize) const {
    if (image.empty()) {
        throw std::invalid_argument("Cannot process an empty image.");
    }

    const std::vector<Tile> tiles = tiler_.createTiles(image.cols, image.rows, tileSize);
    cv::Mat output = createOutputImage(image);

    for (const Tile& tile : tiles) {
        const cv::Rect region(tile.x, tile.y, tile.width, tile.height);
        const cv::Mat processed = processor_.process(image(region));
        copyTileToOutput({tile, processed}, output);
    }

    return output;
}

cv::Mat Pipeline::processParallel(const cv::Mat& image,
                                  std::size_t workerCount,
                                  int tileSize) const {
    if (image.empty()) {
        throw std::invalid_argument("Cannot process an empty image.");
    }

    const std::vector<Tile> tiles = tiler_.createTiles(image.cols, image.rows, tileSize);
    ThreadPool pool(workerCount);
    std::vector<std::future<ProcessedTile>> results;
    results.reserve(tiles.size());

    for (const Tile& tile : tiles) {
        results.emplace_back(pool.submit([this, &image, tile] {
            const cv::Rect region(tile.x, tile.y, tile.width, tile.height);
            return ProcessedTile{tile, processor_.process(image(region))};
        }));
    }

    cv::Mat output = createOutputImage(image);
    for (std::future<ProcessedTile>& result : results) {
        copyTileToOutput(result.get(), output);
    }

    return output;
}

cv::Mat Pipeline::createOutputImage(const cv::Mat& input) const {
    return cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
}

void Pipeline::copyTileToOutput(const ProcessedTile& tile, cv::Mat& output) const {
    const cv::Rect destination(tile.metadata.x,
                               tile.metadata.y,
                               tile.metadata.width,
                               tile.metadata.height);
    if (tile.image.size() != destination.size() || tile.image.type() != output.type()) {
        throw std::runtime_error("Processed tile dimensions do not match its destination region.");
    }

    tile.image.copyTo(output(destination));
}

}  // namespace imaging
