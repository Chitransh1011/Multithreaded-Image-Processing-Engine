# Multithreaded Image Processing Engine

High-performance C++17 image-processing engine for tiled image workloads. The project uses OpenCV for image I/O, with a modular architecture designed to support concurrent execution and measurable performance analysis.

> Scope: this is an engineering and image-processing project. It does not diagnose disease, make clinical decisions, or represent medical-device software.

## Current capabilities

- Loads colour images with OpenCV.
- Validates empty, missing, malformed, and unsupported input paths.
- Reports image dimensions and channel count through a focused command-line interface.
- Builds with CMake and Visual Studio Build Tools on Windows.

## Build (Windows / Visual Studio Build Tools)

```powershell
$cmake = 'C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
& $cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="tools/opencv/opencv/build/x64/vc16/lib"
& $cmake --build build --config Debug
```

## Run

```powershell
.\build\Debug\image_processor.exe <image-path>
```

## Architecture

```text
Input Image -> Image Loader -> Image Tiler -> Task Queue
                                             |-> Worker threads
                                             v
                                      Processing Pipeline
                                             |
                                      Result Reconstruction -> Output Image
```

## Roadmap

1. Edge-safe image tiling
2. Modular OpenCV processing pipeline
3. Reusable C++ thread pool
4. Sequential and parallel reconstruction
5. Measured benchmarks
6. Tests and project documentation

## Relevance to High-Performance Imaging Systems

The finished engine will demonstrate transferable C++ systems programming, tiled high-resolution image workloads, modular image-processing stages, concurrent task execution, and measured performance trade-offs. It makes no claims of clinical use, diagnostic capability, scanner firmware, or production medical-device deployment.
