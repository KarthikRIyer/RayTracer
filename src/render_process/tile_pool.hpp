#pragma once

#include <queue>
#include <mutex>

struct Tile
{
	const int xMin, xMax, yMin, yMax;
};

class TilePool {
public:
	TilePool();
	TilePool(const int width, const int height, const int tileSize);
	void initializeTilePool(const int width, const int height, const int tileSize);
	int getPoolSize();
	Tile getNextTile();

private:
	std::mutex readLock;
	std::mutex writeLock;
	std::queue<Tile> tilePool;
	void processTiles(const int width, const int height, const int tileSize);
};
