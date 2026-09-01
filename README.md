# Multithreaded Image Processing Engine

High-performance C++ image-processing engine built around tiled workloads and a reusable thread pool. The project is being developed incrementally for correctness, measurable performance, and interview-ready code quality.

> Scope: this is an engineering and image-processing project. It does not diagnose disease, make clinical decisions, or represent medical-device software.

## Current status

Phase 1 is complete: a portable C++17/CMake project skeleton builds a minimal executable. Image loading, OpenCV integration, tiling, the processing pipeline, and concurrency are intentionally introduced in later verified phases.

## Build (Windows / MinGW)

```powershell
$cmake = 'C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
& $cmake -S . -B build -G "MinGW Makefiles"
& $cmake --build build
```

## Run

```powershell
.\build\image_processor.exe
```

## Planned architecture

```text
Input Image -> Image Loader -> Image Tiler -> Task Queue
                                             |-> Worker threads
                                             v
                                      Processing Pipeline
                                             |
                                      Result Reconstruction -> Output Image
```

## Development phases

1. Build system - complete
2. OpenCV image loading and validation
3. Edge-safe image tiling
4. Modular OpenCV processing pipeline
5. Reusable C++ thread pool
6. Sequential and parallel reconstruction
7. Measured benchmarks
8. Tests and cleanup
9. Complete project documentation

## Relevance to High-Performance Imaging Systems

The finished engine will demonstrate transferable C++ systems programming, tiled high-resolution image workloads, modular image-processing stages, concurrent task execution, and measured performance trade-offs. It makes no claims of clinical use, diagnostic capability, scanner firmware, or production medical-device deployment.

