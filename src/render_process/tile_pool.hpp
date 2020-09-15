#pragma once

#include <queue>
#include <mutex>

struct Tile {
    const int xMin, xMax, yMin, yMax;
};

class TilePool {
public:
    TilePool(int width, int height, int tileSize);

    size_t getPoolSize();

    Tile getNextTile();

private:
    std::mutex readLock;
    std::mutex writeLock;
    std::queue<Tile> tilePool;
};