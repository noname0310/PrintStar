#pragma once

#include <assert.h>

struct Point;
typedef struct Point Point;

struct Point {
	int x;
	int y;
};

Point Point_new(int x, int y);