#include "tile_pool.h"

TilePool::TilePool(const int width, const int height, const int tileSize) {
	int xTilesCount = width / tileSize;
	int yTilesCount = height / tileSize;
	bool extraTilesX = width - xTilesCount * tileSize != 0;
	bool extraTilesY = height - yTilesCount * tileSize != 0;

	for (unsigned int y = 1; y <= yTilesCount; y++)
	{
		int yStart = height - y * tileSize;
		int yEnd = yStart + tileSize;
		for (unsigned int x = 0; x < xTilesCount; x++)
		{
			int xStart = x * tileSize;
			int xEnd = xStart + tileSize;
			tilePool.push(Tile{ xStart, xEnd, yStart, yEnd });
		}
	}

	if (extraTilesY)
	{
		int yStart = 0;
		int yEnd = height - yTilesCount * tileSize;
		for (unsigned int x = 0; x < xTilesCount; x++)
		{
			int xStart = x * tileSize;
			int xEnd = xStart + tileSize;
			tilePool.push(Tile{ xStart, xEnd, yStart, yEnd });
		}
	}

	if (extraTilesX)
	{
		int xStart = xTilesCount * tileSize;
		int xEnd = xStart + tileSize;
		for (unsigned int y = 1; y <= yTilesCount; y++)
		{
			int yStart = height - y * tileSize;
			int yEnd = yStart + tileSize;
			tilePool.push(Tile{ xStart, xEnd, yStart, yEnd });
		}
	}

	int xStart = xTilesCount * tileSize;
	int yEnd = height - yTilesCount * tileSize;
	tilePool.push(Tile{ xStart, width, 0, yEnd });

}

int TilePool::getPoolSize() {
	std::lock_guard<std::mutex> lock(readLock);
	int poolSize = tilePool.size();
	return poolSize;
}

Tile TilePool::getNextTile() {
	std::lock_guard<std::mutex> lock(writeLock);
	Tile tile = tilePool.front();
	tilePool.pop();
	return tile;
}