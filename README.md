# Multithreaded Image Processing Engine

High-performance C++17 image-processing engine that partitions large images into independent tiles, applies an OpenCV pipeline, and executes work sequentially or through a custom thread pool.

> This is an engineering and image-processing project. It does not diagnose disease, make clinical decisions, or represent medical-device software.

## Features

- OpenCV image loading with clear failures for empty, missing, malformed, and unsupported inputs.
- Edge-safe 512 x 512 tiling that preserves complete coverage for non-divisible image dimensions.
- Modular processing pipeline: optional resize, grayscale conversion, Gaussian blur, binary threshold, and Canny edge detection.
- Reusable C++17 thread pool using `std::thread`, `std::mutex`, `std::condition_variable`, `std::future`, and `std::packaged_task`.
- Sequential baseline and configurable parallel tile processing through `--threads <count>`.
- Race-free result reconstruction and PNG output at the original image dimensions.
- Measured benchmark mode for sequential and 1/2/4/8-worker configurations.
- Lightweight CTest suite covering tiling, processing, concurrency, and parallel-result correctness.

## Architecture

```text
                         +-------------------+
Input image -----------> | ImageLoader       |
                         +---------+---------+
                                   |
                         +---------v---------+
                         | ImageTiler         |
                         +---------+---------+
                                   |
                    +--------------+--------------+
                    |                             |
          Sequential Pipeline                 ThreadPool
                    |                             |
                    |                  +----------+----------+
                    |                  | Worker tile tasks   |
                    |                  +----------+----------+
                    +--------------+--------------+
                                   |
                         +---------v---------+
                         | Reconstruction    |
                         +---------+---------+
                                   |
                              output/result.png
```

Each task processes a distinct source tile and returns an independent result. The calling thread reconstructs results after futures complete, so workers do not write to the shared output image.

## Requirements

- CMake 3.16+
- C++17-compatible compiler (Visual Studio Build Tools 2019+ recommended on Windows)
- OpenCV 4.x with `core`, `imgcodecs`, and `imgproc`

Set `OpenCV_DIR` to the directory containing `OpenCVConfig.cmake`. For the official Windows OpenCV package, this is typically similar to:

```text
<opencv>/build/x64/vc16/lib
```

## Build on Windows

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="<opencv>/build/x64/vc16/lib"
cmake --build build --config Debug
```

Before running, make the OpenCV DLL directory available on `PATH`:

```powershell
$env:PATH = "<opencv>/build/x64/vc16/bin;$env:PATH"
```

## Usage

```powershell
# Sequential tiled processing
.\build\Debug\image_processor.exe <image-path>

# Parallel tiled processing
.\build\Debug\image_processor.exe --threads 4 <image-path>

# Actual local benchmark measurements
.\build\Debug\image_processor.exe --benchmark <image-path>
```

Normal and parallel modes write the reconstructed edge image to `output/result.png`.

## Benchmarking

Benchmark mode performs one untimed warm-up, then measures sequential processing and parallel processing with 1, 2, 4, and 8 workers. Speedup is calculated as:

```text
speedup = sequential time / parallel time
```

Use a representative high-resolution image with multiple tiles. Results vary with hardware, build configuration, image content, tile size, and background system activity, so this repository intentionally contains no fabricated benchmark numbers.

## Tests

```powershell
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The test suite validates tile boundaries, processor input/configuration errors, thread-pool futures, and identical sequential versus parallel output for a multi-tile image with clipped edge tiles.

## Design Decisions

- **Tiles carry metadata, not copied image data.** Tile coordinates and dimensions are lightweight; workers create `cv::Mat` views only when processing begins.
- **A fixed thread pool avoids per-tile thread creation.** Idle workers sleep on a condition variable and wake only when work is available or shutdown begins.
- **Futures separate work from reconstruction.** Workers produce local results; reconstruction occurs after `future::get`, which keeps output writes deterministic and race-free.
- **Sequential processing remains available.** It provides a correctness reference and a meaningful baseline for performance comparisons.

## Relevance to High-Performance Imaging Systems

This project demonstrates transferable skills for data-heavy imaging software: modern C++ systems programming, image-processing pipelines, high-resolution tiling, concurrent task execution, performance measurement, and modular architecture. It does not claim clinical, diagnostic, scanner-firmware, robotics, or production medical-device experience.

## Future Improvements

- Multi-resolution image pyramids
- Tile cache with an LRU eviction policy
- Dynamic task scheduling and adaptive tile sizes
- SIMD or GPU acceleration
- Interactive image viewer or service API
