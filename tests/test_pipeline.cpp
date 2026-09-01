#include "Pipeline.h"
#include "TestHelpers.h"

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(513, 1025, CV_8UC3);
    cv::randu(image, 0, 256);

    imaging::Pipeline pipeline;
    const cv::Mat sequential = pipeline.processSequential(image);
    const cv::Mat parallel = pipeline.processParallel(image, 4);

    test::expect(sequential.size() == image.size(), "sequential reconstruction should preserve dimensions");
    test::expect(parallel.size() == image.size(), "parallel reconstruction should preserve dimensions");
    test::expect(sequential.type() == CV_8UC1 && parallel.type() == CV_8UC1,
                 "reconstructed outputs should be single-channel edge images");
    test::expect(cv::norm(sequential, parallel, cv::NORM_INF) == 0.0,
                 "sequential and parallel tile outputs should match exactly");
    return test::finish();
}
