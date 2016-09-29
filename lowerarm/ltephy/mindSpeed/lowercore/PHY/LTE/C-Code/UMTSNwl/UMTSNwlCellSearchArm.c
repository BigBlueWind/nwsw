//-------------------------------------------------------------------------------------------
/** @file UMTSNwlCellSearchArm.c
 *
 * @brief arm side function which control data flow
 * @author Mindspeed Technologies
 * @version $Revision: 1.2.4.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <math.h>
#include <string.h>
#include <stdio.h>

//#include "typedef.h"
//#include "supervisor.h"
#include "UMTSNwlCommMacro.h"
#include "UMTSNwlCellSearchArm.h"




S8 HadamaSeq16[16][16]=
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1},
	{1,1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,1,-1,-1},
	{1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,1,-1,-1,1},
	{1,1,1,1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1},
	{1,-1,1,-1,-1,1,-1,1,1,-1,1,-1,-1,1,-1,1},
	{1,1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1,1,1},
	{1,-1,-1,1,-1,1,1,-1,1,-1,-1,1,-1,1,1,-1,},
	
	{1,1,1,1,1,1,1,1, -1,-1,-1,-1,-1,-1,-1,-1},
	{1,-1,1,-1,1,-1,1,-1,-1,1,-1,1,-1,1,-1,1},
	{1,1,-1,-1,1,1,-1,-1,-1,-1,1,1,-1,-1,1,1},
	{1,-1,-1,1,1,-1,-1,1,-1,1,1,-1,-1,1,1,-1},
	{1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,1,1,1},	
	{1,-1,1,-1,-1,1,-1,1,-1,1,-1,1,1,-1,1,-1},	
	{1,1,-1,-1,-1,-1,1,1,-1,-1,1,1,1,1,-1,-1},	
	{1,-1,-1,1,-1,1,1,-1, -1,1,1,-1,1,-1,-1,1,}
};


U8 GrpSlotMapTbl[64][15]=
{
{1,1 ,2  ,8  ,9  ,10 ,15 ,8  , 10 , 16 ,2  , 7 ,15 ,7  ,16 },                  
{1,1 ,5  ,16 ,7  ,3  ,14 ,16 , 3  , 10 ,5  , 12,14 ,12 ,10 },                  
{1,2 ,1  ,15 ,5  ,5  ,12 ,16 , 6  , 11 ,2  , 16,11 ,15 ,12 },                  
{1,2 ,3  ,1  ,8  ,6  ,5  ,2  , 5  , 8  ,4  , 4 ,6  ,3  ,7  },                  
{1,2 ,16 ,6  ,6  ,11 ,15 ,5  , 12 , 1  ,15 , 12,16 ,11 ,2  },                  
{1,3 ,4  ,7  ,4  ,1  ,5  ,5  , 3  , 6  ,2  , 8 ,7  ,6  ,8  },                  
{1,4 ,11 ,3  ,4  ,10 ,9  ,2  , 11 , 2  ,10 , 12,12 ,9  ,3  },                  
{1,5 ,6  ,6  ,14 ,9  ,10 ,2  , 13 , 9  ,2  , 5 ,14 ,1  ,13 },                  
{1,6 ,10 ,10 ,4  ,11 ,7  ,13 , 16 , 11 ,13 , 6 ,4  ,1  ,16 },                  
{1,6 ,13 ,2  ,14 ,2  ,6  ,5  , 5  , 13 ,10 , 9 ,1  ,14 ,10 },                  
{1,7 ,8  ,5  ,7  ,2  ,4  ,3  , 8  , 3  ,2  , 6 ,6  ,4  ,5  },                  
{1,7 ,10 ,9  ,16 ,7  ,9  ,15 , 1  , 8  ,16 , 8 ,15 ,2  ,2  },                  
{1,8 ,12 ,9  ,9  ,4  ,13 ,16 , 5  , 1  ,13 , 5 ,12 ,4  ,8  },                  
{1,8 ,14 ,10 ,14 ,1  ,15 ,15 , 8  , 5  ,11 , 4 ,10 ,5  ,4  },                  
{1,9 ,2  ,15 ,15 ,16 ,10 ,7  , 8  , 1  ,10 , 8 ,2  ,16 ,9  },                  
{1,9 ,15 ,6  ,16 ,2  ,13 ,14 , 10 , 11 ,7  , 4 ,5  ,12 ,3  },                  
{1,10,9  ,11 ,15 ,7  ,6  ,4  , 16 , 5  ,2  , 12,13 ,3  ,14 },                  
{1,11,14 ,4  ,13 ,2  ,9  ,10 , 12 , 16 ,8  , 5 ,3  ,15 ,6  },                  
{1,12,12 ,13 ,14 ,7  ,2  ,8  , 14 , 2  ,1  , 13,11 ,8  ,11 },                  
{1,12,15 ,5  ,4  ,14 ,3  ,16 , 7  , 8  ,6  , 2 ,10 ,11 ,13 },                  
{1,15,4  ,3  ,7  ,6  ,10 ,13 , 12 , 5  ,14 , 16,8  ,2  ,11 },                  
{1,16,3  ,12 ,11 ,9  ,13 ,5  , 8  , 2  ,14 , 7 ,4  ,10 ,15 },                  
{2,2 ,5  ,10 ,16 ,11 ,3  ,10 , 11 , 8  ,5  , 13,3  ,13 ,8  },                  
{2,2 ,12 ,3  ,15 ,5  ,8  ,3  , 5  , 14 ,12 , 9 ,8  ,9  ,14 },                  
{2,3 ,6  ,16 ,12 ,16 ,3  ,13 , 13 , 6  ,7  , 9 ,2  ,12 ,7  },                  
{2,3 ,8  ,2  ,9  ,15 ,14 ,3  , 14 , 9  ,5  , 5 ,15 ,8  ,12 },                  
{2,4 ,7  ,9  ,5  ,4  ,9  ,11 , 2  , 14 ,5  , 14,11 ,16 ,16 },                  
{2,4 ,13 ,12 ,12 ,7  ,15 ,10 , 5  , 2  ,15 , 5 ,13 ,7  ,4  },                  
{2,5 ,9  ,9  ,3  ,12 ,8  ,14 , 15 , 12 ,14 , 5 ,3  ,2  ,15 },                  
{2,5 ,11 ,7  ,2  ,11 ,9  ,4  , 16 , 7  ,16 , 9 ,14 ,14 ,4  },                  
{2,6 ,2  ,13 ,3  ,3  ,12 ,9  , 7  , 16 ,6  , 9 ,16 ,13 ,12 },                  
{2,6 ,9  ,7  ,7  ,16 ,13 ,3  , 12 , 2  ,13 , 12,9  ,16 ,6  },                  
{2,7 ,12 ,15 ,2  ,12 ,4  ,10 , 13 , 15 ,13 , 4 ,5  ,5  ,10 },                  
{2,7 ,14 ,16 ,5  ,9  ,2  ,9  , 16 , 11 ,11 , 5 ,7  ,4  ,14 },                  
{2,8 ,5  ,12 ,5  ,2  ,14 ,14 , 8  , 15 ,3  , 9 ,12 ,15 ,9  },                  
{2,9 ,13 ,4  ,2  ,13 ,8  ,11 , 6  , 4  ,6  , 8 ,15 ,15 ,11 },                  
{2,10,3  ,2  ,13 ,16 ,8  ,10 , 8  , 13 ,11 , 11,16 ,3  ,5  },                  
{2,11,15 ,3  ,11 ,6  ,14 ,10 , 15 , 10 ,6  , 7 ,7  ,14 ,3  },                  
{2,16,4  ,5  ,16 ,14 ,7  ,11 , 4  , 11 ,14 , 9 ,9  ,7  ,5  },                  
{3,3 ,4  ,6  ,11 ,12 ,13 ,6  , 12 , 14 ,4  , 5 ,13 ,5  ,14 },                  
{3,3 ,6  ,5  ,16 ,9  ,15 ,5  , 9  , 10 ,6  , 4 ,15 ,4  ,10 },                  
{3,4 ,5  ,14 ,4  ,6  ,12 ,13 , 5  , 13 ,6  , 11,11 ,12 ,14 },                  
{3,4 ,9  ,16 ,10 ,4  ,16 ,15 , 3  , 5  ,10 , 5 ,15 ,6  ,6  },                  
{3,4 ,16 ,10 ,5  ,10 ,4  ,9  , 9  , 16 ,15 , 6 ,3  ,5  ,15 },                  
{3,5 ,12 ,11 ,14 ,5  ,11 ,13 , 3  , 6  ,14 , 6 ,13 ,4  ,4  },                  
{3,6 ,4  ,10 ,6  ,5  ,9  ,15 , 4  , 15 ,5  , 16,16 ,9  ,10 },                  
{3,7 ,8  ,8  ,16 ,11 ,12 ,4  , 15 , 11 ,4  , 7 ,16 ,3  ,15 },                  
{3,7 ,16 ,11 ,4  ,15 ,3  ,15 , 11 , 12 ,12 , 4 ,7  ,8  ,16 },                  
{3,8 ,7  ,15 ,4  ,8  ,15 ,12 , 3  , 16 ,4  , 16,12 ,11 ,11 },                  
{3,8 ,15 ,4  ,16 ,4  ,8  ,7  , 7  , 15 ,12 , 11,3  ,16 ,12 },                  
{3,10,10 ,15 ,16 ,5  ,4  ,6  , 16 , 4  ,3  , 15,9  ,6  ,9  },                  
{3,13,11 ,5  ,4  ,12 ,4  ,11 , 6  , 6  ,5  , 3 ,14 ,13 ,12 },                  
{3,14,7  ,9  ,14 ,10 ,13 ,8  , 7  , 8  ,10 , 4 ,4  ,13 ,9  },                  
{5,5 ,8  ,14 ,16 ,13 ,6  ,14 , 13 , 7  ,8  , 15,6  ,15 ,7  },                  
{5,6 ,11 ,7  ,10 ,8  ,5  ,8  , 7  , 12 ,12 , 10,6  ,9  ,11 },                  
{5,6 ,13 ,8  ,13 ,5  ,7  ,7  , 6  , 16 ,14 , 15,8  ,16 ,15 },                  
{5,7 ,9  ,10 ,7  ,11 ,6  ,12 , 9  , 12 ,11 , 8 ,8  ,6  ,10 },                  
{5,9 ,6  ,8  ,10 ,9  ,8  ,12 , 5  , 11 ,10 , 11,12 ,7  ,7  },                  
{5,10,10 ,12 ,8  ,11 ,9  ,7  , 8  , 9  ,5  , 12,6  ,7  ,6  },                  
{5,10,12 ,6  ,5  ,12 ,8  ,9  , 7  , 6  ,7  , 8 ,11 ,11 ,9  },                  
{5,13,15 ,15 ,14 ,8  ,6  ,7  , 16 , 8  ,7  , 13,14 ,5  ,16 },                  
{9,10,13 ,10 ,11 ,15 ,15 ,9  , 16 , 12 ,14 , 13,16 ,14 ,11 },                  
{9,11,12 ,15 ,12 ,9  ,13 ,13 , 11 , 14 ,10 , 16,15 ,14 ,16 },                  
{9,12,10 ,15 ,13 ,14 ,9  ,14 , 15 , 11 ,11 , 13,12 ,16 ,10 }                
};


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   configure cell search parameters

 Function SetSearchCfgPara.

 @param    pCellSrchPara = a global variable, it is the store the configure  paremeters .
 @param    meaPeroid = number of frames of rscp non-conherernt length.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void SetSearchCfgPara(CellSrchConfigPara_t *pCellSrchPara,U8 meaPeroid)
{
	//the following parameters could be change in debug version for performance improvement
	pCellSrchPara->pschAccLen = 1;//number of frames of psch non-conherernt length
	pCellSrchPara->sschAccLen = 1;//number of frames of ssch non-conherernt length
	pCellSrchPara->pcpichAccLen = 1;//number of frames of cpich non-conherernt length
	pCellSrchPara->rxAntNumber = 1;
   	pCellSrchPara->pmAccLen = PM_ACC_SLOT_NUM;        //number of slots of path PDP correlation non-conherernt length
	pCellSrchPara->rscpAccLen =meaPeroid;//number of frames of rscp non-conherernt length
    
	pCellSrchPara->pschThsdFactor = 5;// threshold for psch detection
	pCellSrchPara->sschThsdFactor = 2;// threshold for ssch detection
	pCellSrchPara->pcpichThsdFactor = 3;// threshold for cpich detection
	pCellSrchPara->pmThsdFactor = 6;    // threshold for path monitor detection
	pCellSrchPara->pschSubWinLen = 1280;  // subwindow lenght for  psch correlation
	pCellSrchPara->pcpichConherentLen = 2; //cpich coherernt is 2*256 chip
	pCellSrchPara->maxPmTskNum = 5; // process 5 cells path monitor each time

}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   set the information for BCH decode, these information is got in cell  searcher  side.

 Function SetSharePara.

 @param    index = the arrary index which save shared information .
 @param    pCellInfo = the pointer of cell searcher result.
 @param    angle = the angle of initial frequency offset .
 @param    pNwlCommInfo = the global pointer of global variable which save  share information.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void SetSharePara(U8 index,CellPathInfo *pCellInfo, S16 angle,NWL_CommInfor  *pNwlCommInfo )
{
    U8 idx;
    NWL_CommInfor *pInf = NULL;
    
    if(index>MAX_CELLLSRCH_NUM)
    {
        printf("set shared parameter err: index--%d \n", index);
		return;
    }

    pInf = pNwlCommInfo+index;

    pInf->Psc = pCellInfo->PIndx;
    pInf->WinBegin = pCellInfo->WinBegin;
    pInf->SlotNum = pCellInfo->SlotNum;

    for(idx=0;idx<PM_MAX_FINGER_NUM;idx++)
    {
        if(0 != pCellInfo->Path[idx].PathPower)
        {            
            pInf->PathOffset[idx] = (U16)(pCellInfo->Path[idx].PathOffset);
        }
    }

    pInf->FingerNum = pCellInfo->FingNum;
        
    pInf->InitAngle= angle;
    pInf->SetFlag = 1;

    return;
}






void ZSeqGen(S8* pcOutputAdrr)
{
	S8 acSeq2[16] = {1, 1, 1, 1, 1, 1, -1, -1, -1, 1, -1, 1, -1, 1, 1, -1};
	S8 acSeq1[16] = {1, 1, 1, -1, 1, 1,-1,-1,1,-1,1,-1,-1,-1,-1,-1};
	S32 loop1;
	S32 loop2;

	for(loop1 = 0;loop1<16;loop1++)
	{
		for(loop2 = 0;loop2<16;loop2++)
		{
			*pcOutputAdrr = acSeq1[loop1]*acSeq2[loop2];
			 pcOutputAdrr ++;
			
		}
	}
	return;
}



void PmParaCala(CellGrpDetcInfo_t *cellGrpDetcInfo,PschPeakPos_t  *pschPeakPos,PschPeak_t *pschPeakInfo,U16 PkIdx)
{
    U32 loop;
    U32 cellMaxPow;
    U32 peakMaxPow;
	
	if(0==cellGrpDetcInfo->CellNum)
	{
		if(pschPeakPos->Position[PkIdx]>=PM_SEARCH_WIN_LEN)
		{
			cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].WinBegin = 
				pschPeakPos->Position[PkIdx]-PM_SEARCH_WIN_LEN;
		}
		else
		{
			cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].WinBegin = 0;
		}
		cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].PIndx =  
			pschPeakInfo->PsynPk.PIndex[PkIdx];
        	cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].SlotNum=  
			pschPeakInfo->PsynPk.SlotNum[PkIdx];
            
       cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].maxPow = 
                   pschPeakPos->Power[PkIdx];
        cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].maxPos = 
                    pschPeakPos->Position[PkIdx];
	cellGrpDetcInfo->CellNum ++;
	}
	else
	{
		for(loop=0;loop<cellGrpDetcInfo->CellNum;loop++)
		{
			if(cellGrpDetcInfo->CellPathInfo[loop].PIndx==pschPeakInfo->PsynPk.PIndex[PkIdx])
			{
				break;
			}
		}
		if(loop==cellGrpDetcInfo->CellNum)
		{
			if(pschPeakPos->Position[PkIdx]>=PM_SEARCH_WIN_LEN)
			{
				cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].WinBegin = 
					pschPeakPos->Position[PkIdx]-PM_SEARCH_WIN_LEN;
			}
			else
			{
				cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].WinBegin = 0;
			}
			cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].PIndx =  
			pschPeakInfo->PsynPk.PIndex[PkIdx];

                    cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].SlotNum=  
			pschPeakInfo->PsynPk.SlotNum[PkIdx];
                   
                     cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].maxPow = 
                       pschPeakPos->Power[PkIdx];
                    cellGrpDetcInfo->CellPathInfo[cellGrpDetcInfo->CellNum].maxPos = 
                        pschPeakPos->Position[PkIdx];
	            cellGrpDetcInfo->CellNum ++;

		}
              else
              {
                    cellMaxPow = cellGrpDetcInfo->CellPathInfo[loop].maxPow ;
                    peakMaxPow = pschPeakPos->Power[PkIdx];
                    
                    if(peakMaxPow>cellMaxPow)
                    {
                        cellGrpDetcInfo->CellPathInfo[loop].maxPow = 
                            pschPeakPos->Power[PkIdx];
                        cellGrpDetcInfo->CellPathInfo[loop].maxPos = 
                            pschPeakPos->Position[PkIdx];
                    }
              }
	}
	return ;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief    calculate the mean pow for Psch correlate , just use ant 0 .

 Function PscMeanCalc.

 @param    pPschAnt0Pow = the power of antenna 0 .
 @param    pPschAnt1Pow = the power of antenna 1.
 @param    antNum = the antenna number, should be 1 always for NWL.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32	PscMeanCalc(U32* pPschAnt0Pow,U32* pPschAnt1Pow,U8 antNum)
{
	U32	 meanVal;
	U32	 i,j;
	U32	peakPos[MAX_CELLLSRCH_NUM];
	U32	peakpow[MAX_CELLLSRCH_NUM];
	meanVal = 0;
	for( i = 0; i < MAX_CELLLSRCH_NUM; i++)
	{
		peakpow[i] = 0;
		for( j =0 ; j < CHIP_NUM_PER_SLOT*2; j++)
		{
			if(pPschAnt0Pow[j]>peakpow[i])
			{
				peakPos[i] = j;
				peakpow[i] = pPschAnt0Pow[j];
			}

			if(0==i)
			{
				meanVal +=pPschAnt0Pow[j];
			}

		}
		meanVal -=pPschAnt0Pow[peakPos[i]];
		pPschAnt0Pow[peakPos[i]] = 0;
		if(0==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[1];
			meanVal -=pPschAnt0Pow[2];
			meanVal -=pPschAnt0Pow[3];
			meanVal -=pPschAnt0Pow[4];
			pPschAnt0Pow[1] = 0;
			pPschAnt0Pow[2] = 0;
			pPschAnt0Pow[3] = 0;
			pPschAnt0Pow[4] = 0;
		}
		else if(1==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[0];
			meanVal -=pPschAnt0Pow[2];
			meanVal -=pPschAnt0Pow[3];
			meanVal -=pPschAnt0Pow[4];
			pPschAnt0Pow[0] = 0;
			pPschAnt0Pow[2] = 0;
			pPschAnt0Pow[3] = 0;
			pPschAnt0Pow[4] = 0;
		}
		else if(2==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[1];
			meanVal -=pPschAnt0Pow[3];
			meanVal -=pPschAnt0Pow[4];
			meanVal -=pPschAnt0Pow[5];
			pPschAnt0Pow[1] = 0;
			pPschAnt0Pow[3] = 0;
			pPschAnt0Pow[4] = 0;
			pPschAnt0Pow[5] = 0;
		}
		else if((CHIP_NUM_PER_SLOT*2-1)==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-1];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-2];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4];
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-1] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-2] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4] = 0;
		}
		else if((CHIP_NUM_PER_SLOT*2-2)==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-2];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4];
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-2] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4] = 0;
		}
		else if((CHIP_NUM_PER_SLOT*2-3)==peakPos[i])
		{
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-1];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4];
			meanVal -=pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-5];
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-1] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-3] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-4] = 0;
			pPschAnt0Pow[(CHIP_NUM_PER_SLOT*2-1)-5] = 0;
		}
		else
		{
			meanVal -=pPschAnt0Pow[peakPos[i]+1];
			meanVal -=pPschAnt0Pow[peakPos[i]-1];
			meanVal -=pPschAnt0Pow[peakPos[i]+3];
			meanVal -=pPschAnt0Pow[peakPos[i]-3];
			pPschAnt0Pow[peakPos[i]+1] = 0;
			pPschAnt0Pow[peakPos[i]-1] = 0;
			pPschAnt0Pow[peakPos[i]+3] = 0;
			pPschAnt0Pow[peakPos[i]-3] = 0;	
		}
	}
	for( i = 0; i < MAX_CELLLSRCH_NUM; i++)
	{
		pPschAnt0Pow[peakPos[i]] = peakpow[i];
	}
	meanVal /=(CHIP_NUM_PER_SLOT*2 -MAX_CELLLSRCH_NUM*5 );
	return meanVal;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief    detect the peak of PSCH.

 Function PschPeakDetc.

 @param    Ant0Power = the power of antenna 0 .
 @param    Ant1Power = the power of antenna 1.
 @param    Thsd = the threshold of detect peak.
 @param    antNum = the antenna number, should be 1 always for NWL.
 @param    PeakInfo = the detected peak information.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void PschPeakDetc(U32 Ant0Power[],U32 Ant1Power[],U32 Thsd,U8 AntNum,PschPeakPos_t* PeakInfo)
{

	U32 loop;	
	PeakInfo->PkNum = 0;
	if(1==AntNum)
	{
		for(loop = 0;loop<CHIP_NUM_PER_SLOT*SAMPLE_RATE;loop++)
		{
			if(MAX_PEAK_NUM==PeakInfo->PkNum)
			{
				//need add warning !!!
				break;
			}
			if(Ant0Power[loop]>Thsd)
			{
				PeakInfo->Position[PeakInfo->PkNum]=(U16) loop;
				PeakInfo->Power[PeakInfo->PkNum]= Ant0Power[loop];
				PeakInfo->Flag[PeakInfo->PkNum]=1;
				PeakInfo->PkNum++;				
			}
		}
	}
	else
	{
		for(loop = 0;loop<CHIP_NUM_PER_SLOT*SAMPLE_RATE;loop++)
		{
			if(MAX_PEAK_NUM==PeakInfo->PkNum)
			{
				//need add warning !!!
				break;
			}
			if(((Ant0Power[loop]+Ant1Power[loop])>>1)>Thsd)
			{
				PeakInfo->Position[PeakInfo->PkNum]=(U16) loop;
				PeakInfo->Power[PeakInfo->PkNum]= ((Ant0Power[loop]+Ant1Power[loop])>>1);
				PeakInfo->Flag[PeakInfo->PkNum]=1;
				PeakInfo->PkNum++;
			}
		}
	}	
	return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief    detect the cell primary scrambing code based on PCPICH correlation results.

 Function CellPscDetec.

 @param    conherentLen = coherent process length .
 @param    PkIdx = the arrary index of peak information struct.
 @param    ThsdFactor = the threshold of detect peak.
 @param    pschPeakInfo = the detected peak information.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellPscDetec(U8 conherentLen, U16 PkIdx,U8 ThsdFactor,PschPeak_t *pschPeakInfo )
{
	U32	i,j;
	U8	taskNum;
	U32	pscAcc[8]={0,0,0,0,0,0,0,0};
	U8	peakIdx;
	U32	AccMax = 0;
	U8	MaxPos = 0;
	U32	meanVal = 0;
	U8	thresholdFactor;
	U8	pscGrpNo;
	taskNum =CHIP_NUM_PER_SLOT/(conherentLen*256);
	peakIdx = (U8)PkIdx;
	thresholdFactor = ThsdFactor;
	pscGrpNo =pschPeakInfo->PsynPk.GrpNum[peakIdx];
	for(i = 0; i < taskNum; i++)
	{
		for(j = 0; j < 8; j++)
		{
			pscAcc[j] +=pschPeakInfo->PsynPk.CpiAcc[peakIdx][i][j];
		}
	}
	for(i = 0; i < 8; i++)
	{
		if(pscAcc[i]>=AccMax)
		{
			AccMax = pscAcc[i];
			MaxPos = (U8)i;
		}
		meanVal +=pscAcc[i];
	}
	meanVal =(meanVal -AccMax)/7;
	//uart_printf("cpichMaxPow,%d,Mean,%d,Factor,%d\n",AccMax,meanVal,(AccMax/meanVal));
	if(AccMax>(thresholdFactor*meanVal))
	{
		pschPeakInfo->PsynPk.PIndex[peakIdx]=pscGrpNo*8 + MaxPos;
	}
	else
	{
		pschPeakInfo->PsynPk.PIndex[peakIdx] = 0xffff;
	}
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief    calculate the frequency angle 

 Function GetFreAngle.

 @param    cellGrpDetcInfo = the detected cell information .
 @param    tskIdx = the arrary index of cell information struct.
 @param    tskNum = the number of peaks which got based on pate searcher.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void GetFreAngle(CellGrpDetcInfo_t *cellGrpDetcInfo,U8 tskIdx, U8 tskNum)
{
	U8 idx;
	float slot_angle[15];
	float slot_aver = 0;
	U8	i;
	CellPathInfo  *pCellPathInfo;
	S32 Sum_Temp_I = 0;
	S32 Sum_Temp_Q = 0;
	S32 Sum_I[15];
	S32 Sum_Q[15];
	S32 I_1frame = 0;
	S32 Q_1frame = 0;
	


	memset(&Sum_I[0],0,sizeof(Sum_I));
	memset(&Sum_Q[0],0,sizeof(Sum_Q));

       for( i = 0; i < tskNum; i++)
	{
	     pCellPathInfo= &cellGrpDetcInfo->CellPathInfo[tskIdx+i];
            for(idx=0;idx<(15*10-2);idx++)
            {
                Sum_Temp_I = (pCellPathInfo->freqConjSym[idx][0]*pCellPathInfo->freqConjSym[idx+1][0] + 
                    pCellPathInfo->freqConjSym[idx][1]*pCellPathInfo->freqConjSym[idx+1][1])>>13;    
                Sum_Temp_Q= (pCellPathInfo->freqConjSym[idx][0]*pCellPathInfo->freqConjSym[idx+1][1] -
                    pCellPathInfo->freqConjSym[idx][1]*pCellPathInfo->freqConjSym[idx+1][0])>>13;
                Sum_I[idx/10] += Sum_Temp_I >> 2;
                Sum_Q[idx/10] += Sum_Temp_Q >> 2;
                //a_angle [idx] = atan((float)Sum_Temp_Q/(float)Sum_Temp_I);
                slot_angle [idx/10] = (float)atan((float)Sum_Q[idx/10]/(float)Sum_I[idx/10]);

            }
			

			for(idx=0;idx<15;idx++)
			{
				slot_aver += slot_angle [idx];
				I_1frame += Sum_I[idx]; 
				Q_1frame += Sum_Q[idx];
			}

			
			slot_aver /= 15;

			if(0>I_1frame)
			{
				if(Q_1frame >= 0)
				{
					slot_aver = (float)3.1416 + slot_aver;
				}
				else
				{
					slot_aver = slot_aver - (float)3.1416 ;
				}
			}
		
		    pCellPathInfo->angle = (S16)(slot_aver*0x1000);
		   
	}   		
}

