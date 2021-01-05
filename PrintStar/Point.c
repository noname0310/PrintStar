#include "Point.h"

Point Point_new(int x, int y) {
	Point instance = {
		.x = x,
		.y = y
	};

	return instance;
}
