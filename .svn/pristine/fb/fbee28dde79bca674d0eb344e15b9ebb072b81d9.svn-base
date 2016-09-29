
#ifndef __EXPORT__H__
#define __EXPORT__H__


#include "CRAMHeapMgr.h"

#ifdef CEVAAPI_EXPORT
#define CEVAAPI __declspec(dllexport)
extern "C" {
#else
#define CEVAAPI __declspec(dllimport)
#endif

BOOL CEVAAPI CEVAConnect(int dspid);
BOOL CEVAAPI CEVADisconnect(int dspid);

BOOL CEVAAPI CEVAInit(int dspid, PTSTR filename);

BOOL CEVAAPI CEVALoadCoff(int dspid, PTSTR filename);
BOOL CEVAAPI CRAMLoadCoff(PTSTR filename);
PBYTE CEVAAPI CRAMShadowBuf();

BOOL CEVAAPI CRAMWrite(DWORD addr, PVOID pdata, DWORD size);
BOOL CEVAAPI CRAMRead(DWORD addr, PVOID pdata, DWORD size);

BOOL CEVAAPI CEVAGenInt0(int dspid);
BOOL CEVAAPI CEVAGenNMI(int dspid);
BOOL CEVAAPI CEVAWait(int dspid, DWORD timeout);
BOOL CEVAAPI CEVAGetCycleCount(int dspid, DWORD * cnt);

typedef int (*PrintFuncPtr)(const TCHAR *, ...);
BOOL CEVAAPI CEVASetLogger(PrintFuncPtr pf);

#ifdef CEVAAPI_EXPORT
}
#endif


#endif // __EXPORT__H__
