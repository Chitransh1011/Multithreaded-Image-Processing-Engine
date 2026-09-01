#include "Benchmark.h"
#include "ImageLoader.h"
#include "ImageTiler.h"
#include "Pipeline.h"

#include <opencv2/imgcodecs.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

struct CommandLineOptions {
    std::string imagePath;
    std::size_t workerCount = 1;
    bool runBenchmark = false;
};

CommandLineOptions parseCommandLine(int argc, char* argv[]) {
    if (argc == 2) {
        return {argv[1], 1, false};
    }
    if (argc == 3 && std::string(argv[1]) == "--benchmark") {
        return {argv[2], 1, true};
    }
    if (argc == 4 && std::string(argv[1]) == "--threads") {
        const std::string workerCountText = argv[2];
        std::size_t parsedCharacters = 0;
        const unsigned long long parsedWorkerCount = std::stoull(workerCountText, &parsedCharacters);
        if (parsedCharacters != workerCountText.size() || parsedWorkerCount == 0) {
            throw std::invalid_argument("Thread count must be a positive integer.");
        }

        return {argv[3], static_cast<std::size_t>(parsedWorkerCount), false};
    }

    throw std::invalid_argument(
        "Usage: image_processor [--threads <count> | --benchmark] <image-path>");
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        const CommandLineOptions options = parseCommandLine(argc, argv);
        std::cout << "[INFO] Loading image: " << options.imagePath << '\n';

        imaging::ImageLoader loader;
        const cv::Mat image = loader.load(options.imagePath);

        imaging::ImageTiler tiler;
        const std::vector<imaging::Tile> tiles = tiler.createTiles(image.cols, image.rows);

        std::cout << "[INFO] Image loaded successfully\n";
        std::cout << "[INFO] Image size: " << image.cols << "x" << image.rows
                  << ", channels: " << image.channels() << '\n';
        std::cout << "[INFO] Generated " << tiles.size() << " tiles ("
                  << imaging::ImageTiler::kDefaultTileSize << "x"
                  << imaging::ImageTiler::kDefaultTileSize << ")\n";

        if (options.runBenchmark) {
            imaging::Benchmark benchmark;
            const imaging::BenchmarkReport report = benchmark.run(image);
            std::cout << "\nSequential: " << report.sequentialMilliseconds << " ms\n";
            std::cout << "\nParallel:\n";
            for (const imaging::ParallelBenchmarkResult& result : report.parallelResults) {
                std::cout << "  Threads: " << result.workerCount
                          << " | Time: " << result.elapsedMilliseconds
                          << " ms | Speedup: " << result.speedup << "x\n";
            }
            return 0;
        }

        imaging::Pipeline pipeline;
        const bool useParallelProcessing = options.workerCount > 1;
        const cv::Mat output = useParallelProcessing
                                   ? pipeline.processParallel(image, options.workerCount)
                                   : pipeline.processSequential(image);

        const std::filesystem::path outputDirectory = "output";
        std::filesystem::create_directories(outputDirectory);
        const std::filesystem::path outputPath = outputDirectory / "result.png";
        if (!cv::imwrite(outputPath.string(), output)) {
            throw std::runtime_error("Failed to write output image to '" + outputPath.string() + "'.");
        }

        std::cout << "[INFO] Processing mode: "
                  << (useParallelProcessing ? "parallel" : "sequential") << '\n';
        std::cout << "[INFO] Output written to " << outputPath.string() << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "[ERROR] " << error.what() << '\n';
        return 1;
    }
}
