#pragma once

#include "Pipeline.h"

#include <cstddef>
#include <vector>

namespace imaging {

struct ParallelBenchmarkResult {
    std::size_t workerCount;
    double elapsedMilliseconds;
    double speedup;
};

struct BenchmarkReport {
    std::size_t tileCount;
    double sequentialMilliseconds;
    std::vector<ParallelBenchmarkResult> parallelResults;
};

class Benchmark {
public:
    explicit Benchmark(Pipeline pipeline = Pipeline{});

    BenchmarkReport run(const cv::Mat& image,
                        int tileSize = ImageTiler::kDefaultTileSize) const;

private:
    Pipeline pipeline_;
};

}  // namespace imaging
