#ifndef _CELLSEARCHCOM_H
#define _CELLSEARCHCOM_H



#define 	SAMPLE_RATE 					2
#define 	SLOT_NUM_PER_FREAM 			15
#define 	CHIP_NUM_PER_SLOT 			2560

#define 	CPICH_NONCONHERENT_LEN		3
#define  	PM_NONCONHERENT_LEN      	3
#define 	PSC_NONCONHERENT_LEN		6
#define   RSCP_ACC_LEN               		3

#define 	PM_CONHERENT_LEN  		512
#define   CPICH_COHERENT_LEN 			512
#define   RSCP_COHERENT_LEN			512

#define  PM_MAX_FINGER_NUM			4

#define	DL_CODEGRP_NUM				64
#define	PSCR_NUM_PER_CODEGRP			8
#define   PM_SEARCH_WIN_LEN			64

#define   MAX_PEAK_NUM				256
#define   MAX_CELLLSRCH_NUM			16
#define   AGC_DMA_DONE                              0xfefe
#define   AGC_SOFTWARE_DONE                    0xabab

typedef struct 
{
    U16 		PkNum;
    U8 		Flag[MAX_PEAK_NUM];
    U16		Position[MAX_PEAK_NUM] ;
    U32		Power[MAX_PEAK_NUM]; 
} PschPeakPos_t;


typedef struct _PSynPeakInfo
{

    U8		GrpNum[MAX_PEAK_NUM];
    U8		SlotNum[MAX_PEAK_NUM];
    U16           PIndex[MAX_PEAK_NUM];
    U32          CpiAcc[MAX_PEAK_NUM][10][PSCR_NUM_PER_CODEGRP];
    U32          FHTRst[MAX_PEAK_NUM][15][16];
	U32			 SschPow[MAX_PEAK_NUM];
	U32			 SschMean[MAX_PEAK_NUM];
} PSynPeakInfo;

typedef struct 
{
   PSynPeakInfo  PsynPk;

} PschPeak_t;

typedef struct _CellInfo
{
   U16  CellPInx;
   U32  RSCP;

} CellInfo;

typedef struct _CellRptInfo
{
   U8 	CellNum;
   CellInfo  CellInfo[MAX_CELLLSRCH_NUM];

} CellRptInfo;

typedef struct _PathInfo
{
   U8    AntId;
   U32  PathOffset;
   U32  PathPower;
} PathInfo;

typedef struct _CellPathInfo
{
   U16   PIndx;
   U8    SlotNum;
   U8    FingNum;
   U16  WinBegin;
   U32  MinPos;
   U32  PscrOffset;
   U32  NoiseHis;
   U32  RscpAcc;
   U32  RssiAcc;
   U8   RscpAccCnt;
   U32	RscpAccFrame;
   U32	RssiAccFrame;
   U32  maxPos;  
   U32  maxPow;   
   S16  freqConjSym[150][2];   
   S16  angle;
   PathInfo Path[PM_MAX_FINGER_NUM];
} CellPathInfo;

typedef struct 
{
   U8 	CellNum;   
   CellPathInfo  CellPathInfo[MAX_CELLLSRCH_NUM];
} CellGrpDetcInfo_t;


#endif

