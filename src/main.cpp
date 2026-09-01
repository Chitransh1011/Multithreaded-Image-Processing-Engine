#include "ImageLoader.h"
#include "ImageTiler.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: image_processor <image-path>\n";
        return 1;
    }

    try {
        const std::string imagePath = argv[1];
        std::cout << "[INFO] Loading image: " << imagePath << '\n';

        imaging::ImageLoader loader;
        const cv::Mat image = loader.load(imagePath);

        imaging::ImageTiler tiler;
        const std::vector<imaging::Tile> tiles = tiler.createTiles(image.cols, image.rows);

        std::cout << "[INFO] Image loaded successfully\n";
        std::cout << "[INFO] Image size: " << image.cols << "x" << image.rows
                  << ", channels: " << image.channels() << '\n';
        std::cout << "[INFO] Generated " << tiles.size() << " tiles ("
                  << imaging::ImageTiler::kDefaultTileSize << "x"
                  << imaging::ImageTiler::kDefaultTileSize << ")\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "[ERROR] " << error.what() << '\n';
        return 1;
    }
}
