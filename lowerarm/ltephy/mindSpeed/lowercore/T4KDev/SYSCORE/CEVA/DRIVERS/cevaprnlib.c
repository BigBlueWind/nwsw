/************************************************************************
*                                                                       *
*       Copyright 2000 by Conexant Systems.                             *
*       All Rights Reserved                                             *
*                                                                       *
*       Conexant Systems                                                *
*       4311 Jamboree Road                                              *
*       Newport Beach, CA 92658                                         *
*                                                                       *
*                                                                       *
*       File:  appprintf.c                                              *
*                                                                       *
*       This file implements printf and sprintf functions               *
*                                                                       *
************************************************************************/
#include "cevaprnlib.h"

#define intofdigit(x) ((x)-'0')

//#ifdef 1
#define xputc(dr, ch, f) (dr->putc1(ch, f))
//#else
//#define xputc(dr, ch, f)
//#endif

#define pr_padding(dr, ch, n, f)  while (--n>=0) charcount++, xputc(dr, ch, f);

#define pre_padding(dr, f)                 \
   if (!(flags&_LJUSTIFY))                                           \
   {                      \
    char padchar = flags & _PADZERO ? '0' : ' ';       \
    pr_padding(dr, padchar, width, f);          \
   }                      \
 
#define post_padding(dr, f)                \
   if (flags&_LJUSTIFY)                \
   {                      \
    pr_padding(dr, ' ', width, f);           \
   }                      \
 

__inline int is_digit(int c)
{
    return (unsigned int)(c - '0') <= 9;
}

int _sputc(int ch, FILE * fp)
{
    *((StringOutputFile *) fp)->ptr++ = ch;
    /* char *op = sf->ptr;
     int r = *op++ = ch;
     sf->ptr = op;
     return r; */
    return ch;
}

int _serror(FILE * f)
{
    IGNORE(f);
    return 0;
}

int _no_fp_display(int ch, char buff[], fp_print_rec * p)
{
    ch = ch;
    buff = buff;
    p = p;
    return 0;
}

int printf_display(FILE * p, int ch, printf_display_rec * dr, unsigned int v)
{
    int len = 0, charcount = 0;
    char buff[32];   /* used to accumulate value to print    */
    int flags = dr->fpr.flags;

    dr->fpr.before_dot = -1, dr->fpr.after_dot = -1;
    if (!(dr->fpr.flags & (_FPCONV + _PRECGIVEN)))
        dr->fpr.precision = 1;

    switch (ch)
    {
        case 'p':
        case 'X':
        case 'x':
            while (v != 0)
            {
                buff[len++] = dr->hextab[v & 0xf];
                v = v >> 4;
            }
            break;
        case 'o':
            while (v != 0)
            {
                buff[len++] = '0' + (v & 7);
                v = v >> 3;
            }
            /* Fix for X3J11 Interpretation #21 [printf("%#.4o,345) => "0531"]       */
            /* The precision is increased only if necessary to obtain 0 as 1st digit */
            if (flags & _VARIANT)
                dr->fpr.precision--;
            break;
        case 'u':
        case 'i':
        case 'd':
            while (v != 0)
            {
                buff[len++] = '0' + (char)(v % 10);
                v = v / 10;
            }
            break;

            /* If floating point is not supported I display ALL %e, %f and %g        */
            /* items as 0.0                                                          */
        default:
            buff[0] = '0';
            buff[1] = '.';   /* DecimalPoint; */
            buff[2] = '0';
            len = 3;
            break;
    }
    /* now work out how many leading '0's are needed for precision specifier */
    /* _FPCONV is the case of FP printing in which case extra digits to make */
    /* up the precision come within the number as marked by characters '<'   */
    /* and '>' in the buffer.                                                */
    {
        int precision;
        int width = dr->width;
        if (flags & _FPCONV)
        {
            precision = 0;
            if (dr->fpr.before_dot > 0)
                precision = dr->fpr.before_dot - 1;
            if (dr->fpr.after_dot > 0)
                precision += dr->fpr.after_dot - 1;
        }
        else
        {
            precision = dr->fpr.precision;
            if ((precision -= len) < 0)
                precision = 0;
        }
        /* and how much padding is needed */
        width -= (precision + len + strlen((const char *)dr->fpr.prefix));
#ifdef 0
        /* AM: ANSI appear (Oct 86) to suggest that the padding (even if with '0') */
        /*     occurs before the possible sign!  Treat this as fatuous for now.    */
        if (!(flags & _PADZERO))
            pre_padding(dr, p);
#endif
        {
            int c;     /* prefix    */
            char *prefix = dr->fpr.prefix;
            for (; (c = *prefix++) != 0; charcount++)
                xputc(dr, c, p);
                //dr->putc(c, p,k);
        }

        pre_padding(dr, p);

        /* floating point numbers are in buff[] the normal way around, while     */
        /* integers have been pushed in with the digits in reverse order.        */
        if (flags & _FPCONV)
        {
            int i, c;
            for (i = 0; i < len; i++)
            {
                switch (c = buff[i])
                {
                    case '<':
                        pr_padding(dr, '0', dr->fpr.before_dot, p);
                        break;
                    case '>':
                        pr_padding(dr, '0', dr->fpr.after_dot, p);
                        break;
                    default:
                        xputc(dr, c, p);
                        charcount++;
                        break;
                }
            }
        }
        else
        {
            pr_padding(dr, '0', precision, p);
            charcount += len;
            while (len-- > 0)
                xputc(dr, buff[len], p);
        }
        /* By here if the padding has already been printed width will be zero    */
        post_padding(dr, p);
        return charcount;
    }
}

