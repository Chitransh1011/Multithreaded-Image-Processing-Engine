#include "ImageTiler.h"
#include "TestHelpers.h"

#include <vector>

int main() {
    imaging::ImageTiler tiler;

    const std::vector<imaging::Tile> tiles = tiler.createTiles(1025, 513);
    test::expect(tiles.size() == 6, "1025x513 image should produce six 512-pixel tiles");
    test::expect(tiles.front().id == 0 && tiles.front().width == 512 && tiles.front().height == 512,
                 "first tile should cover the upper-left 512x512 region");

    const imaging::Tile& edge = tiles.back();
    test::expect(edge.id == 5 && edge.x == 1024 && edge.y == 512 &&
                     edge.width == 1 && edge.height == 1,
                 "final edge tile should be clipped to 1x1");

    const std::vector<imaging::Tile> smallImageTiles = tiler.createTiles(10, 8);
    test::expect(smallImageTiles.size() == 1 && smallImageTiles.front().width == 10 &&
                     smallImageTiles.front().height == 8,
                 "an image smaller than one tile should produce one correctly sized tile");

    test::expectThrows([&] { tiler.createTiles(0, 10); }, "zero image width should be rejected");
    test::expectThrows([&] { tiler.createTiles(10, 10, 0); }, "zero tile size should be rejected");
    return test::finish();
}
