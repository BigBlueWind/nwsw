#include "syscore.h"
#include <stdlib.h>
#include "appprintf.h"

extern MXRC (*LteMacAppInitCallback)(void);
extern MXRC (*AppInitCallback)(void);

extern UINT32 MacRegMsgThread(void * p);

MXRC MyCpu0(void)
{
    MXRC rc;

    uart_printf("PHY init (%x) ...\n", AppInitCallback);

    if (AppInitCallback != NULL)
    {
        if (FAILED(rc = AppInitCallback()))
        {
            uart_printf("L-ARM PHY application failed to start, error=0x%08x\r\n", rc);
            _ASSERT_RC(rc);
        }
    }
    else
    {
        uart_printf("WARNING: No application found\r\n");
    }

    uart_printf("MAC init (%x) ...\n", LteMacAppInitCallback);

    if (LteMacAppInitCallback != NULL)
    {
        if (FAILED(rc = LteMacAppInitCallback()))
        {
            uart_printf("L-ARM MAC application failed to start, error=0x%08x\r\n", rc);
            _ASSERT_RC(rc);
        }
    }
    else
    {
        uart_printf("WARNING: No application found\r\n");
    }

    return MX_OK;
}

MXRC MyCpu1(void)
{
    SvsrSelectICpuMaster (1);
    MacRegMsgThread(NULL);
    return MX_OK;
}

MXRC MyCpu2 (void)
{
    MacRegMsgThread(NULL);
    return MX_OK;
}

MXRC MyCpu3(void)
{
    MacRegMsgThread(NULL);
    return MX_OK;
}

int main(void)
{
    SyscoreParam param;
    UINT32 clusterId = MxGetARMID();

    memset(&param, 0, sizeof(param));

    param.app_entry[0] = MyCpu0;
#ifdef CL_MAC_LWLC
    param.app_entry[1] = ((clusterId == 0) ? MyCpu1 : NULL);
    param.app_entry[2] = ((clusterId == 0) ? MyCpu2 : NULL);
#else
    param.app_entry[1] = ((clusterId != 0) ? MyCpu1 : NULL);
    param.app_entry[2] = NULL;
#endif
    param.app_entry[3] = ((clusterId == 0) ? MyCpu3 : NULL);

    param.mmu_cfg[0] = NULL;
    param.mmu_cfg[1] = NULL;
    param.mmu_cfg[2] = NULL;
    param.mmu_cfg[3] = NULL;

    return SysCoreStart(&param);
}
