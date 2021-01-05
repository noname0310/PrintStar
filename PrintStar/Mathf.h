#pragma once

#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Windows.h>

struct _Mathf;
typedef struct _Mathf _Mathf;

struct _Mathf {
	const float PI;
	const float Infinity;
	const float NegativeInfinity;
	const float Deg2Rad;
	const float Rad2Deg;
	float (*const sin)(float f);
	float (*const cos)(float f);
	float (*const tan)(float f);
	float (*const asin)(float f);
	float (*const acos)(float f);
	float (*const atan)(float f);
	float (*const atan2)(float y, float x);
	float (*const sqrt)(float f);
	float (*const abs)(float f);
	int (*const abs_i32)(int f);
	float (*const min)(float a, float b);
	int (*const min_i32)(int a, int b);
	float (*const max)(float a, float b);
	int (*const max_i32)(int a, int b);
	float (*const pow)(float f, float p);
	float (*const exp)(float power);
	float (*const log)(float f, float p);
	float (*const log_e)(float f);
	float (*const log_10)(float f);
	float (*const ceil)(float f);
	float (*const floor)(float f);
	float (*const round)(float f);
	int (*const ceil_to_int)(float f);
	int (*const floor_to_int)(float f);
	int (*const round_to_int)(float f);
};

extern const _Mathf Mathf;
