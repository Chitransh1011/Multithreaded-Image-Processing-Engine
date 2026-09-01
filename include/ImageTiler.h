#pragma once

#include <vector>

namespace imaging {

struct Tile {
    int id;
    int x;
    int y;
    int width;
    int height;
};

class ImageTiler {
public:
    static constexpr int kDefaultTileSize = 512;

    // Produces non-overlapping tiles that fully cover the image dimensions.
    // Edge tiles may be smaller than tileSize.
    std::vector<Tile> createTiles(int imageWidth,
                                  int imageHeight,
                                  int tileSize = kDefaultTileSize) const;
};

}  // namespace imaging
