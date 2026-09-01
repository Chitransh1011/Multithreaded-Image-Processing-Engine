cmake_minimum_required(VERSION 3.16)

project(MultithreadedImageProcessingEngine
    VERSION 0.1.0
    DESCRIPTION "High-performance tiled image processing engine"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(image_processor
    src/main.cpp
    src/ImageLoader.cpp
    src/ImageTiler.cpp
    src/ImageProcessor.cpp
)

target_include_directories(image_processor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(image_processor PRIVATE /W4 /permissive-)
else()
    target_compile_options(image_processor PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)

target_link_libraries(image_processor PRIVATE ${OpenCV_LIBS})
