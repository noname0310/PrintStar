#pragma once

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <assert.h>
#include <stdarg.h>
#include <locale.h>

struct _Console; 
typedef struct _Console _Console;

enum CursorStat;
typedef enum CursorStat CursorStat;

enum CodePage;
typedef enum CodePage CodePage;

//Struct _Console
struct _Console {
	void (*const write_single)(const wchar_t ch);
	void (*const set_cursor_vis)(CursorStat stat);
	void (*const set_pos)(short x, short y);
	void (*const set_size)(int x, int y);
	void (*const default_init)();
	void (*const font_normalize)(int size);
	void (*const clear)();
	void (*const pause)();
};

extern const _Console Console;

//Enum ConsoleCursorStat
enum CursorStat {
	CursorStat_hide = 0,
	CursorStat_show = 1
};

enum CodePage {
	CodePage_Default = 0,
	CodePage_Utf8 = 65001
};
