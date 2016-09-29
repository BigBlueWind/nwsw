
//////////////////////////////////////////////////////////////////////////
// CRAMHeapMgr.h: interface for the CRAM Heap Manager.
//////////////////////////////////////////////////////////////////////////

#ifndef __HEAPMGR_H__
#define __HEAPMGR_H__


#ifdef CEVAAPI_EXPORT
#define CEVAAPI __declspec(dllexport)
extern "C" {
#else
#define CEVAAPI __declspec(dllimport)
#endif

#define MAX_FRAGMENTAION 256
#define ROUND_UP(_v, b)  (((DWORD)(_v)+b-1)&~(b-1))

typedef struct tagHEAPHEADER {
	struct tagHEAPHEADER *Flink;
	struct tagHEAPHEADER *Blink;
	DWORD  size;
	DWORD  pBuffer;
	WORD   alignment;
	WORD   bUsed;
} HEAPHEADER, *PHEAPHEADER;

// Heap Manager helpers
void CEVAAPI HmCRAMHeapInit(DWORD size, DWORD buffer, WORD alignment);
PVOID CEVAAPI HmCRAMAlloc(DWORD size);
DWORD CEVAAPI HmCRAMFree(PVOID p);

#ifdef CEVAAPI_EXPORT
}
#endif


#endif //__HEAPMGR_H__
