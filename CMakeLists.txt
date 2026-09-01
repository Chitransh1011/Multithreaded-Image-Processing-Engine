cmake_minimum_required(VERSION 3.16)

project(MultithreadedImageProcessingEngine
    VERSION 0.1.0
    DESCRIPTION "High-performance tiled image processing engine"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)

add_library(image_engine
    src/ImageLoader.cpp
    src/ImageTiler.cpp
    src/ImageProcessor.cpp
    src/ThreadPool.cpp
    src/Pipeline.cpp
    src/Benchmark.cpp
)

target_include_directories(image_engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(image_engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(image_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(image_engine PUBLIC ${OpenCV_LIBS})
target_include_directories(image_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(image_processor src/main.cpp)
target_link_libraries(image_processor PRIVATE image_engine)

include(CTest)
if(BUILD_TESTING)
    foreach(test_name IN ITEMS test_tiler test_processor test_threadpool test_pipeline)
        add_executable(${test_name} tests/${test_name}.cpp)
        target_link_libraries(${test_name} PRIVATE image_engine)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
