#include "Graphics.h"

static void draw_single(const Graphics* self, wchar_t ch, Point a, int thickness);
static void draw_line(const Graphics* self, wchar_t ch, Point a, Point b);

const impl_Graphics* get_impl_Graphics_table() {
	static impl_Graphics impl_Graphics_table = {
		.draw_single = draw_single,
		.draw_line = draw_line
	};

	if (*(void**)&impl_Graphics_table == NULL) {
		OOPTool.set_const_value(&impl_Graphics_table, get_impl_FrameBuffer_table(), sizeof(impl_FrameBuffer));
	}

	return &impl_Graphics_table;
}

Graphics Graphics_new(int width, int height) {
	FrameBuffer parent_instance = FrameBuffer_new(width, height);

	Graphics instance = {
		.f = get_impl_Graphics_table()
	};

	OOPTool.set_const_value((size_t*)(&instance) + 1, (size_t*)(&parent_instance) + 1, sizeof(FrameBuffer) - sizeof(size_t));
	return instance;
}

static void draw_single(const Graphics* self, wchar_t ch, Point a, int thickness) {
	assert(self != NULL);
	assert(!(thickness <= 0) || !"thickness must be higher than 0");

	if (thickness == 1) {
		self->f->set_pixel((FrameBuffer*)self, a.x, a.y, ch);
		return;
	}

	thickness -= 1;

	int xk = 0;
	int yk = thickness;
	int pk = 3 - (thickness + thickness);

	do {
		int axkt = a.x + xk;
		for (int i = a.x - xk; i <= axkt; i++) {
			self->f->set_pixel((FrameBuffer*)self, i, a.y - yk, ch);
			self->f->set_pixel((FrameBuffer*)self, i, a.y + yk, ch);
		}
		int aykt = a.y + xk;
		for (int i = a.y - xk; i <= aykt; i++) {
			self->f->set_pixel((FrameBuffer*)self, a.x - yk, i, ch);
			self->f->set_pixel((FrameBuffer*)self, a.x + yk, i, ch);
		}

		xk += 1;
		if (pk < 0) pk += (xk << 2) + 6;
		else {
			yk -= 1;
			pk += ((xk - yk) << 2) + 10;

		}

	} while (xk <= yk);

	int axxkm1 = a.x + xk - 1;
	int ayyk = a.y + yk;

	for (int i = a.y - yk; i <= ayyk; i++) {
		for (int j = a.x - xk + 1; j <= axxkm1; j++) {
			self->f->set_pixel((FrameBuffer*)self, j, i, ch);
		}
	}
}

//https://gist.github.com/bert/1085538
static void draw_line(const Graphics* self, wchar_t ch, Point a, Point b) {
	assert(self != NULL);

	int dx = abs(b.x - a.x);
	int sx = a.x < b.x ? 1 : -1;
	int dy = -abs(b.y - a.y);
	int sy = a.y < b.y ? 1 : -1;
	int err = dx + dy, e2;

	for( ; ;) {
		self->f->set_pixel((FrameBuffer*)self, a.x, a.y, ch);
		if (a.x == b.x && a.y == b.y) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; a.x += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; a.y += sy; } /* e_xy+e_y < 0 */
	}
}