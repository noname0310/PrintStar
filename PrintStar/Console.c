#include "Console.h"

#define MAX_INPUT_LEN 257


static void write_line(const wchar_t str[], ...);
static void write(const wchar_t str[], ...);
static void write_single(const wchar_t ch);

static const wchar_t* read_line();

static void set_cursor_vis(CursorStat stat);
static void set_pos(short x, short y);
static void set_size(int x, int y);
static void set_codepage(CodePage codepage);
static void set_locale2utf8(void);
static void default_init(void);
static void font_normalize(int size);
static void clear(void);
static void pause(void);

const _Console Console = {
	.write_line = write_line,
	.write = write,
	.write_single = write_single,
	.read_line = read_line,

	.set_cursor_vis = set_cursor_vis,
	.set_pos = set_pos,
	.set_size = set_size,
	.set_codepage = set_codepage,
	.set_locale2utf8 = set_locale2utf8,
	.default_init = default_init,
	.font_normalize = font_normalize,
	.clear = clear,
	.pause = pause
};

static void write_line(const wchar_t str[], ...) {
	assert(str != NULL);
	va_list args;
	va_start(args, str); 
	vwprintf(str, args);
	va_end(args);
	wprintf_s(L"\n");
}

static void write(const wchar_t str[], ...) {
	assert(str != NULL);
	va_list args;
	va_start(args, str);
	vwprintf(str, args);
	va_end(args);
}

static void write_single(const wchar_t ch) {
	putwchar(ch);
}

static const wchar_t* read_line() {
	wchar_t wstr[MAX_INPUT_LEN] = { 0 };

	HANDLE conin = GetStdHandle(STD_INPUT_HANDLE);
	assert(conin != INVALID_HANDLE_VALUE);
	bool flush_result = FlushConsoleInputBuffer(conin);
	assert(flush_result != false);
	unsigned long read = 0;
	bool read_result = ReadConsoleW(conin, wstr, MAX_INPUT_LEN, &read, NULL);
	assert(read_result != false);
	int readbyte = read * sizeof(wchar_t);
	
	if (wstr[read - 2] == L'\r') {
		wstr[read - 2] = L'\0';
		wchar_t* result = malloc((size_t)readbyte - 1);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, (size_t)readbyte - 1, wstr, (size_t)readbyte - 1);
		return result;
	}
	else if (wstr[read - 1] == '\n') {
		wstr[read - 1] = L'\0';
		wchar_t* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
	else if (wstr[read - 1] != '\0') {
		wchar_t* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
	else {
		wchar_t* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
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

static void set_codepage(CodePage codepage) {
	SetConsoleOutputCP(codepage);
	SetConsoleCP(codepage);
}

static void set_locale2utf8() {
	setlocale(LC_ALL, ".UTF8");
}

static void default_init() {
	set_codepage(CodePage_Utf8);
	set_locale2utf8();
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
