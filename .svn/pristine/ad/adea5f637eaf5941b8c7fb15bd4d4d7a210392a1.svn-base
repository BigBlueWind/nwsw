/************************************************************************
*                                                                       *
*       Copyright 2012 by Conexant Systems.                             *
*       All Rights Reserved                                             *
*                                                                       *
*       Conexant Systems                                                *
*       4311 Jamboree Road                                              *
*       Newport Beach, CA 92658                                         *
*                                                                       *
*                                                                       *
*       File:  appprintf.h                                              *
*                                                                       *
*       This file defines printf and sprintf functions                  *
*                                                                       *
************************************************************************/

#ifndef __PRINTF_H_
#define __PRINTF_H_

#include <stdio.h>

#define FILE void

#ifndef IGNORE
#define IGNORE(x) ((x)=(x))
#endif

#define release_printf(x)

typedef struct
{
    char *ptr;
} StringOutputFile;

typedef struct
{
    int flags;
    char *prefix;
    int precision;
    int before_dot;
    int after_dot;
    double d;
} fp_print_rec;

typedef int fp_print(int ch, char buff[], fp_print_rec * p);

typedef struct
{
    fp_print_rec fpr;
    fp_print *fn;
    int (*putc1)(int, FILE *);
    int (*ferror)(FILE *);
    char *hextab;
    int width;
} printf_display_rec;

#define _LJUSTIFY         0x1
#define _SIGNED           0x2
#define _BLANKER          0x4
#define _VARIANT          0x8
#define _PRECGIVEN       0x10
#define _LONGSPEC        0x20
#define _LLSPEC          0x40
#define _SHORTSPEC       0x80
#define _PADZERO        0x100 /* *** DEPRECATED FEATURE *** */
#define _FPCONV         0x200


int _sputc(int ch, FILE * fp);
int _serror(FILE * f);
int _no_fp_display(int ch, char buff[], fp_print_rec * p);
int printf_display(FILE * p, int ch, printf_display_rec * dr, unsigned int v);
int __vfprintf(FILE * p, const char *fmt, va_list args,
               printf_display_rec * dr);


#endif /* printf.h */
