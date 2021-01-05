#include "Console.h"

static void write_single(const wchar_t ch);
static void set_cursor_vis(CursorStat stat);
static void set_pos(short x, short y);
static void set_size(int x, int y);
static void default_init(void);
static void font_normalize(int size);
static void clear(void);
static void pause(void);

const _Console Console = {
	.write_single = write_single,
	.set_cursor_vis = set_cursor_vis,
	.set_pos = set_pos,
	.set_size = set_size,
	.default_init = default_init,
	.font_normalize = font_normalize,
	.clear = clear,
	.pause = pause
};

static void write_single(const wchar_t ch) {
	putwchar(ch);
}

static void set_cursor_vis(CursorStat stat) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = stat;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

static void set_pos(short x, short y) {
	COORD pos = {
		.X = x,
		.Y = y
	};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void set_size(int x, int y) {
	char str[44];
	sprintf_s(str, sizeof(str), "mode con cols=%d lines=%d", x, y);

	system(str);
}

static void default_init() {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, ".UTF8");
}

static void font_normalize(int size) {
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	const wchar_t myFont[] = L"GulimChe";
	fontInfo.dwFontSize.X = size;
	fontInfo.dwFontSize.Y = size;
	memcpy(fontInfo.FaceName, myFont, sizeof(myFont));
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontInfo);
}

static void clear() {
	system("cls");
}

static void pause() {
	system("pause");
}
