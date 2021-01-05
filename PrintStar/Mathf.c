#include "Mathf.h"

#undef min
#undef max

static float sin_value (float f);
static float cos_value (float f);
static float tan_value (float f);
static float asin_value (float f);
static float acos_value (float f);
static float atan_value (float f);
static float atan2_value (float y, float x);
static float sqrt_value (float f);
static float abs_value (float f);
static int abs_i32 (int f);
static float min(float a, float b);
static int min_i32(int a, int b);
static float max(float a, float b);
static int max_i32(int a, int b);
static float pow_value (float f, float p);
static float exp_value (float power);
static float log_value (float f, float p);
static float log_e (float f);
static float log_10 (float f);
static float ceil_value (float f);
static float floor_value (float f);
static float round (float f);
static int ceil_to_int (float f);
static int floor_to_int (float f);
static int round_to_int (float f);

static float frexpf(float, int*);
static float ldexpf(float, int);
static float polevlf(float xx, float* coef, int N);

//https://github.com/jeremybarnes/cephes/blob/master/single/sinf.c
const static float MAXNUMF = 3.4028234663852885981170418348451692544e38f;
const static float FOPI = 1.27323954473516f;
const static float PIF = 3.14159265358979323846264338328f;
const static float PIO2F = 1.57079632679489661923132169164f;
const static float PIO4F = 0.78539816339744830961566084582f;

const static float DP1 = 0.78515625f;
const static float DP2 = 2.4187564849853515625e-4f;
const static float DP3 = 3.77489497744594108e-8f;
const static float lossth = 8192.f;
const static float T24M1 = 16777215.f;

const static float NAN = (float)(((float)(1e+300 * 1e+300)) * 0.0F);

const static float P0 = 0.2499999995E+0f;
const static float P1 = 0.4160288626E-2f;
const static float Q0 = 0.5000000000E+0f;
const static float Q1 = 0.4998717877E-1f;

const static float C1 = 0.693359375f;
const static float C2 = -2.1219444005469058277e-4f;

const static float BIGX = 88.72283911f;
const static float EXPEPS = 1.0E-7f;
const static float K1 = 1.4426950409f;

const static float LOGFA0 = - 0.5527074855E+0f;
const static float LOGFB0 = -0.6632718214E+1f;

const static float LOGFC1 = 0.693359375f;
const static float LOGFC2 = -2.121944400546905827679E-4f;

const static float SQRTH = 0.70710678118654752440f;
const static float L102A = 3.0078125E-1f;
const static float L102B = 2.48745663981195213739E-4f;
const static float L10EA = 4.3359375E-1f;
const static float L10EB = 7.00731903251827651129E-4f;

const static float MAXL10 = 38.230809449325611792f;

const static float sincof[] = {
-1.9515295891E-4f,
 8.3321608736E-3f,
-1.6666654611E-1f
};

const static float coscof[] = {
 2.443315711809948E-005f,
-1.388731625493765E-003f,
 4.166664568298827E-002f
};

const static float P[] = {
	 7.0376836292E-2f,
	-1.1514610310E-1f,
	 1.1676998740E-1f,
	-1.2420140846E-1f,
	 1.4249322787E-1f,
	-1.6668057665E-1f,
	 2.0000714765E-1f,
	-2.4999993993E-1f,
	 3.3333331174E-1f
};

static unsigned short bmask[] = {
	0xffff,
	0xfffe,
	0xfffc,
	0xfff8,
	0xfff0,
	0xffe0,
	0xffc0,
	0xff80,
	0xff00,
	0xfe00,
	0xfc00,
	0xf800,
	0xf000,
	0xe000,
	0xc000,
	0x8000,
	0x0000,
};

const _Mathf Mathf = {
	.PI = 3.141593f,
	.Infinity = ((float)(1e+300 * 1e+300)),
	.NegativeInfinity = -((float)(1e+300 * 1e+300)),
	.Deg2Rad = 0.01745329f,
	.Rad2Deg = 57.29578f,
	.sin = sin_value,
	.cos = cos_value,
	.tan = tan_value,
	.asin = asin_value,
	.acos = acos_value,
	.atan = atan_value,
	.atan2 = atan2_value,
	.sqrt = sqrt_value,
	.abs = abs_value,
	.abs_i32 = abs_i32,
	.min = min,
	.min_i32 = min_i32,
	.max = max,
	.max_i32 = max_i32,
	.pow = pow_value,
	.exp = exp_value,
	.log = log_value,
	.log_e = log_e,
	.log_10 = log_10,
	.ceil = ceil_value,
	.floor = floor_value,
	.round = round,
	.ceil_to_int = ceil_to_int,
	.floor_to_int = floor_to_int,
	.round_to_int = round_to_int
};

