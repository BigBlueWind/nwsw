/**
 * @file    basic_types.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief
 *
 * @author : Manu Sharma
 * Date   :
 * Description :
 *
 */

#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/** Macros */
#define    def(type,value)          (type)value

#ifndef NULL
#define NULL           ((void*)0)
#endif

#define TRUE               1
#define FALSE              0

/*
 * instead of x == 3, use x EQ 3.
 * This will avoid if (x = 3) kind of errors.
 */
#define EQ                 ==
#define NOTEQ              !=

#define LEN_IPV4_ADDR         16
#define LEN_IEEE_ADDR         24
#define LEN_IPV6_ADDR         64

typedef signed char      SINT8;
typedef signed short     SINT16;
typedef signed int       SINT32;
typedef signed long long SINT64;

typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef UINT8              TBOOL;
typedef float              REAL;
typedef unsigned char      IPV4ADDRESS;
typedef unsigned char      MACADDRESS;
typedef unsigned char      FILENAME;
typedef unsigned char      LOCALTIME;
typedef unsigned char      OFFSETTIME;
typedef unsigned char      SWVERSION;
typedef unsigned char      MCC;
typedef unsigned char      MNC;
typedef unsigned char      LATITUDE;
typedef unsigned char      LONGITUDE;
typedef unsigned char      ENODEBNAME;
typedef unsigned char      PROFILENAME;

/*typedef long long int    UINT64; 20Oct2010, Manu: removed as 64 bit numbers are not to be used.*/

typedef SINT32 Index;
typedef SINT16 Count;
typedef UINT8  String5[5];
typedef UINT8  String10[10];
typedef UINT8  String16[16];
typedef UINT8  String32[32];
typedef UINT8  String50[50];
typedef UINT8  String100[100];
typedef UINT8  String150[150];
typedef UINT8  String256[256];
typedef UINT8  String512[512];
typedef UINT8  String1kb[1024];
typedef UINT8  String2kb[2048];
typedef UINT8  String4kb[4096];
typedef UINT8  String5kb[5110];
typedef UINT8  String6kb[6144];
typedef UINT8  String30kb[30720];
typedef UINT32 TraceLen;

//typedef UINT8  FileName[128];
//typedef FileName FileName128;

#define MAX_LEN_FILE_NAME    100

typedef UINT8    Percent; // 0..100
typedef UINT32   DataAndTime;

typedef UINT16     PortNum;
typedef UINT32     IntervalMin;
typedef UINT32     IntervalSec;
typedef UINT32     BytesPerSec;
typedef UINT32     IntervalMsec;
typedef String50   ServerName;
typedef String50   LoginId;
typedef String50   Password;
typedef String256  FilePath;
typedef String256  FileName; /* TO DO : This size may require for development machine and in future we can change the size */
typedef String16   IpAddrStr; /* IP Address stated as 123.3.41.23 */

typedef struct s_SINT8_O
{
  TBOOL   used;
  SINT8   value;
} __attribute__((__packed__))SINT8_O;

typedef struct s_SINT16_O
{
  TBOOL   used;
  SINT16  value;
} __attribute__((__packed__))SINT16_O;

typedef struct s_SINT32_O
{
  TBOOL   used;
  SINT32  value;
} __attribute__((__packed__))SINT32_O;

typedef struct s_SINT64_O
{
  TBOOL   used;
  SINT32  value;
} __attribute__((__packed__))SINT64_O;

typedef struct s_UINT0_O
{
  TBOOL used;
} __attribute__((__packed__))UINT0_O;

typedef struct s_UINT8_O
{
  TBOOL used;
  UINT8 value;
} __attribute__((__packed__))UINT8_O;

typedef struct s_UINT16_O
{
  TBOOL  used;
  UINT16 value;
} __attribute__((__packed__))UINT16_O;

typedef struct s_UINT32_O
{
  TBOOL  used;
  UINT32 value;
} __attribute__((__packed__))UINT32_O;

typedef struct s_ARRAY8_O
{
  TBOOL  used;
  UINT16 len;
  UINT8  value[8];
} __attribute__((__packed__))ARRAY8_O;

typedef struct s_ARRAY16_O
{
  TBOOL  used;
  UINT16 len;
  UINT8  value[16];
} __attribute__((__packed__))ARRAY16_O;

typedef struct s_ARRAY32_O
{
  TBOOL  used;
  UINT16 len;
  UINT8  value[32];
} __attribute__((__packed__))ARRAY32_O;

typedef struct s_ARRAY256_O
{
  TBOOL  used;
  UINT16 len;
  UINT8  value[256];
} __attribute__((__packed__))ARRAY256_O;

typedef UINT16 Length;

typedef struct s_OctetBuf255
{
  Length len;
  UINT8  value[255];
} __attribute__((__packed__))OctetBuf255;

typedef struct s_OctetBuf1024
{
  Length len;
  UINT8  value[1024];
} __attribute__((__packed__))OctetBuf1024;

typedef struct s_OctetBuf2048
{
  Length len;
  UINT8  value[2048];
} __attribute__((__packed__))OctetBuf2048;

#define skipBytes(bufP, n)  (bufP)->index +=n



/* IP / IEEE Address ************************************/
typedef union u_NodeAddr
{
  UINT8 v4Addr[LEN_IPV4_ADDR];
  UINT8 ieeeAddr[LEN_IEEE_ADDR];
  UINT8 v6Addr[LEN_IPV6_ADDR];
} __attribute__((__packed__))NodeAddr;

typedef struct s_NodeAddr_O
{
  TBOOL        used;
  UINT16       addrType;
  NodeAddr     nodeAddr;
} __attribute__((__packed__))NodeAddr_O;

typedef enum e_IpAddrChoice
{
  IPADDRCHOICE_IPV4 = 1,
  IPADDRCHOICE_IPV6,
  IPADDRCHOICE_BOTH
}IpAddrChoice;

/* Types of node addresses */
#define ADDR_IPV4             1
#define ADDR_IEEE             2
#define ADDR_IPV6             3

#ifdef UNUSED
#undef UNUSED
#define UNUSED(x) (x=x)
#else
#define UNUSED(x) (x=x)
#endif


/** Macros */
#define copyBytes( src, len, dest )  memcpy((char*)dest,(const char*)src,(size_t)(len))
#define clearBytes( src, len)    memset(src, 0, (size_t)len)

/* Qualifiers for function parameters ************************************/

/** Qualifiers for function parameters.
 * Keyword | Meaning
 * =============================================
 * IN      | 1. the parameter is input to the function and
 *         | 2. parameter is not changed inside function
 *
 * OUT     | 1. Parameter returned by function
 *         | 2. Memory allocated inside function.
 *
 * IO      | 1. Parameter returned (i.e. filled inside function)
 *         | 2. Function assumes that memory has already been allocated.
 *
 * FR      | 1. Function can use this parameter and
 *         | 2. It should free the memory OR
 *         |    pass it to another function along with FR flag.
 *
 * Examples:
 *
 * 1. void get_date(IO Date* fill_it)
 *    It means fill_it parameter is allocated outside get_date.
 *    And it is filled by get_date.
 * 2. void create_buff(OUT void **buf, IN int len)
 *    It means crate_buff will allocate memory of length len and return it in *buf.
 */

#define IN const
#define OUT
#define IO
#define FR
#define CONS /* same as FR, needs to be removed */

#endif

