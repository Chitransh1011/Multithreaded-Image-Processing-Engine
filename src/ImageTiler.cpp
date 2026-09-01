#include "ImageTiler.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace imaging {

std::vector<Tile> ImageTiler::createTiles(int imageWidth, int imageHeight, int tileSize) const {
    if (imageWidth <= 0 || imageHeight <= 0) {
        throw std::invalid_argument("Image dimensions must be positive.");
    }
    if (tileSize <= 0) {
        throw std::invalid_argument("Tile size must be positive.");
    }

    const int tilesAcross = (imageWidth + tileSize - 1) / tileSize;
    const int tilesDown = (imageHeight + tileSize - 1) / tileSize;
    std::vector<Tile> tiles;
    tiles.reserve(static_cast<std::size_t>(tilesAcross) * static_cast<std::size_t>(tilesDown));

    int tileId = 0;
    for (int y = 0; y < imageHeight; y += tileSize) {
        for (int x = 0; x < imageWidth; x += tileSize) {
            tiles.push_back({
                tileId++,
                x,
                y,
                std::min(tileSize, imageWidth - x),
                std::min(tileSize, imageHeight - y),
            });
        }
    }

    return tiles;
}

}  // namespace imaging