#pragma warning(disable:4244)
static float sin_value(float f) {
	const float* p;
	float x, y, z;
	register unsigned long j;
	register int sign;

	sign = 1;
	x = f;
	if (f < 0) {
		sign = -1;
		x = -f;
	}
	if (x > T24M1) {
		return 0.0;
	}
	j = FOPI * x;
	y = j;
	if (j & 1) {
		j += 1;
		y += 1.0;
	}
	j &= 7;
	if (j > 3) {
		sign = -sign;
		j -= 4;
	}

	if (x > lossth) {
		x = x - y * PIO4F;
	}
	else {
		x = ((x - y * DP1) - y * DP2) - y * DP3;
	}

	z = x * x;
	if ((j == 1) || (j == 2)) {
		p = coscof;
		y = *p++;
		y = y * z + *p++;
		y = y * z + *p++;
		y *= z * z;
		y -= 0.5 * z;
		y += 1.0;
	}
	else {
		p = sincof;
		y = *p++;
		y = y * z + *p++;
		y = y * z + *p++;
		y *= z * x;
		y += x;
	}
	if (sign < 0)
		y = -y;
	return y;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float cos_value(float f) {
	float x, y, z;
	int j, sign;

	sign = 1;
	x = f;
	if (x < 0)
		x = -x;

	if (x > T24M1)
		return 0.0;

	j = FOPI * x;
	y = j;
	if (j & 1) {
		j += 1;
		y += 1.0;
	}
	j &= 7;
	if (j > 3) {
		j -= 4;
		sign = -sign;
	}

	if (j > 1)
		sign = -sign;

	if (x > lossth) {
		x = x - y * PIO4F;
	}
	else
		x = ((x - y * DP1) - y * DP2) - y * DP3;

	z = x * x;

	if ((j == 1) || (j == 2)) {
		y = (((-1.9515295891E-4 * z
			+ 8.3321608736E-3) * z
			- 1.6666654611E-1) * z * x)
			+ x;
	}
	else {
		y = ((2.443315711809948E-005 * z
			- 1.388731625493765E-003) * z
			+ 4.166664568298827E-002) * z * z;
		y -= 0.5 * z;
		y += 1.0;
	}
	if (sign < 0)
		y = -y;
	return y;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float tan_value(float f) {
	float x, y, z, zz;
	long j;
	int sign;

	if (f < 0.0) {
		x = -f;
		sign = -1;
	}
	else {
		x = f;
		sign = 1;
	}

	if (x > lossth) {
		return 0.0;
	}

	j = FOPI * x;
	y = j;

	if (j & 1) {
		j += 1;
		y += 1.0;
	}

	z = ((x - y * DP1) - y * DP2) - y * DP3;
	zz = z * z;

	if (x > 1.0e-4) {
		y =
			(((((9.38540185543E-3 * zz
				+ 3.11992232697E-3) * zz
				+ 2.44301354525E-2) * zz
				+ 5.34112807005E-2) * zz
				+ 1.33387994085E-1) * zz
				+ 3.33331568548E-1) * zz * z
			+ z;
	}
	else
		y = z;

	if (j & 2)
		y = -1.0 / y;

	if (sign < 0)
		y = -y;

	return y;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float asin_value(float f) {
	if (f < -1 || 1 < f)
		return NAN;

	float a, x, z;
	int sign, flag;

	x = f;

	if (x > 0) {
		sign = 1;
		a = x;
	}
	else {
		sign = -1;
		a = -x;
	}

	if (a > 1.0)
		return 0.0;

	if (a < 1.0e-4) {
		z = a;
		goto done;
	}

	if (a > 0.5) {
		z = 0.5 * (1.0 - a);
		x = sqrt_value(z);
		flag = 1;
	}
	else {
		x = a;
		z = x * x;
		flag = 0;
	}

	z =
		((((4.2163199048E-2 * z
			+ 2.4181311049E-2) * z
			+ 4.5470025998E-2) * z
			+ 7.4953002686E-2) * z
			+ 1.6666752422E-1) * z * x
		+ x;

	if (flag != 0) {
		z = z + z;
		z = PIO2F - z;
	}
done:
	if (sign < 0)
		z = -z;
	return z;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float acos_value(float f) {
	if (f < -1 || 1 < f)
		return NAN;
	if (f < -1.0)
		goto domerr;

	if (f < -0.5)
		return PIF - 2.0 * asin_value(sqrt_value(0.5 * (1.0 + f)));

	if (f > 1.0) {
	domerr:
		return 0.0;
	}

	if (f > 0.5)
		return 2.0 * asin_value(sqrt_value(0.5 * (1.0 - f)));

	return PIO2F - asin_value(f);
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float atan_value(float f) {
	float x, y, z;
	int sign;

	x = f;

	if (f < 0.0) {
		sign = -1;
		x = -f;
	}
	else {
		sign = 1;
		x = f;
	}

	if (x > 2.414213562373095) {
		y = PIO2F;
		x = -(1.0 / x);
	}

	else if (x > 0.4142135623730950) {
		y = PIO4F;
		x = (x - 1.0) / (x + 1.0);
	}
	else
		y = 0.0;

	z = x * x;
	y +=
		(((8.05374449538e-2 * z
			- 1.38776856032E-1) * z
			+ 1.99777106478E-1) * z
			- 3.33329491539E-1) * z * x
		+ x;

	if (sign < 0)
		y = -y;

	return y;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float atan2_value(float y, float x) {
	float z, w;
	int code;

	code = 0;

	if (x < 0.0)
		code = 2;
	if (y < 0.0)
		code |= 1;

	if (x == 0.0) {
		if (code & 1) {
			return -PIO2F;
			//return(3.0 * PIO2F);
		}
		if (y == 0.0)
			return 0.0;
		return PIO2F;
	}

	if (y == 0.0) {
		if (code & 2)
			return PIF;
		return 0.0;
	}

	switch (code) {
		default:
		case 0:
		case 1: w = 0.0; break;
		case 2: w = PIF; break;
		case 3: w = -PIF; break;
		/*case 0: w = 0.0; break;
		case 1: w = 2.0 * PIF; break;
		case 2:
		case 3: w = PIF; break;*/
	}

	z = atan_value(y / x);

	return w + z;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float frexpf(float x, int* pw2) {
	union {
		float y;
		unsigned short i[2];
	} u;
	int i, k;
	short* q;

	u.y = x;

	q = &u.i[1];

	i = (*q >> 7) & 0xff;
	if (i == 0) {
		if (u.y == 0.0) {
			*pw2 = 0;
			return 0.0;
		}
		do {
			u.y *= 2.0;
			i -= 1;
			k = (*q >> 7) & 0xff;
		} while (k == 0);
		i = i + k;
	}
	i -= 0x7e;
	*pw2 = i;
	*q &= 0x807f;
	*q |= 0x3f00;
	return u.y;
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float ldexpf(float x, int pw2) {
	union {
		float f;
		long l;
	} fl;
	long e;

	fl.f = x;

	e = (fl.l >> 23) & 0x000000ff;
	e += pw2;
	fl.l = ((e & 0xff) << 23) | (fl.l & 0x807fffff);

	return(fl.f);
}
#pragma warning(default:4244)

#pragma warning(disable:4244)
static float sqrt_value(float f) {
	if (f < 0)
		return NAN;

	float x, y;
	int e;
	if (f <= 0.0)
		return 0.0;

	x = frexpf(f, &e);
	if (e & 1) {
		x = x + x;
		e -= 1;
	}

	e >>= 1;

	if (x > 1.41421356237) {
		x = x - 2.0;
		y =
			(((((-9.8843065718E-4 * x
				+ 7.9479950957E-4) * x
				- 3.5890535377E-3) * x
				+ 1.1028809744E-2) * x
				- 4.4195203560E-2) * x
				+ 3.5355338194E-1) * x
			+ 1.41421356237E0;
		goto sqdon;
	}

	if (x > 0.707106781187) {
		x = x - 1.0;
		y =
			(((((1.35199291026E-2 * x
				- 2.26657767832E-2) * x
				+ 2.78720776889E-2) * x
				- 3.89582788321E-2) * x
				+ 6.24811144548E-2) * x
				- 1.25001503933E-1) * x * x
			+ 0.5 * x
			+ 1.0;
		goto sqdon;
	}

	x = x - 0.5;
	y =
		(((((-3.9495006054E-1 * x
			+ 5.1743034569E-1) * x
			- 4.3214437330E-1) * x
			+ 3.5310730460E-1) * x
			- 3.5354581892E-1) * x
			+ 7.0710676017E-1) * x
		+ 7.07106781187E-1;

sqdon:
	y = ldexpf(y, e);
	return y;
}
#pragma warning(default:4244)

static float abs_value(float f) {
	union {
		float f;
		int i;
	} u;
	u.f = f;
	u.i &= 0x7FFFFFFFULL;
	return u.f;
}

static int abs_i32(int f) {
	int s = f >> 31;
	return (f ^ s) - s;
}

static float min(float a, float b) {
    return a < b ? a : b;
}

static int min_i32(int a, int b) {
	return a < b ? a : b;
}

static float max(float a, float b) {
	return a > b ? a : b;
}

static int max_i32(int a, int b) {
    return a > b ? a : b;
}

static float pow_value(float f, float p) {
	if (p == 0.0) return 1.0;
	if (p == 1.0) return f;
	if (f <= 0.0) return 0.0;
	return exp_value(log_e(f) * p);
}

#pragma warning(disable:4244)
#pragma warning(disable:26451)
static float exp_value(float power) {
	int n;
	float xn, g, r, z, y;
	bool sign;

	if (power >= 0.0) {
		y = power;  sign = 0;
	}
	else {
		y = -power; sign = 1;
	}

	if (y < EXPEPS) return 1.0;

	if (y > BIGX) {
		if (sign) {
			errno = ERANGE;
			return Mathf.Infinity;
		}
		else {
			return 0.0;
		}
	}

	z = y * K1;
	n = z;

	if (n < 0) --n;
	if (z - n >= 0.5) ++n;
	xn = n;
	g = ((y - xn * C1)) - xn * C2;
	z = g * g;
	r = ((P1 * z) + P0) * g;
	r = 0.5 + (r / ((Q1 * z) + Q0 - r));

	n++;
	z = ldexpf(r, n);
	if (sign)
		return 1.0 / z;
	else
		return z;
}
#pragma warning(default:26451)
#pragma warning(default:4244)

static float log_value(float f, float p) {
	if (f < 0 || f == NAN)
		return NAN;
	if (f == 0)
		return Mathf.NegativeInfinity;
	if (f == Mathf.Infinity)
		return Mathf.Infinity;

	return log_10(f) / log_10(p);
}

#pragma warning(disable:4244)
static float log_e(float f) {
	if (f < 0 || f == NAN)
		return NAN;
	if (f == 0)
		return Mathf.NegativeInfinity;
	if (f == Mathf.Infinity)
		return Mathf.Infinity;

	float rz;
	float g, z, w, znum, zden, xn;
	int n;

	if (f <= 0.0) {
		errno = EDOM;
		return 0.0;
	}
	g = frexpf(f, &n);
	znum = g - 0.5;
	if (g > SQRTH) {
		znum -= 0.5;
		zden = (g * 0.5) + 0.5;
	}
	else {
		n--;
		zden = znum * 0.5 + 0.5;
	}
	z = znum / zden;
	w = z * z;

	rz = z + z * (w * LOGFA0 / (w + LOGFB0));
	xn = n;
	return ((xn * LOGFC2 + rz) + xn * LOGFC1);
}
#pragma warning(default:4244)

static float polevlf(float xx, float* coef, int N) {
	float ans, x;
	float* p;
	int i;

	x = xx;
	p = coef;
	ans = *p++;

	i = N;
	do ans = ans * x + *p++;
	while (--i);

	return(ans);
}

#pragma warning(disable:4244)
static float log_10(float f) {
	if (f < 0 || f == NAN)
		return NAN;
	if (f == 0)
		return Mathf.NegativeInfinity;
	if (f == Mathf.Infinity)
		return Mathf.Infinity;

	float x, y, z;
	int e;

	x = f;

	if (x <= 0.0)
		return -MAXL10;

	x = frexpf(x, &e);

	if (x < SQRTH)
	{
		e -= 1;
		x = 2.0 * x - 1.0;
	}
	else
	{
		x = x - 1.0;
	}

	z = x * x;
	y = x * (z * polevlf(x, (float*)P, 8));
	y = y - 0.5 * z;

	z = (x + y) * L10EB;
	z += y * L10EA;
	z += x * L10EA;
	x = e;
	z += x * L102B;
	z += x * L102A;

	return z;
}
#pragma warning(default:4244)

static float ceil_value(float f) {
	float y;

	y = floor_value((float)f);
	if (y < f)
		y += 1.0;
	return y;
}

static float floor_value(float f) {
	unsigned short* p;
	union {
		float y;
		unsigned short i[2];
	} u;
	int e;

	u.y = f;
	p = &u.i[1];
	e = ((*p >> 7) & 0xff) - 0x7f;
	p -= 1;

	if (e < 0) {
		if (u.y < 0)
			return(-1.0);
		else
			return(0.0);
	}

	e = (24 - 1) - e;

	while (e >= 16) {
		*p++ = 0;
		e -= 16;
	}

	if (e > 0)
		*p &= bmask[e];

	if ((f < 0) && (u.y != f))
		u.y -= 1.0;

	return u.y;
}

static float round(float f) {
	return floor_value(f) + 0.5f;
}

static int ceil_to_int(float f) {
	int inum = (int)f;
	if (f < 0 || f == (float)inum) {
		return inum;
	}
	return inum + 1;
}

static int floor_to_int(float f) {
	if (f >= 0)
		return (int)f;
	else {
		int y = (int)f;
		return ((float)y == f) ? y : y - 1;
	}
}

static int round_to_int(float f) {
	return (int)(f < 0 ? (f - 0.5) : (f + 0.5));
}