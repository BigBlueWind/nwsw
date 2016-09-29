#define PI 3.1416
#define CONSTANT_VALUE 8388608	// (2^23)
#define ROWS 0
#define COLUMNS 1
//#define DEBUG_PRINT

#include "typedef.h"
#if 0
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned __int64 U64;
typedef signed __int64 S64;
#endif

typedef unsigned long U24;
typedef signed long S24;

#ifndef _UINT64_
#define _UINT64_
#if defined (__CC_ARM)
typedef unsigned __int64 U64, *PU64;
typedef signed __int64 S64, *PS64;
#elif defined (__GNUC__)
typedef long long unsigned int U64, *PU64;
typedef long long signed int S64, *PS64;
#elif defined _WIN32
typedef unsigned __int64 U64, *PU64;
typedef signed __int64 S64, *PS64;
#endif
#endif /*_UINT64_*/

struct COMPLEX16 {S16 real; S16 imag;};
struct COMPLEX24 {S24 real; S24 imag;};
struct COMPLEX64 {S64 real; S64 imag;};


void BreakDownRow(struct COMPLEX24 *input, struct COMPLEX24 *output, S16 stage, S16 flag, S16 product, S16 *product_array, S16 num_factors);
void MultCosntRow(struct COMPLEX24 *input, struct COMPLEX24 *output, S16 stage, S16 product, S16 *product_array, S16 num_factors, S16 level);

void Idft2Pt(struct COMPLEX24 *input, struct COMPLEX24 *output);
void Idft3Pt(struct COMPLEX24 *input, struct COMPLEX24 *output);
void Idft4Pt(struct COMPLEX24 *input, struct COMPLEX24 *output);
void Idft5Pt(struct COMPLEX24 *input, struct COMPLEX24 *output);

void ConvertComplex64to24(struct COMPLEX64 *inter, struct COMPLEX24 *output, S16 count);
struct COMPLEX64 SubtractComplex24(struct COMPLEX24 a, struct COMPLEX24 b);
struct COMPLEX64 SubtractComplex64(struct COMPLEX64 a, struct COMPLEX64 b);
struct COMPLEX64 AddComplex24(struct COMPLEX24 a, struct COMPLEX24 b);
struct COMPLEX64 AddComplex64(struct COMPLEX64 a, struct COMPLEX64 b);
struct COMPLEX64 MultiplyComplex24(struct COMPLEX24 input, struct COMPLEX16 twiddle);

void FindMax(struct COMPLEX24 *input, S16 length, S16 *width);

S16 Round(double input);





#define IDFT_1296
#define IDFT_1200
#define IDFT_1152
#define IDFT_1080
#define IDFT_972
#define IDFT_960
#define IDFT_900
#define IDFT_864
#define IDFT_768
#define IDFT_720
#define IDFT_648
#define IDFT_600
#define IDFT_576
#define IDFT_540
#define IDFT_480
#define IDFT_432
#define IDFT_384
#define IDFT_360
#define IDFT_324
#define IDFT_300
#define IDFT_288
#define IDFT_240
#define IDFT_216
#define IDFT_192
#define IDFT_180
#define IDFT_144
#define IDFT_120
#define IDFT_108
#define IDFT_96
#define IDFT_72
#define IDFT_60
#define IDFT_48
#define IDFT_36
#define IDFT_24
#define IDFT_12

