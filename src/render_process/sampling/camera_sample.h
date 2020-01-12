#pragma once

class Point2f
{
public:
	Point2f(float x = 0, float y = 0): x(x), y(y) {}
	float x = 0;
	float y = 0;
};

class Point2i
{
public:
	Point2i(int x = 0, int y = 0) : x(x), y(y) {}
	int x = 0;
	int y = 0;
};

class CameraSample
{
public:
	CameraSample() {}
	Point2f pFilm;
	float time = 0;
	Point2f pLens;

};
