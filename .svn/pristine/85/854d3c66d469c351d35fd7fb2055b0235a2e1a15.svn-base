#ifndef NWL_DL_GEN_PSC_
#define NWL_DL_GEN_PSC_

#ifndef VS_ENV
#include "..\\gputil_fx.h"
#include "TCB.h" 
#include "..//prototypes.h"
#endif

   
typedef struct 
{
	S8 x;		/* X coordinate */
	S8 y;		/* Y coordinate */
}Complex_S8;



typedef struct Scrambler_Info {
   S16			PriScramCode;		// primary scrambling code
   S16			SecScramCode;		// secondary scrambling code
   S16			AltScramCode;		// =0 = normal, 1= left, 2 = right
   U32			Xstate;				// x register saved off
   U32			Ystate;				// y register saved off
 } Scram_t;


#ifndef VS_ENV
void DlGenSC(Scram_t *ScramParms, U16 NumChips, Complex_S8 *pBuf)CEVAEXTERN;
void phy_DetermineScramOffset(Scram_t  *Scram)CEVAEXTERN;
void UMTSNwlGenDLPSCTask(PTCB pTCB)CEVAEXTERN;
void GenerateScrCode(U16 index, S8 *pOut)CEVAEXTERN;
U8  Mod2OnesCtr(U32 input, S16 length)CEVAEXTERN;
#else
void GenerateScrCode(U16 index, S8 *pOut);
#endif


#endif