int __vfprintf(FILE * p, const char *fmt, va_list args, printf_display_rec * dr)
/* ACN: I apologize for this function - it seems long and ugly. Some of  */
/*      that is dealing with all the jolly flag options available with   */
/*      printf, and rather a lot more is a cautious floating point print */
/*      package that takes great care to avoid the corruption of its     */
/*      input by rounding, and to generate consistent decimal versions   */
/*      of all possible values in all possible formats.                  */
{
    int ch, charcount = 0;
    while ((ch = *fmt++) != 0)
    {
        if (ch != '%')
        {
            xputc(dr, ch, p);
            charcount++;
            if (ch == '\n')
            {
                xputc(dr, '\r', p);
                charcount++;
            }

        }
        else
        {
            int flags = 0, width = 0;
            unsigned int v = 0;
            dr->fpr.precision = 0;
            /* This decodes all the nasty flags and options associated with an       */
            /* entry in the format list. For some entries many of these options      */
            /* will be useless, but I parse them all the same.                       */
            for (;;)
            {
                switch (ch = *fmt++)
                {
                        /* '-'  Left justify converted flag. Only relevant if width specified    */
                        /* explicitly and converted value is too short to fill it.               */
                    case '-':
                        flags = _LJUSTIFY | (flags & ~_PADZERO);
                        continue;

                        /* '+'  Always print either '+' or '-' at start of numbers.              */
                    case '+':
                        flags |= _SIGNED;
                        continue;

                        /* ' '  Print either ' ' or '-' at start of numbers.                     */
                    case ' ':
                        flags |= _BLANKER;
                        continue;

                        /* '#'  Variant on main print routine (effect varies across different    */
                        /*      styles, but for instance %#x puts 0x on the front of displayed   */
                        /*      numbers.                                                         */
                    case '#':
                        flags |= _VARIANT;
                        continue;

                        /* '0'  Leading blanks are printed as zeros                              */
                        /*        This is a *** DEPRECATED FEATURE *** (precision subsumes)      */
                    case '0':
                        flags |= _PADZERO;
                        continue;

                    default:
                        break;
                }
                break;
            }
            /* now look for 'width' spec */
            {
                int t = 0;
                if (ch == '*')
                {
                    t = va_arg(args, int);
                    /* If a negative width is passed as an argument I take its absolute      */
                    /* value and use the negativeness to indicate the presence of the '-'    */
                    /* flag (left justification). If '-' was already specified I lose it.    */
                    if (t < 0)
                    {
                        t = -t;
                        flags ^= _LJUSTIFY;
                    }
                    ch = *fmt++;
                }
                else
                {
                    while (is_digit(ch))
                    {
                        t = t * 10 + intofdigit(ch);
                        ch = *fmt++;
                    }
                }
                width = t >= 0 ? t : 0; /* disallow -ve arg */
            }
            if (ch == '.')  /* precision spec */
            {
                int t = 0;
                ch = *fmt++;
                if (ch == '*')
                {
                    t = va_arg(args, int);
                    ch = *fmt++;
                }
                else
                    while (is_digit(ch))
                    {
                        t = t * 10 + intofdigit(ch);
                        ch = *fmt++;
                    }
                if (t >= 0)
                    flags |= _PRECGIVEN, dr->fpr.precision = t;
            }
            if (ch == 'l' || ch == 'L')
                /* 'l'  Indicate that a numeric argument is 'long'. Here int and long    */
                /*      are the same (32 bits) and so I can ignore this flag!            */
                /* 'L'  Marks floating arguments as being of type long double. Here this */
                /*      is the same as just double, and so I can ignore the flag.        */
            {
                flags |= _LONGSPEC;
                ch = *fmt++;
            }
            else if (ch == 'h')
                /* 'h' Indicates that an integer value is to be treated as short.        */
            {
                flags |= _SHORTSPEC;
                ch = *fmt++;
            }

            /* Now the options have been decoded - I can process the main dispatch   */
            switch (ch)
            {

                    /* %c causes a single character to be fetched from the argument list     */
                    /* and printed. This is subject to padding.                              */
                case 'c':
                    ch = va_arg(args, int);
                    /* drop through */

                    /* %? where ? is some character not properly defined as a command char   */
                    /* for printf causes ? to be displayed with padding and field widths     */
                    /* as specified by the various modifers. %% is handled by this general   */
                    /* mechanism.                                                            */
                default:
                    width--;  /* char width is 1       */
                    pre_padding(dr, p);
                    xputc(dr, ch, p);
                    charcount++;
                    post_padding(dr, p);
                    continue;

                    /* If a '%' occurs at the end of a format string (possibly with a few    */
                    /* width specifiers and qualifiers after it) I end up here with a '\0'   */
                    /* in my hand. Unless I do something special the fact that the format    */
                    /* string terminated gets lost...                                        */
                case 0:
                    fmt--;
                    continue;

                    /* %n assigns the number of chars printed so far to the next arg (which  */
                    /* is expected to be of type (int *).                                    */
                case 'n':
                    if (flags & _SHORTSPEC)
                        *va_arg(args, short *) = (short)charcount;
                    else if (flags & _LONGSPEC)
                        *va_arg(args, long *) = charcount;
                    else
                        *va_arg(args, int *) = charcount;
                    continue;

                    /* %s prints a string. If a precision is given it can limit the number   */
                    /* of characters taken from the string, and padding and justification    */
                    /* behave as usual.                                                      */
                case 's':
                    {
                        char *str = va_arg(args, char *);
                        int i, n;
                        if (flags & _PRECGIVEN)
                        {
                            int precision = dr->fpr.precision;
                            for (n = 0; n < precision && str[n] != 0; n++)
                                continue;
                        }
                        else
                            n = strlen((const char *)str);
                        width -= n;
                        pre_padding(dr, p);
                        for (i = 0; i < n; i++)
                            xputc(dr, str[i], p);
                        charcount += n;
                        post_padding(dr, p);
                    }
                    continue;

                    /* %x prints in hexadecimal. %X does the same, but uses upper case       */
                    /* when printing things that are not (decimal) digits.                   */
                    /* I can share some messy decoding here with the code that deals with    */
                    /* octal and decimal output via %o and %d.                               */
                case 'X':
                    v = va_arg(args, int);
                    if (flags & _SHORTSPEC)
                        v = (unsigned short)v;
                    dr->hextab = "0123456789ABCDEF";
                    dr->fpr.prefix = (char *)((flags & _VARIANT) ? "0X" : "");
                    if (flags & _PRECGIVEN)
                        flags &= ~_PADZERO;
                    break;

                case 'x':
                    v = va_arg(args, int);
                    if (flags & _SHORTSPEC)
                        v = (unsigned short)v;
                    dr->hextab = "0123456789abcdef";
                    dr->fpr.prefix = (char *)((flags & _VARIANT) ? "0x" : "");
                    if (flags & _PRECGIVEN)
                        flags &= ~_PADZERO;
                    break;

                    /* %p is for printing a pointer - I print it as a hex number with the    */
                    /* precision always forced to 8.                                         */
                case 'p':
                    v = (unsigned int)va_arg(args, void *);
                    dr->hextab = "0123456789abcdef";
                    dr->fpr.prefix = (char *)((flags & _VARIANT) ? "@" : "");
                    dr->fpr.precision = 8;
                    flags |= _PRECGIVEN;
                    break;

                case 'o':
                    v = va_arg(args, int);
                    if (flags & _SHORTSPEC)
                        v = (unsigned short)v;
                    dr->fpr.prefix = (char *)((flags & _VARIANT) ? "0" : "");
                    if (flags & _PRECGIVEN)
                        flags &= ~_PADZERO;
                    break;

                case 'u':
                    v = va_arg(args, unsigned int);
                    if (flags & _SHORTSPEC)
                        v = (unsigned short)v;
                    dr->fpr.prefix = "";
                    if (flags & _PRECGIVEN)
                        flags &= ~_PADZERO;
                    break;

                case 'i':
                case 'd':
                    {
                        int w;
                        w = va_arg(args, int);
                        if (flags & _SHORTSPEC)
                            w = (signed short)w;
                        v = (w < 0) ? -w : w;
                        dr->fpr.prefix = (w < 0) ? (char *)"-" :
                                         (flags & _SIGNED) ? (char *)"+" :
                                         (flags & _BLANKER) ? (char *)" " : (char *)"";
                    }
                    if (flags & _PRECGIVEN)
                        flags &= ~_PADZERO;
                    break;

                case 'f':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                    flags |= _FPCONV;
                    if (!(flags & _PRECGIVEN))
                        dr->fpr.precision = 6;

                    {
                        int w = va_arg(args, int);
                        w = va_arg(args, int);
                        /* If the pre-processor symbol FLOATING_POINT is not set I assume that   */
                        /* floating point is not available, and so support %e, %f and %g with    */
                        /* a fragment of code that skips over the relevant argument.             */
                        /* I also assume that a double takes two int-sized arg positions.        */
                        dr->fpr.prefix = (flags & _SIGNED) ? (char *)"+" :
                                         (flags & _BLANKER) ? (char *)" " : (char *)"";
                    }
                    break;

            }
            dr->width = width;
            dr->fpr.flags = flags;
            charcount += printf_display(p, ch, dr, v);
            continue;
        }
    }
    return dr->ferror(p) ? (-1) : charcount;
}
/* End of printf.c */
