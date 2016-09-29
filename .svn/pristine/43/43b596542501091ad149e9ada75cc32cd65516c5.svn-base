//-------------------------------------------------------------------------------------------
/** @file devinfo.c
 *
 * @brief Device Information functions - the module is intended to collect all the supported devices differences
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "devinfo.h"
#include "heaps.h"
#include "sysddrctrl.h"

const ObjectInfo T4KDevTable[] =
{// the table tracks T4K HWID specific peripherals numbers
    {DEVID_ARM_CORE,                4,       NULL},
    {DEVID_DSP_CORE,                10,      NULL},
    {DEVID_L1_CACHE,                1,       NULL},
    {DEVID_L2_CACHE,                1,       NULL},
    {DEVID_FEC,                     2,       NULL},
    {DEVID_FP,                      10,      NULL},
    {DEVID_MDMA,                    3,       NULL},
    {DEVID_GEMAC,                   2,       NULL},
    {DEVID_DDR,                     2,       NULL},
    {DEVID_CPRI,                    1,       NULL},
    {DEVID_SRIO,                    1,       NULL},
    {DEVID_PCIE,                    1,       NULL},
    {DEVID_SPA,                     1,       NULL},
    {DEVID_TIMER,                   2,       NULL},
    {DEVID_I2C,                     1,       NULL},
    {DEVID_SPI,                     1,       NULL},

    {DEVID_END,                     0,       NULL}
};

const ObjectInfo T3KDevTable[] =
{// the table tracks T4K HWID specific peripherals numbers
    {DEVID_ARM_CORE,                4,       NULL},
    {DEVID_DSP_CORE,                10,      NULL},
    {DEVID_L1_CACHE,                1,       NULL},
    {DEVID_L2_CACHE,                1,       NULL},
    {DEVID_FEC,                     2,       NULL},
    {DEVID_FP,                      10,      NULL},
    {DEVID_MDMA,                    3,       NULL},
    {DEVID_GEMAC,                   2,       NULL},
    {DEVID_DDR,                     2,       NULL},
    {DEVID_CPRI,                    1,       NULL},
    {DEVID_SRIO,                    1,       NULL},
    {DEVID_PCIE,                    1,       NULL},
    {DEVID_SPA,                     1,       NULL},
    {DEVID_TIMER,                   2,       NULL},
    {DEVID_I2C,                     1,       NULL},
    {DEVID_SPI,                     1,       NULL},

    {DEVID_END,                     0,       NULL}
};


const ObjectInfo T4KMemTable[] =
{// the table tracks T4K HWID specific various memory sizes
    {MEMID_DDR0,                    APP_HEAP_SIZE,                  NULL},
    {MEMID_DDR0_NCNB,               NCNB_HEAP_SIZE,                 NULL},
    {MEMID_DDR1,                    DDR1_CACHEABLE_HEAP_SIZE,       NULL},
    {MEMID_DDR1_NCNB,               MEMID_SIZE_DYNAMIC,             GetDDR1NcNbHeapSize},
    {MEMID_CRAM,                    0x00300000 - (CRAM_VAR_SIZE),   NULL},
    {MEMID_IRAM,                    IRAM_HEAP_SIZE,                 NULL},

    {MEMID_END,                     0,                              NULL}
};

const ObjectInfo T3KMemTable[] =
{// the table tracks T4K HWID specific various memory sizes
    {MEMID_DDR0,                    APP_HEAP_SIZE,                  NULL},
    {MEMID_DDR0_NCNB,               NCNB_HEAP_SIZE,                 NULL},
    {MEMID_DDR1,                    DDR1_CACHEABLE_HEAP_SIZE,       NULL},
    {MEMID_DDR1_NCNB,               MEMID_SIZE_DYNAMIC,             GetDDR1NcNbHeapSize},
    {MEMID_CRAM,                    0x00300000 - (CRAM_VAR_SIZE),   NULL},
    {MEMID_IRAM,                    IRAM_HEAP_SIZE,                 NULL},

    {MEMID_END,                     0,                              NULL}
};

const ObjectInfo T4KFeatTable[] =
{// the table tracks T4K HWID specific sotfware features to be either used or not
    {FEATID_END,                   0,     NULL}
};

const ObjectInfo T3KFeatTable[] =
{// the table tracks T3K HWID specific sotfware features to be either used or not
    {FEATID_END,                   0,     NULL}
};

const ObjectInfo T4KConstTable[] =
{// the table tracks T4K HWID specific constant values
    {CONSTID_DSP_STARTID,           0,    NULL},
    {CONSTID_PRACH_DSP_NUM,         1,    NULL},
    {CONSTID_PRACH_DSP_ID_START,    9,    NULL},
    {CONSTID_IDFT_FP_ID_START,      0,    NULL},
    {CONSTID_IDFT_FP_ID_STOP,       4,    NULL},
    {CONSTID_IFFT_FP_ID_START,      5,    NULL},
    {CONSTID_IFFT_FP_ID_STOP,       9,    NULL},
    {CONSTID_FFT_FP_ID_START,       5,    NULL},
    {CONSTID_FFT_FP_ID_STOP,        9,    NULL},

    {CONSTID_END,                   0,    NULL}
};

const ObjectInfo T4Kx6ConstTable[] =
{// the table tracks T4K HWID specific constant values
    {CONSTID_DSP_STARTID,           0,    NULL},                  // for X6 DSP-0 is OK
    {CONSTID_PRACH_DSP_NUM,         1,    NULL},
    {CONSTID_PRACH_DSP_ID_START,    9,    NULL},
    {CONSTID_IDFT_FP_ID_START,      0,    NULL},
    {CONSTID_IDFT_FP_ID_STOP,       4,    NULL},
    {CONSTID_IFFT_FP_ID_START,      5,    NULL},
    {CONSTID_IFFT_FP_ID_STOP,       9,    NULL},
    {CONSTID_FFT_FP_ID_START,       5,    NULL},
    {CONSTID_FFT_FP_ID_STOP,        9,    NULL},

    {CONSTID_END,                   0,    NULL}
};


const ObjectInfo T3KConstTable[] =
{// the table tracks T4K HWID specific constant values
    {CONSTID_DSP_STARTID,           1,    NULL},
    {CONSTID_PRACH_DSP_NUM,         1,    NULL},
    {CONSTID_PRACH_DSP_ID_START,    9,    NULL},
    {CONSTID_IDFT_FP_ID_START,      0,    NULL},
    {CONSTID_IDFT_FP_ID_STOP,       4,    NULL},
    {CONSTID_IFFT_FP_ID_START,      5,    NULL},
    {CONSTID_IFFT_FP_ID_STOP,       9,    NULL},
    {CONSTID_FFT_FP_ID_START,       5,    NULL},
    {CONSTID_FFT_FP_ID_STOP,        9,    NULL},

    {CONSTID_END,                   0,    NULL}
};

const ObjectInfo* T4KInfo[] =
{
    T4KDevTable,
    T4KMemTable,
    T4KFeatTable,
    T4KConstTable
};

const ObjectInfo* T4Kx6Info[] =
{
    T4KDevTable,
    T4KMemTable,
    T4KFeatTable,
    T4Kx6ConstTable
};


const ObjectInfo* T3KInfo[] =
{
    T3KDevTable,
    T3KMemTable,
    T3KFeatTable,
    T3KConstTable
};

UINT32 DevInfo_DevManufacturerID = 0; // this variable contains identifier of T4K device x1, x6, x7 ... 

UINT32 DevInfoReadID (void)
{
    UINT32 tmp = (REG32(DEVICE_BONDING_STAT_REG) >> 1) & 0x7FF;

    UINT32 ID = HWID_T4K;

    if (tmp == 0x22D)
    {
        tmp = (UINT8)(REG32(DEVICE_BONDING_STAT_REG) >> 28);

        if (tmp == 2)
        {
            if (GetChipRevisionByTestingDdrPhy () == 7)
            {
                return HWID_T4K_X7;
            }
            else
            {
                return HWID_T4K_X6;
            }
        }
    }

    return ID;
}

MXRC DevInfoInit (void)
{
    DevInfo_DevManufacturerID = DevInfoReadID();

    return MX_OK;
}

UINT32 DevInfoGetDevRef (void)
{
    return DevInfo_DevManufacturerID;
}

UINT32 DevInfoIsT4Kx4 (void)
{
    return (DevInfo_DevManufacturerID == HWID_T4K_X4);
}

UINT32 DevInfoIsT4Kx6 (void)
{
    return (DevInfo_DevManufacturerID == HWID_T4K_X6);
}

UINT32 DevInfoIsT4Kx7 (void)
{
    return (DevInfo_DevManufacturerID == HWID_T4K_X7);
}

UINT32 DevInfoIsT2K (void)
{
    return (DevInfo_DevManufacturerID == HWID_T2K);
}

// it should return the hardwareID based on HW info
UINT32 DevInfoGetHWID(void)
{
    if (DevInfoIsT4Kx6())
        return HWID_T4K_X6;

    if (DevInfoIsT4Kx7())
        return HWID_T4K_X7;

    return HWID_T4K;
}

const ObjectInfo** DevInfoGetTable(UINT32 HWID)
{
    switch(HWID)
    {
        case HWID_T4K:
            return T4KInfo;
            
        case HWID_T4K_X6:
        case HWID_T4K_X7:
            return T4Kx6Info;
            
        case HWID_T3K:
            return T3KInfo;
            
        default:
            return T4KInfo;
    }
}

UINT32 DevInfoIsEnabled (UINT32 ObjectID)
{
    UINT32 rc = 0;

    if(ObjectID >= OBJECTID_START && ObjectID < OBJECTID_END)
    {
        UINT32 idx = 0;
        UINT32 ObjIDLast = 0;
        const ObjectInfo** pInfo;
        const ObjectInfo* pTable;

        pInfo = DevInfoGetTable(DevInfoGetHWID());

        if(ObjectID < DEVID_END)
        {
            ObjIDLast = DEVID_END;
            pTable = pInfo[DEVID_IDX];
        }
        else if(ObjectID >= MEMID_START && ObjectID < MEMID_END)
        {
            ObjIDLast = MEMID_END;
            pTable = pInfo[MEMID_IDX];
        }
        else if(ObjectID >= FEATID_START && ObjectID < FEATID_END)
        {
            ObjIDLast = FEATID_END;
            pTable = pInfo[FEATID_IDX];
        }

        if(ObjIDLast)
        {
            while(pTable[idx].ID != ObjIDLast)
            {
                if(pTable[idx].ID == ObjectID)
                {
                    rc = (pTable[idx].Value == 0)? 0 : 1;
                    break;
                }
                idx++;
            }
        }
    }

    return rc;
}

UINT32 DevInfoGetSize (MemIDs ObjectID)
{
    UINT32 rc = 0;

    if(ObjectID >= MEMID_START && ObjectID < MEMID_END)
    {
        UINT32 idx = 0;
        const ObjectInfo** pInfo;
        const ObjectInfo* pTable;

        pInfo = DevInfoGetTable(DevInfoGetHWID());
        pTable = pInfo[MEMID_IDX];

        while(pTable[idx].ID != MEMID_END)
        {
            if(pTable[idx].ID == ObjectID)
            {
                rc = pTable[idx].Value;
                break;
            }
            idx++;
        }

        if(rc == MEMID_SIZE_DYNAMIC)
        {
            if(pTable[idx].pHandler)
            {// should exist
                rc = pTable[idx].pHandler();
            }
        }
    }

    return rc;
}

UINT32 DevInfoGetNumber (DevIDs ObjectID)
{
    UINT32 rc = 0;

    if(ObjectID >= DEVID_START && ObjectID < DEVID_END)
    {
        UINT32 idx = 0;
        const ObjectInfo** pInfo;
        const ObjectInfo* pTable;

        pInfo = DevInfoGetTable(DevInfoGetHWID());
        pTable = pInfo[DEVID_IDX];

        while(pTable[idx].ID != DEVID_END)
        {
            if(pTable[idx].ID == ObjectID)
            {
                rc = pTable[idx].Value;
                break;
            }
            idx++;
        }
    }

    return rc;
}

UINT32 DevInfoGetValue (ConstIDs ObjectID)
{
    UINT32 rc = 0;

    if(ObjectID >= CONSTID_START && ObjectID < CONSTID_END)
    {
        UINT32 idx = 0;
        const ObjectInfo** pInfo;
        const ObjectInfo* pTable;

        pInfo = DevInfoGetTable(DevInfoGetHWID());
        pTable = pInfo[CONSTID_IDX];

        while(pTable[idx].ID != CONSTID_END)
        {
            if(pTable[idx].ID == ObjectID)
            {
                rc = pTable[idx].Value;
                break;
            }
            idx++;
        }
    }

    return rc;
}

