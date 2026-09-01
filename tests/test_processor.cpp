#include "ImageProcessor.h"
#include "TestHelpers.h"

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(32, 32, CV_8UC3);
    cv::randu(image, 0, 256);

    imaging::ImageProcessor processor;
    const cv::Mat output = processor.process(image);
    test::expect(!output.empty(), "processor output should not be empty");
    test::expect(output.size() == image.size(), "processor should preserve dimensions at default scale");
    test::expect(output.type() == CV_8UC1, "processor should produce a one-channel edge image");
    test::expectThrows([&] { processor.process(cv::Mat{}); }, "empty input should be rejected");

    imaging::ProcessingConfig invalidConfig;
    invalidConfig.blurKernelSize = 4;
    test::expectThrows([&] { imaging::ImageProcessor{invalidConfig}; },
                       "even blur kernel should be rejected");
    return test::finish();
}
