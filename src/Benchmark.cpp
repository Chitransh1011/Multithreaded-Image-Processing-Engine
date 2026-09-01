#include "Benchmark.h"

#include "ImageTiler.h"

#include <array>
#include <chrono>
#include <stdexcept>
#include <utility>

namespace imaging {

Benchmark::Benchmark(Pipeline pipeline) : pipeline_(std::move(pipeline)) {}

BenchmarkReport Benchmark::run(const cv::Mat& image, int tileSize) const {
    if (image.empty()) {
        throw std::invalid_argument("Cannot benchmark an empty image.");
    }

    const ImageTiler tiler;
    const std::size_t tileCount = tiler.createTiles(image.cols, image.rows, tileSize).size();

    // Keep one-time OpenCV initialization out of the measured baseline.
    const cv::Mat warmupOutput = pipeline_.processSequential(image, tileSize);
    if (warmupOutput.empty()) {
        throw std::runtime_error("Benchmark warm-up produced an empty result.");
    }

    const auto sequentialStart = std::chrono::steady_clock::now();
    const cv::Mat sequentialOutput = pipeline_.processSequential(image, tileSize);
    const auto sequentialEnd = std::chrono::steady_clock::now();
    if (sequentialOutput.empty()) {
        throw std::runtime_error("Sequential processing produced an empty result.");
    }

    const double sequentialMilliseconds = std::chrono::duration<double, std::milli>(
        sequentialEnd - sequentialStart).count();

    BenchmarkReport report{tileCount, sequentialMilliseconds, {}};
    constexpr std::array<std::size_t, 4> workerCounts{1, 2, 4, 8};
    report.parallelResults.reserve(workerCounts.size());

    for (const std::size_t workerCount : workerCounts) {
        const auto parallelStart = std::chrono::steady_clock::now();
        const cv::Mat parallelOutput = pipeline_.processParallel(image, workerCount, tileSize);
        const auto parallelEnd = std::chrono::steady_clock::now();
        if (parallelOutput.empty()) {
            throw std::runtime_error("Parallel processing produced an empty result.");
        }

        const double parallelMilliseconds = std::chrono::duration<double, std::milli>(
            parallelEnd - parallelStart).count();
        const double speedup = parallelMilliseconds > 0.0
                                   ? sequentialMilliseconds / parallelMilliseconds
                                   : 0.0;
        report.parallelResults.push_back({workerCount, parallelMilliseconds, speedup});
    }

    return report;
}

}  // namespace imaging
