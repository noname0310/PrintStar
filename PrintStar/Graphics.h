#pragma once

#include <math.h>
#include "FrameBuffer.h"

struct impl_Graphics;
typedef struct impl_Graphics impl_Graphics;

struct Graphics;
typedef struct Graphics Graphics;

struct impl_Graphics {
	impl_FrameBuffer_Members
		
	void (*const draw_single)(const Graphics* self, wchar_t ch, Point a, int thickness);
	void (*const draw_line)(const Graphics* self, wchar_t ch, Point a, Point b);
};

struct Graphics {
	const impl_Graphics* const f;
	FrameBuffer_Members
};

Graphics Graphics_new(int width, int height);
const impl_Graphics* get_impl_Graphics_table();
