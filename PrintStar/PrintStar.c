#include "Graphics.h"

int main()
{
	const int width = 160 * 5;
	const int height = 90 * 5;
	const int point_max = 50;
	const float pi = 3.14159265358979323846f;

	Console.default_init();
	Console.font_normalize(1);
	Console.set_size(width, height);
	Console.set_cursor_vis(CursorStat_hide);
	Graphics graphics = Graphics_new(width, height);

	int radius = min(height, width) / 2;
	int screen_center_x = width / 2;
	int screen_center_y = height / 2;

	Point* points = (Point*)malloc(sizeof(Point) * ((size_t)point_max + 2));
	assert(points != NULL);
	for (int star_point = 3; star_point < point_max; star_point+=2)
	{
		int point_counter = 0;
		for (int i = 0; i <= 360; i++)
		{
			if (!((i - 90) % (360 / star_point)))
			{
				Point point = Point_new(
					screen_center_x + (int)(cos(i * pi / 180) * radius),
					screen_center_y + (int)(sin(i * pi / 180) * radius)
				);
				OOPTool.set_const_value(&points[point_counter++], &point, sizeof(Point));
				graphics.f->draw_single(&graphics, L'&', point, 1);
			}
		}
		for (int i = 0; i < star_point; i++)
			graphics.f->draw_line(&graphics, L'*', points[i], points[(i + 2) % star_point]);
		graphics.f->print((FrameBuffer*)&graphics);
		graphics.f->clear((FrameBuffer*)&graphics);
	}
	free(points);
	graphics.f->dispose((FrameBuffer*)&graphics);
	Console.pause();
	return 0;
}