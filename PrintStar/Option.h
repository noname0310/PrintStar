#pragma once

#define decl_Option(T, declname) \
\
typedef struct { \
    Option option; \
    T value; \
} Option_##declname; \
\
Option_##declname Option_##declname##_some(T val); \
Option_##declname Option_##declname##_none();

#define def_Option(T, declname) \
 \
Option_##declname Option_##declname##_some(T val) \
{ \
    Option_##declname instance = { \
        .option = Some, \
        .value = val \
    }; \
    return instance; \
} \
 \
Option_##declname Option_##declname##_none() \
{ \
    Option_##declname instance = { \
        .option = None, \
        .value = 0 \
    }; \
    return instance; \
}

enum _Option;
typedef enum _Option Option;

enum _Option {
    None = 0,
    Some = 1
};

decl_Option(char, char)
decl_Option(wchar_t, wchar_t)
decl_Option(short, short)
decl_Option(int, int)
decl_Option(long long int, int64)
decl_Option(float, float)
decl_Option(double, double)
decl_Option(const wchar_t*, c_str)
