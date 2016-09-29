//*****************************************************************************
//**********                        BPSK                             **********
//*****************************************************************************

// SYMBOL SIZE: 1-BIT
// DATA POINTS: +-1
// DATA POINT SCALING: x 1

#if 0  // BPSK mapping table is not used
#define P_BPSK_1        32767           // Point = 1
#define B0   0x7FFF  //     P_BPSK_1
#define B1   0x8001  //    -P_BPSK_1
const unsigned short MODTAB_BPSK_256[2048] =
{

    // R0, R1, R2, R3, R4, R5, R6, R7,              // B0 B1 B2 B3 B4 B5 B6 B7

    B0, B0, B0, B0, B0, B0, B0, B0, //  0  0  0  0  0  0  0  0
    B0, B0, B0, B0, B0, B0, B0, B1, //  0  0  0  0  0  0  0  1
    B0, B0, B0, B0, B0, B0, B1, B0, //  0  0  0  0  0  0  1  0
    B0, B0, B0, B0, B0, B0, B1, B1, //  0  0  0  0  0  0  1  1
    B0, B0, B0, B0, B0, B1, B0, B0, //  0  0  0  0  0  1  0  0
    B0, B0, B0, B0, B0, B1, B0, B1, //  0  0  0  0  0  1  0  1
    B0, B0, B0, B0, B0, B1, B1, B0, //  0  0  0  0  0  1  1  0
    B0, B0, B0, B0, B0, B1, B1, B1, //  0  0  0  0  0  1  1  1
    B0, B0, B0, B0, B1, B0, B0, B0, //  0  0  0  0  1  0  0  0
    B0, B0, B0, B0, B1, B0, B0, B1, //  0  0  0  0  1  0  0  1
    B0, B0, B0, B0, B1, B0, B1, B0, //  0  0  0  0  1  0  1  0
    B0, B0, B0, B0, B1, B0, B1, B1, //  0  0  0  0  1  0  1  1
    B0, B0, B0, B0, B1, B1, B0, B0, //  0  0  0  0  1  1  0  0
    B0, B0, B0, B0, B1, B1, B0, B1, //  0  0  0  0  1  1  0  1
    B0, B0, B0, B0, B1, B1, B1, B0, //  0  0  0  0  1  1  1  0
    B0, B0, B0, B0, B1, B1, B1, B1, //  0  0  0  0  1  1  1  1

    B0, B0, B0, B1, B0, B0, B0, B0, //  0  0  0  1  0  0  0  0
    B0, B0, B0, B1, B0, B0, B0, B1, //  0  0  0  1  0  0  0  1
    B0, B0, B0, B1, B0, B0, B1, B0, //  0  0  0  1  0  0  1  0
    B0, B0, B0, B1, B0, B0, B1, B1, //  0  0  0  1  0  0  1  1
    B0, B0, B0, B1, B0, B1, B0, B0, //  0  0  0  1  0  1  0  0
    B0, B0, B0, B1, B0, B1, B0, B1, //  0  0  0  1  0  1  0  1
    B0, B0, B0, B1, B0, B1, B1, B0, //  0  0  0  1  0  1  1  0
    B0, B0, B0, B1, B0, B1, B1, B1, //  0  0  0  1  0  1  1  1
    B0, B0, B0, B1, B1, B0, B0, B0, //  0  0  0  1  1  0  0  0
    B0, B0, B0, B1, B1, B0, B0, B1, //  0  0  0  1  1  0  0  1
    B0, B0, B0, B1, B1, B0, B1, B0, //  0  0  0  1  1  0  1  0
    B0, B0, B0, B1, B1, B0, B1, B1, //  0  0  0  1  1  0  1  1
    B0, B0, B0, B1, B1, B1, B0, B0, //  0  0  0  1  1  1  0  0
    B0, B0, B0, B1, B1, B1, B0, B1, //  0  0  0  1  1  1  0  1
    B0, B0, B0, B1, B1, B1, B1, B0, //  0  0  0  1  1  1  1  0
    B0, B0, B0, B1, B1, B1, B1, B1, //  0  0  0  1  1  1  1  1

    B0, B0, B1, B0, B0, B0, B0, B0, //  0  0  1  0  0  0  0  0
    B0, B0, B1, B0, B0, B0, B0, B1, //  0  0  1  0  0  0  0  1
    B0, B0, B1, B0, B0, B0, B1, B0, //  0  0  1  0  0  0  1  0
    B0, B0, B1, B0, B0, B0, B1, B1, //  0  0  1  0  0  0  1  1
    B0, B0, B1, B0, B0, B1, B0, B0, //  0  0  1  0  0  1  0  0
    B0, B0, B1, B0, B0, B1, B0, B1, //  0  0  1  0  0  1  0  1
    B0, B0, B1, B0, B0, B1, B1, B0, //  0  0  1  0  0  1  1  0
    B0, B0, B1, B0, B0, B1, B1, B1, //  0  0  1  0  0  1  1  1
    B0, B0, B1, B0, B1, B0, B0, B0, //  0  0  1  0  1  0  0  0
    B0, B0, B1, B0, B1, B0, B0, B1, //  0  0  1  0  1  0  0  1
    B0, B0, B1, B0, B1, B0, B1, B0, //  0  0  1  0  1  0  1  0
    B0, B0, B1, B0, B1, B0, B1, B1, //  0  0  1  0  1  0  1  1
    B0, B0, B1, B0, B1, B1, B0, B0, //  0  0  1  0  1  1  0  0
    B0, B0, B1, B0, B1, B1, B0, B1, //  0  0  1  0  1  1  0  1
    B0, B0, B1, B0, B1, B1, B1, B0, //  0  0  1  0  1  1  1  0
    B0, B0, B1, B0, B1, B1, B1, B1, //  0  0  1  0  1  1  1  1

    B0, B0, B1, B1, B0, B0, B0, B0, //  0  0  1  1  0  0  0  0
    B0, B0, B1, B1, B0, B0, B0, B1, //  0  0  1  1  0  0  0  1
    B0, B0, B1, B1, B0, B0, B1, B0, //  0  0  1  1  0  0  1  0
    B0, B0, B1, B1, B0, B0, B1, B1, //  0  0  1  1  0  0  1  1
    B0, B0, B1, B1, B0, B1, B0, B0, //  0  0  1  1  0  1  0  0
    B0, B0, B1, B1, B0, B1, B0, B1, //  0  0  1  1  0  1  0  1
    B0, B0, B1, B1, B0, B1, B1, B0, //  0  0  1  1  0  1  1  0
    B0, B0, B1, B1, B0, B1, B1, B1, //  0  0  1  1  0  1  1  1
    B0, B0, B1, B1, B1, B0, B0, B0, //  0  0  1  1  1  0  0  0
    B0, B0, B1, B1, B1, B0, B0, B1, //  0  0  1  1  1  0  0  1
    B0, B0, B1, B1, B1, B0, B1, B0, //  0  0  1  1  1  0  1  0
    B0, B0, B1, B1, B1, B0, B1, B1, //  0  0  1  1  1  0  1  1
    B0, B0, B1, B1, B1, B1, B0, B0, //  0  0  1  1  1  1  0  0
    B0, B0, B1, B1, B1, B1, B0, B1, //  0  0  1  1  1  1  0  1
    B0, B0, B1, B1, B1, B1, B1, B0, //  0  0  1  1  1  1  1  0
    B0, B0, B1, B1, B1, B1, B1, B1, //  0  0  1  1  1  1  1  1

    B0, B1, B0, B0, B0, B0, B0, B0, //  0  1  0  0  0  0  0  0
    B0, B1, B0, B0, B0, B0, B0, B1, //  0  1  0  0  0  0  0  1
    B0, B1, B0, B0, B0, B0, B1, B0, //  0  1  0  0  0  0  1  0
    B0, B1, B0, B0, B0, B0, B1, B1, //  0  1  0  0  0  0  1  1
    B0, B1, B0, B0, B0, B1, B0, B0, //  0  1  0  0  0  1  0  0
    B0, B1, B0, B0, B0, B1, B0, B1, //  0  1  0  0  0  1  0  1
    B0, B1, B0, B0, B0, B1, B1, B0, //  0  1  0  0  0  1  1  0
    B0, B1, B0, B0, B0, B1, B1, B1, //  0  1  0  0  0  1  1  1
    B0, B1, B0, B0, B1, B0, B0, B0, //  0  1  0  0  1  0  0  0
    B0, B1, B0, B0, B1, B0, B0, B1, //  0  1  0  0  1  0  0  1
    B0, B1, B0, B0, B1, B0, B1, B0, //  0  1  0  0  1  0  1  0
    B0, B1, B0, B0, B1, B0, B1, B1, //  0  1  0  0  1  0  1  1
    B0, B1, B0, B0, B1, B1, B0, B0, //  0  1  0  0  1  1  0  0
    B0, B1, B0, B0, B1, B1, B0, B1, //  0  1  0  0  1  1  0  1
    B0, B1, B0, B0, B1, B1, B1, B0, //  0  1  0  0  1  1  1  0
    B0, B1, B0, B0, B1, B1, B1, B1, //  0  1  0  0  1  1  1  1

    B0, B1, B0, B1, B0, B0, B0, B0, //  0  1  0  1  0  0  0  0
    B0, B1, B0, B1, B0, B0, B0, B1, //  0  1  0  1  0  0  0  1
    B0, B1, B0, B1, B0, B0, B1, B0, //  0  1  0  1  0  0  1  0
    B0, B1, B0, B1, B0, B0, B1, B1, //  0  1  0  1  0  0  1  1
    B0, B1, B0, B1, B0, B1, B0, B0, //  0  1  0  1  0  1  0  0
    B0, B1, B0, B1, B0, B1, B0, B1, //  0  1  0  1  0  1  0  1
    B0, B1, B0, B1, B0, B1, B1, B0, //  0  1  0  1  0  1  1  0
    B0, B1, B0, B1, B0, B1, B1, B1, //  0  1  0  1  0  1  1  1
    B0, B1, B0, B1, B1, B0, B0, B0, //  0  1  0  1  1  0  0  0
    B0, B1, B0, B1, B1, B0, B0, B1, //  0  1  0  1  1  0  0  1
    B0, B1, B0, B1, B1, B0, B1, B0, //  0  1  0  1  1  0  1  0
    B0, B1, B0, B1, B1, B0, B1, B1, //  0  1  0  1  1  0  1  1
    B0, B1, B0, B1, B1, B1, B0, B0, //  0  1  0  1  1  1  0  0
    B0, B1, B0, B1, B1, B1, B0, B1, //  0  1  0  1  1  1  0  1
    B0, B1, B0, B1, B1, B1, B1, B0, //  0  1  0  1  1  1  1  0
    B0, B1, B0, B1, B1, B1, B1, B1, //  0  1  0  1  1  1  1  1

    B0, B1, B1, B0, B0, B0, B0, B0, //  0  1  1  0  0  0  0  0
    B0, B1, B1, B0, B0, B0, B0, B1, //  0  1  1  0  0  0  0  1
    B0, B1, B1, B0, B0, B0, B1, B0, //  0  1  1  0  0  0  1  0
    B0, B1, B1, B0, B0, B0, B1, B1, //  0  1  1  0  0  0  1  1
    B0, B1, B1, B0, B0, B1, B0, B0, //  0  1  1  0  0  1  0  0
    B0, B1, B1, B0, B0, B1, B0, B1, //  0  1  1  0  0  1  0  1
    B0, B1, B1, B0, B0, B1, B1, B0, //  0  1  1  0  0  1  1  0
    B0, B1, B1, B0, B0, B1, B1, B1, //  0  1  1  0  0  1  1  1
    B0, B1, B1, B0, B1, B0, B0, B0, //  0  1  1  0  1  0  0  0
    B0, B1, B1, B0, B1, B0, B0, B1, //  0  1  1  0  1  0  0  1
    B0, B1, B1, B0, B1, B0, B1, B0, //  0  1  1  0  1  0  1  0
    B0, B1, B1, B0, B1, B0, B1, B1, //  0  1  1  0  1  0  1  1
    B0, B1, B1, B0, B1, B1, B0, B0, //  0  1  1  0  1  1  0  0
    B0, B1, B1, B0, B1, B1, B0, B1, //  0  1  1  0  1  1  0  1
    B0, B1, B1, B0, B1, B1, B1, B0, //  0  1  1  0  1  1  1  0
    B0, B1, B1, B0, B1, B1, B1, B1, //  0  1  1  0  1  1  1  1

    B0, B1, B1, B1, B0, B0, B0, B0, //  0  1  1  1  0  0  0  0
    B0, B1, B1, B1, B0, B0, B0, B1, //  0  1  1  1  0  0  0  1
    B0, B1, B1, B1, B0, B0, B1, B0, //  0  1  1  1  0  0  1  0
    B0, B1, B1, B1, B0, B0, B1, B1, //  0  1  1  1  0  0  1  1
    B0, B1, B1, B1, B0, B1, B0, B0, //  0  1  1  1  0  1  0  0
    B0, B1, B1, B1, B0, B1, B0, B1, //  0  1  1  1  0  1  0  1
    B0, B1, B1, B1, B0, B1, B1, B0, //  0  1  1  1  0  1  1  0
    B0, B1, B1, B1, B0, B1, B1, B1, //  0  1  1  1  0  1  1  1
    B0, B1, B1, B1, B1, B0, B0, B0, //  0  1  1  1  1  0  0  0
    B0, B1, B1, B1, B1, B0, B0, B1, //  0  1  1  1  1  0  0  1
    B0, B1, B1, B1, B1, B0, B1, B0, //  0  1  1  1  1  0  1  0
    B0, B1, B1, B1, B1, B0, B1, B1, //  0  1  1  1  1  0  1  1
    B0, B1, B1, B1, B1, B1, B0, B0, //  0  1  1  1  1  1  0  0
    B0, B1, B1, B1, B1, B1, B0, B1, //  0  1  1  1  1  1  0  1
    B0, B1, B1, B1, B1, B1, B1, B0, //  0  1  1  1  1  1  1  0
    B0, B1, B1, B1, B1, B1, B1, B1, //  0  1  1  1  1  1  1  1

    B1, B0, B0, B0, B0, B0, B0, B0, //  1  0  0  0  0  0  0  0
    B1, B0, B0, B0, B0, B0, B0, B1, //  1  0  0  0  0  0  0  1
    B1, B0, B0, B0, B0, B0, B1, B0, //  1  0  0  0  0  0  1  0
    B1, B0, B0, B0, B0, B0, B1, B1, //  1  0  0  0  0  0  1  1
    B1, B0, B0, B0, B0, B1, B0, B0, //  1  0  0  0  0  1  0  0
    B1, B0, B0, B0, B0, B1, B0, B1, //  1  0  0  0  0  1  0  1
    B1, B0, B0, B0, B0, B1, B1, B0, //  1  0  0  0  0  1  1  0
    B1, B0, B0, B0, B0, B1, B1, B1, //  1  0  0  0  0  1  1  1
    B1, B0, B0, B0, B1, B0, B0, B0, //  1  0  0  0  1  0  0  0
    B1, B0, B0, B0, B1, B0, B0, B1, //  1  0  0  0  1  0  0  1
    B1, B0, B0, B0, B1, B0, B1, B0, //  1  0  0  0  1  0  1  0
    B1, B0, B0, B0, B1, B0, B1, B1, //  1  0  0  0  1  0  1  1
    B1, B0, B0, B0, B1, B1, B0, B0, //  1  0  0  0  1  1  0  0
    B1, B0, B0, B0, B1, B1, B0, B1, //  1  0  0  0  1  1  0  1
    B1, B0, B0, B0, B1, B1, B1, B0, //  1  0  0  0  1  1  1  0
    B1, B0, B0, B0, B1, B1, B1, B1, //  1  0  0  0  1  1  1  1

    B1, B0, B0, B1, B0, B0, B0, B0, //  1  0  0  1  0  0  0  0
    B1, B0, B0, B1, B0, B0, B0, B1, //  1  0  0  1  0  0  0  1
    B1, B0, B0, B1, B0, B0, B1, B0, //  1  0  0  1  0  0  1  0
    B1, B0, B0, B1, B0, B0, B1, B1, //  1  0  0  1  0  0  1  1
    B1, B0, B0, B1, B0, B1, B0, B0, //  1  0  0  1  0  1  0  0
    B1, B0, B0, B1, B0, B1, B0, B1, //  1  0  0  1  0  1  0  1
    B1, B0, B0, B1, B0, B1, B1, B0, //  1  0  0  1  0  1  1  0
    B1, B0, B0, B1, B0, B1, B1, B1, //  1  0  0  1  0  1  1  1
    B1, B0, B0, B1, B1, B0, B0, B0, //  1  0  0  1  1  0  0  0
    B1, B0, B0, B1, B1, B0, B0, B1, //  1  0  0  1  1  0  0  1
    B1, B0, B0, B1, B1, B0, B1, B0, //  1  0  0  1  1  0  1  0
    B1, B0, B0, B1, B1, B0, B1, B1, //  1  0  0  1  1  0  1  1
    B1, B0, B0, B1, B1, B1, B0, B0, //  1  0  0  1  1  1  0  0
    B1, B0, B0, B1, B1, B1, B0, B1, //  1  0  0  1  1  1  0  1
    B1, B0, B0, B1, B1, B1, B1, B0, //  1  0  0  1  1  1  1  0
    B1, B0, B0, B1, B1, B1, B1, B1, //  1  0  0  1  1  1  1  1

    B1, B0, B1, B0, B0, B0, B0, B0, //  1  0  1  0  0  0  0  0
    B1, B0, B1, B0, B0, B0, B0, B1, //  1  0  1  0  0  0  0  1
    B1, B0, B1, B0, B0, B0, B1, B0, //  1  0  1  0  0  0  1  0
    B1, B0, B1, B0, B0, B0, B1, B1, //  1  0  1  0  0  0  1  1
    B1, B0, B1, B0, B0, B1, B0, B0, //  1  0  1  0  0  1  0  0
    B1, B0, B1, B0, B0, B1, B0, B1, //  1  0  1  0  0  1  0  1
    B1, B0, B1, B0, B0, B1, B1, B0, //  1  0  1  0  0  1  1  0
    B1, B0, B1, B0, B0, B1, B1, B1, //  1  0  1  0  0  1  1  1
    B1, B0, B1, B0, B1, B0, B0, B0, //  1  0  1  0  1  0  0  0
    B1, B0, B1, B0, B1, B0, B0, B1, //  1  0  1  0  1  0  0  1
    B1, B0, B1, B0, B1, B0, B1, B0, //  1  0  1  0  1  0  1  0
    B1, B0, B1, B0, B1, B0, B1, B1, //  1  0  1  0  1  0  1  1
    B1, B0, B1, B0, B1, B1, B0, B0, //  1  0  1  0  1  1  0  0
    B1, B0, B1, B0, B1, B1, B0, B1, //  1  0  1  0  1  1  0  1
    B1, B0, B1, B0, B1, B1, B1, B0, //  1  0  1  0  1  1  1  0
    B1, B0, B1, B0, B1, B1, B1, B1, //  1  0  1  0  1  1  1  1

    B1, B0, B1, B1, B0, B0, B0, B0, //  1  0  1  1  0  0  0  0
    B1, B0, B1, B1, B0, B0, B0, B1, //  1  0  1  1  0  0  0  1
    B1, B0, B1, B1, B0, B0, B1, B0, //  1  0  1  1  0  0  1  0
    B1, B0, B1, B1, B0, B0, B1, B1, //  1  0  1  1  0  0  1  1
    B1, B0, B1, B1, B0, B1, B0, B0, //  1  0  1  1  0  1  0  0
    B1, B0, B1, B1, B0, B1, B0, B1, //  1  0  1  1  0  1  0  1
    B1, B0, B1, B1, B0, B1, B1, B0, //  1  0  1  1  0  1  1  0
    B1, B0, B1, B1, B0, B1, B1, B1, //  1  0  1  1  0  1  1  1
    B1, B0, B1, B1, B1, B0, B0, B0, //  1  0  1  1  1  0  0  0
    B1, B0, B1, B1, B1, B0, B0, B1, //  1  0  1  1  1  0  0  1
    B1, B0, B1, B1, B1, B0, B1, B0, //  1  0  1  1  1  0  1  0
    B1, B0, B1, B1, B1, B0, B1, B1, //  1  0  1  1  1  0  1  1
    B1, B0, B1, B1, B1, B1, B0, B0, //  1  0  1  1  1  1  0  0
    B1, B0, B1, B1, B1, B1, B0, B1, //  1  0  1  1  1  1  0  1
    B1, B0, B1, B1, B1, B1, B1, B0, //  1  0  1  1  1  1  1  0
    B1, B0, B1, B1, B1, B1, B1, B1, //  1  0  1  1  1  1  1  1

    B1, B1, B0, B0, B0, B0, B0, B0, //  1  1  0  0  0  0  0  0
    B1, B1, B0, B0, B0, B0, B0, B1, //  1  1  0  0  0  0  0  1
    B1, B1, B0, B0, B0, B0, B1, B0, //  1  1  0  0  0  0  1  0
    B1, B1, B0, B0, B0, B0, B1, B1, //  1  1  0  0  0  0  1  1
    B1, B1, B0, B0, B0, B1, B0, B0, //  1  1  0  0  0  1  0  0
    B1, B1, B0, B0, B0, B1, B0, B1, //  1  1  0  0  0  1  0  1
    B1, B1, B0, B0, B0, B1, B1, B0, //  1  1  0  0  0  1  1  0
    B1, B1, B0, B0, B0, B1, B1, B1, //  1  1  0  0  0  1  1  1
    B1, B1, B0, B0, B1, B0, B0, B0, //  1  1  0  0  1  0  0  0
    B1, B1, B0, B0, B1, B0, B0, B1, //  1  1  0  0  1  0  0  1
    B1, B1, B0, B0, B1, B0, B1, B0, //  1  1  0  0  1  0  1  0
    B1, B1, B0, B0, B1, B0, B1, B1, //  1  1  0  0  1  0  1  1
    B1, B1, B0, B0, B1, B1, B0, B0, //  1  1  0  0  1  1  0  0
    B1, B1, B0, B0, B1, B1, B0, B1, //  1  1  0  0  1  1  0  1
    B1, B1, B0, B0, B1, B1, B1, B0, //  1  1  0  0  1  1  1  0
    B1, B1, B0, B0, B1, B1, B1, B1, //  1  1  0  0  1  1  1  1

    B1, B1, B0, B1, B0, B0, B0, B0, //  1  1  0  1  0  0  0  0
    B1, B1, B0, B1, B0, B0, B0, B1, //  1  1  0  1  0  0  0  1
    B1, B1, B0, B1, B0, B0, B1, B0, //  1  1  0  1  0  0  1  0
    B1, B1, B0, B1, B0, B0, B1, B1, //  1  1  0  1  0  0  1  1
    B1, B1, B0, B1, B0, B1, B0, B0, //  1  1  0  1  0  1  0  0
    B1, B1, B0, B1, B0, B1, B0, B1, //  1  1  0  1  0  1  0  1
    B1, B1, B0, B1, B0, B1, B1, B0, //  1  1  0  1  0  1  1  0
    B1, B1, B0, B1, B0, B1, B1, B1, //  1  1  0  1  0  1  1  1
    B1, B1, B0, B1, B1, B0, B0, B0, //  1  1  0  1  1  0  0  0
    B1, B1, B0, B1, B1, B0, B0, B1, //  1  1  0  1  1  0  0  1
    B1, B1, B0, B1, B1, B0, B1, B0, //  1  1  0  1  1  0  1  0
    B1, B1, B0, B1, B1, B0, B1, B1, //  1  1  0  1  1  0  1  1
    B1, B1, B0, B1, B1, B1, B0, B0, //  1  1  0  1  1  1  0  0
    B1, B1, B0, B1, B1, B1, B0, B1, //  1  1  0  1  1  1  0  1
    B1, B1, B0, B1, B1, B1, B1, B0, //  1  1  0  1  1  1  1  0
    B1, B1, B0, B1, B1, B1, B1, B1, //  1  1  0  1  1  1  1  1

    B1, B1, B1, B0, B0, B0, B0, B0, //  1  1  1  0  0  0  0  0
    B1, B1, B1, B0, B0, B0, B0, B1, //  1  1  1  0  0  0  0  1
    B1, B1, B1, B0, B0, B0, B1, B0, //  1  1  1  0  0  0  1  0
    B1, B1, B1, B0, B0, B0, B1, B1, //  1  1  1  0  0  0  1  1
    B1, B1, B1, B0, B0, B1, B0, B0, //  1  1  1  0  0  1  0  0
    B1, B1, B1, B0, B0, B1, B0, B1, //  1  1  1  0  0  1  0  1
    B1, B1, B1, B0, B0, B1, B1, B0, //  1  1  1  0  0  1  1  0
    B1, B1, B1, B0, B0, B1, B1, B1, //  1  1  1  0  0  1  1  1
    B1, B1, B1, B0, B1, B0, B0, B0, //  1  1  1  0  1  0  0  0
    B1, B1, B1, B0, B1, B0, B0, B1, //  1  1  1  0  1  0  0  1
    B1, B1, B1, B0, B1, B0, B1, B0, //  1  1  1  0  1  0  1  0
    B1, B1, B1, B0, B1, B0, B1, B1, //  1  1  1  0  1  0  1  1
    B1, B1, B1, B0, B1, B1, B0, B0, //  1  1  1  0  1  1  0  0
    B1, B1, B1, B0, B1, B1, B0, B1, //  1  1  1  0  1  1  0  1
    B1, B1, B1, B0, B1, B1, B1, B0, //  1  1  1  0  1  1  1  0
    B1, B1, B1, B0, B1, B1, B1, B1, //  1  1  1  0  1  1  1  1

    B1, B1, B1, B1, B0, B0, B0, B0, //  1  1  1  1  0  0  0  0
    B1, B1, B1, B1, B0, B0, B0, B1, //  1  1  1  1  0  0  0  1
    B1, B1, B1, B1, B0, B0, B1, B0, //  1  1  1  1  0  0  1  0
    B1, B1, B1, B1, B0, B0, B1, B1, //  1  1  1  1  0  0  1  1
    B1, B1, B1, B1, B0, B1, B0, B0, //  1  1  1  1  0  1  0  0
    B1, B1, B1, B1, B0, B1, B0, B1, //  1  1  1  1  0  1  0  1
    B1, B1, B1, B1, B0, B1, B1, B0, //  1  1  1  1  0  1  1  0
    B1, B1, B1, B1, B0, B1, B1, B1, //  1  1  1  1  0  1  1  1
    B1, B1, B1, B1, B1, B0, B0, B0, //  1  1  1  1  1  0  0  0
    B1, B1, B1, B1, B1, B0, B0, B1, //  1  1  1  1  1  0  0  1
    B1, B1, B1, B1, B1, B0, B1, B0, //  1  1  1  1  1  0  1  0
    B1, B1, B1, B1, B1, B0, B1, B1, //  1  1  1  1  1  0  1  1
    B1, B1, B1, B1, B1, B1, B0, B0, //  1  1  1  1  1  1  0  0
    B1, B1, B1, B1, B1, B1, B0, B1, //  1  1  1  1  1  1  0  1
    B1, B1, B1, B1, B1, B1, B1, B0, //  1  1  1  1  1  1  1  0
    B1, B1, B1, B1, B1, B1, B1, B1 //  1  1  1  1  1  1  1  1

};

#endif  // BPSK mapping table is not used
//        const Word16 MODTAB_BPSK[4] = {
//
//        //      Real Part,      Imag Part               // B0
//                 P_BPSK_1,      0,                      // 0
//                -P_BPSK_1,      0 };                    // 1
//

//*****************************************************************************
//**********                        QPSK                             **********
//*****************************************************************************

////Q14, Q15
//Word16 CE_ScaleConstel_BPSK_fx = 11585; //23170;  //1/sqrt(2)
//Word16 CE_ScaleConstel_QPSK_fx = 11585; //23170;  //1/sqrt(2)
//Word16 CE_ScaleConstel_16QAM_fx = 5181; //10362; //1/sqrt(10)
//Word16 CE_ScaleConstel_64QAM_fx = 2528; //5056; //1/sqrt(42)

//Word16 CA_Constel_QPSK[8] = {    1,  1,  //00
//                                 1, -1,  //01
//                                -1,  1,  //10
//                                -1, -1}; //11


// SYMBOL SIZE: 2-BIT
// DATA POINTS: +-1
// DATA POINT SCALING: x 1/SQRT(2)

// #define P_QPSK_1  23170           // Point = 1/SQRT/2
#define LTEP0        11585
#define LTEP1       -11585

const short MODTAB_QPSK_256[2048] =
{

// R,  I,  R,  I,  R,  I,  R,  I,              //
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  0  0  0  0  0  0  0  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  0  0  0  0  0  0  0  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  0  0  0  0  0  0  1  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  0  0  0  0  0  0  1  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  0  0  0  0  0  1  0  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  0  0  0  0  0  1  0  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  0  0  0  0  0  1  1  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  0  0  0  0  0  1  1  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  0  0  0  0  1  0  0  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  0  0  0  0  1  0  0  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  0  0  0  0  1  0  1  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  0  0  0  0  1  0  1  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  0  0  0  0  1  1  0  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  0  0  0  0  1  1  0  1
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  0  0  0  0  1  1  1  0
  LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  0  0  0  0  1  1  1  1

  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  0  0  0  1  0  0  0  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  0  0  0  1  0  0  0  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  0  0  0  1  0  0  1  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  0  0  0  1  0  0  1  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  0  0  0  1  0  1  0  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  0  0  0  1  0  1  0  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  0  0  0  1  0  1  1  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  0  0  0  1  0  1  1  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  0  0  0  1  1  0  0  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  0  0  0  1  1  0  0  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  0  0  0  1  1  0  1  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  0  0  0  1  1  0  1  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  0  0  0  1  1  1  0  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  0  0  0  1  1  1  0  1
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  0  0  0  1  1  1  1  0
  LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  0  0  0  1  1  1  1  1

  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  0  0  1  0  0  0  0  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  0  0  1  0  0  0  0  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  0  0  1  0  0  0  1  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  0  0  1  0  0  0  1  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  0  0  1  0  0  1  0  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  0  0  1  0  0  1  0  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  0  0  1  0  0  1  1  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  0  0  1  0  0  1  1  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  0  0  1  0  1  0  0  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  0  0  1  0  1  0  0  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  0  0  1  0  1  0  1  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  0  0  1  0  1  0  1  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  0  0  1  0  1  1  0  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  0  0  1  0  1  1  0  1
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  0  0  1  0  1  1  1  0
  LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  0  0  1  0  1  1  1  1

  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  0  0  1  1  0  0  0  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  0  0  1  1  0  0  0  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  0  0  1  1  0  0  1  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  0  0  1  1  0  0  1  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  0  0  1  1  0  1  0  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  0  0  1  1  0  1  0  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  0  0  1  1  0  1  1  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  0  0  1  1  0  1  1  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  0  0  1  1  1  0  0  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  0  0  1  1  1  0  0  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  0  0  1  1  1  0  1  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  0  0  1  1  1  0  1  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  0  0  1  1  1  1  0  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  0  0  1  1  1  1  0  1
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  0  0  1  1  1  1  1  0
  LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  0  0  1  1  1  1  1  1

  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  0  1  0  0  0  0  0  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  0  1  0  0  0  0  0  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  0  1  0  0  0  0  1  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  0  1  0  0  0  0  1  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  0  1  0  0  0  1  0  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  0  1  0  0  0  1  0  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  0  1  0  0  0  1  1  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  0  1  0  0  0  1  1  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  0  1  0  0  1  0  0  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  0  1  0  0  1  0  0  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  0  1  0  0  1  0  1  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  0  1  0  0  1  0  1  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  0  1  0  0  1  1  0  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  0  1  0  0  1  1  0  1
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  0  1  0  0  1  1  1  0
  LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  0  1  0  0  1  1  1  1

  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  0  1  0  1  0  0  0  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  0  1  0  1  0  0  0  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  0  1  0  1  0  0  1  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  0  1  0  1  0  0  1  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  0  1  0  1  0  1  0  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  0  1  0  1  0  1  0  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  0  1  0  1  0  1  1  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  0  1  0  1  0  1  1  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  0  1  0  1  1  0  0  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  0  1  0  1  1  0  0  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  0  1  0  1  1  0  1  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  0  1  0  1  1  0  1  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  0  1  0  1  1  1  0  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  0  1  0  1  1  1  0  1
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  0  1  0  1  1  1  1  0
  LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  0  1  0  1  1  1  1  1

  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  0  1  1  0  0  0  0  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  0  1  1  0  0  0  0  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  0  1  1  0  0  0  1  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  0  1  1  0  0  0  1  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  0  1  1  0  0  1  0  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  0  1  1  0  0  1  0  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  0  1  1  0  0  1  1  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  0  1  1  0  0  1  1  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  0  1  1  0  1  0  0  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  0  1  1  0  1  0  0  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  0  1  1  0  1  0  1  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  0  1  1  0  1  0  1  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  0  1  1  0  1  1  0  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  0  1  1  0  1  1  0  1
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  0  1  1  0  1  1  1  0
  LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  0  1  1  0  1  1  1  1

  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  0  1  1  1  0  0  0  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  0  1  1  1  0  0  0  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  0  1  1  1  0  0  1  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  0  1  1  1  0  0  1  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  0  1  1  1  0  1  0  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  0  1  1  1  0  1  0  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  0  1  1  1  0  1  1  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  0  1  1  1  0  1  1  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  0  1  1  1  1  0  0  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  0  1  1  1  1  0  0  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  0  1  1  1  1  0  1  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  0  1  1  1  1  0  1  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  0  1  1  1  1  1  0  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  0  1  1  1  1  1  0  1
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  0  1  1  1  1  1  1  0
  LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  0  1  1  1  1  1  1  1

  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  1  0  0  0  0  0  0  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  1  0  0  0  0  0  0  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  1  0  0  0  0  0  1  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  1  0  0  0  0  0  1  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  1  0  0  0  0  1  0  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  1  0  0  0  0  1  0  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  1  0  0  0  0  1  1  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  1  0  0  0  0  1  1  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  1  0  0  0  1  0  0  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  1  0  0  0  1  0  0  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  1  0  0  0  1  0  1  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  1  0  0  0  1  0  1  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  1  0  0  0  1  1  0  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  1  0  0  0  1  1  0  1
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  1  0  0  0  1  1  1  0
  LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  1  0  0  0  1  1  1  1

  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  1  0  0  1  0  0  0  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  1  0  0  1  0  0  0  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  1  0  0  1  0  0  1  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  1  0  0  1  0  0  1  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  1  0  0  1  0  1  0  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  1  0  0  1  0  1  0  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  1  0  0  1  0  1  1  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  1  0  0  1  0  1  1  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  1  0  0  1  1  0  0  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  1  0  0  1  1  0  0  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  1  0  0  1  1  0  1  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  1  0  0  1  1  0  1  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  1  0  0  1  1  1  0  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  1  0  0  1  1  1  0  1
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  1  0  0  1  1  1  1  0
  LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  1  0  0  1  1  1  1  1

  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  1  0  1  0  0  0  0  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  1  0  1  0  0  0  0  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  1  0  1  0  0  0  1  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  1  0  1  0  0  0  1  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  1  0  1  0  0  1  0  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  1  0  1  0  0  1  0  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  1  0  1  0  0  1  1  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  1  0  1  0  0  1  1  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  1  0  1  0  1  0  0  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  1  0  1  0  1  0  0  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  1  0  1  0  1  0  1  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  1  0  1  0  1  0  1  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  1  0  1  0  1  1  0  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  1  0  1  0  1  1  0  1
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  1  0  1  0  1  1  1  0
  LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  1  0  1  0  1  1  1  1

  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  1  0  1  1  0  0  0  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  1  0  1  1  0  0  0  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  1  0  1  1  0  0  1  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  1  0  1  1  0  0  1  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  1  0  1  1  0  1  0  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  1  0  1  1  0  1  0  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  1  0  1  1  0  1  1  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  1  0  1  1  0  1  1  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  1  0  1  1  1  0  0  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  1  0  1  1  1  0  0  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  1  0  1  1  1  0  1  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  1  0  1  1  1  0  1  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  1  0  1  1  1  1  0  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  1  0  1  1  1  1  0  1
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  1  0  1  1  1  1  1  0
  LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  1  0  1  1  1  1  1  1

  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  1  1  0  0  0  0  0  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  1  1  0  0  0  0  0  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  1  1  0  0  0  0  1  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  1  1  0  0  0  0  1  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  1  1  0  0  0  1  0  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  1  1  0  0  0  1  0  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  1  1  0  0  0  1  1  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  1  1  0  0  0  1  1  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  1  1  0  0  1  0  0  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  1  1  0  0  1  0  0  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  1  1  0  0  1  0  1  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  1  1  0  0  1  0  1  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  1  1  0  0  1  1  0  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  1  1  0  0  1  1  0  1
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  1  1  0  0  1  1  1  0
  LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  1  1  0  0  1  1  1  1

  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  1  1  0  1  0  0  0  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  1  1  0  1  0  0  0  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  1  1  0  1  0  0  1  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  1  1  0  1  0  0  1  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  1  1  0  1  0  1  0  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  1  1  0  1  0  1  0  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  1  1  0  1  0  1  1  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  1  1  0  1  0  1  1  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  1  1  0  1  1  0  0  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  1  1  0  1  1  0  0  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  1  1  0  1  1  0  1  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  1  1  0  1  1  0  1  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  1  1  0  1  1  1  0  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  1  1  0  1  1  1  0  1
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  1  1  0  1  1  1  1  0
  LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, //  1  1  0  1  1  1  1  1

  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP0, //  1  1  1  0  0  0  0  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, LTEP1, //  1  1  1  0  0  0  0  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP0, //  1  1  1  0  0  0  1  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, LTEP1, //  1  1  1  0  0  0  1  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP0, //  1  1  1  0  0  1  0  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, LTEP1, //  1  1  1  0  0  1  0  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP0, //  1  1  1  0  0  1  1  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, LTEP1, //  1  1  1  0  0  1  1  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP0, //  1  1  1  0  1  0  0  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, LTEP1, //  1  1  1  0  1  0  0  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP0, //  1  1  1  0  1  0  1  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, LTEP1, //  1  1  1  0  1  0  1  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP0, //  1  1  1  0  1  1  0  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, LTEP1, //  1  1  1  0  1  1  0  1
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP0, //  1  1  1  0  1  1  1  0
  LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, LTEP1, //  1  1  1  0  1  1  1  1

  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP0, //  1  1  1  1  0  0  0  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, LTEP1, //  1  1  1  1  0  0  0  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP0, //  1  1  1  1  0  0  1  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, LTEP1, //  1  1  1  1  0  0  1  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP0, //  1  1  1  1  0  1  0  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, LTEP1, //  1  1  1  1  0  1  0  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP0, //  1  1  1  1  0  1  1  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, LTEP1, //  1  1  1  1  0  1  1  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP0, //  1  1  1  1  1  0  0  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, LTEP1, //  1  1  1  1  1  0  0  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP0, //  1  1  1  1  1  0  1  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, LTEP1, //  1  1  1  1  1  0  1  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP0, //  1  1  1  1  1  1  0  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, LTEP1, //  1  1  1  1  1  1  0  1
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP0, //  1  1  1  1  1  1  1  0
  LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1, LTEP1 //  1  1  1  1  1  1  1  1

    };

//*****************************************************************************
//**********                         16-QAM                          **********
//*****************************************************************************

// SYMBOL SIZE: 4-BIT
// DATA POINTS: +-1, +-3
// DATA POINT SCALING: x 1/SQRT(10)

#define P_LTE16QAM_3       15543
#define P_LTE16QAM_1        5181
#define N_LTE16QAM_1       -5181
#define N_LTE16QAM_3      -15543

/*
 //                          I  Q       b(n),b(n+1),b(n+2),b(n+3)
 Word16 CA_Constel_16QAM[32] = {    1,  1,    //0000
 1,  3,    //0001
 3,  1,    //0010
 3,  3,    //0011
 1, -1,    //0100
 1, -3,    //0101
 3, -1,    //0110
 3, -3,    //0111
 -1,  1,    //1000
 -1,  3,    //1001
 -3,  1,    //1010
 -3,  3,    //1011
 -1, -1,    //1100
 -1, -3,    //1101
 -3, -1,    //1110
 -3, -3};   //1111
 */

const short MODTAB_16QAM_256[1024] =
{

//   Real Part,      Imag Part       Real Part,     Imag Part
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 0000 0000	  1,  1
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 0000 0001	  1,  3
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 0000 0010	  3,  1
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 0000 0011	  3,  3
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 0000 0100	  1, -1
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 0000 0101	  1, -3
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 0000 0110	  3, -1
  P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 0000 0111	  3, -3
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 0000 1000	 -1,  1
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 0000 1001	 -1,  3
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 0000 1010	 -3,  1
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 0000 1011	 -3,  3
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 0000 1100	 -1, -1
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 0000 1101	 -1, -3
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 0000 1110	 -3, -1
  P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 0000 1111	 -3, -3

  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 0001 0000
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 0001 0001
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 0001 0010
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 0001 0011
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 0001 0100
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 0001 0101
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 0001 0110
  P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 0001 0111
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 0001 1000
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 0001 1001
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 0001 1010
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 0001 1011
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 0001 1100
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 0001 1101
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 0001 1110
  P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 0001 1111

  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 0010 0000
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 0010 0001
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 0010 0010
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 0010 0011
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 0010 0100
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 0010 0101
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 0010 0110
  P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 0010 0111
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 0010 1000
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 0010 1001
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 0010 1010
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 0010 1011
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 0010 1100
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 0010 1101
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 0010 1110
  P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 0010 1111

  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 0011 0000
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 0011 0001
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 0011 0010
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 0011 0011
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 0011 0100
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 0011 0101
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 0011 0110
  P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 0011 0111
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 0011 1000
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 0011 1001
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 0011 1010
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 0011 1011
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 0011 1100
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 0011 1101
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 0011 1110
  P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 0011 1111

  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 0100 0000
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 0100 0001
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 0100 0010
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 0100 0011
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 0100 0100
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 0100 0101
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 0100 0110
  P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 0100 0111
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 0100 1000
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 0100 1001
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 0100 1010
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 0100 1011
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 0100 1100
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 0100 1101
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 0100 1110
  P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 0100 1111

  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 0101 0000
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 0101 0001
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 0101 0010
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 0101 0011
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 0101 0100
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 0101 0101
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 0101 0110
  P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 0101 0111
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 0101 1000
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 0101 1001
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 0101 1010
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 0101 1011
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 0101 1100
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 0101 1101
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 0101 1110
  P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 0101 1111

  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 0110 0000
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 0110 0001
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 0110 0010
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 0110 0011
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 0110 0100
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 0110 0101
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 0110 0110
  P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 0110 0111
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 0110 1000
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 0110 1001
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 0110 1010
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 0110 1011
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 0110 1100
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 0110 1101
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 0110 1110
  P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 0110 1111

  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 0111 0000
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 0111 0001
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 0111 0010
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 0111 0011
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 0111 0100
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 0111 0101
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 0111 0110
  P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 0111 0111
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 0111 1000
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 0111 1001
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 0111 1010
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 0111 1011
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 0111 1100
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 0111 1101
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 0111 1110
  P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 0111 1111

  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 1000 0000
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 1000 0001
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 1000 0010
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 1000 0011
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 1000 0100
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 1000 0101
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 1000 0110
  N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 1000 0111
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 1000 1000
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 1000 1001
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 1000 1010
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 1000 1011
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 1000 1100
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 1000 1101
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 1000 1110
  N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 1000 1111

  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 1001 0000
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 1001 0001
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 1001 0010
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 1001 0011
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 1001 0100
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 1001 0101
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 1001 0110
  N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 1001 0111
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 1001 1000
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 1001 1001
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 1001 1010
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 1001 1011
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 1001 1100
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 1001 1101
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 1001 1110
  N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 1001 1111

  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 1010 0000
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 1010 0001
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 1010 0010
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 1010 0011
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 1010 0100
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 1010 0101
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 1010 0110
  N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 1010 0111
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 1010 1000
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 1010 1001
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 1010 1010
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 1010 1011
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 1010 1100
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 1010 1101
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 1010 1110
  N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 1010 1111

  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 1011 0000
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 1011 0001
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 1011 0010
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 1011 0011
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 1011 0100
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 1011 0101
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 1011 0110
  N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 1011 0111
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 1011 1000
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 1011 1001
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 1011 1010
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 1011 1011
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 1011 1100
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 1011 1101
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 1011 1110
  N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 1011 1111

  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 1100 0000
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 1100 0001
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 1100 0010
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 1100 0011
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 1100 0100
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 1100 0101
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 1100 0110
  N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 1100 0111
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 1100 1000
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 1100 1001
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 1100 1010
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 1100 1011
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 1100 1100
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 1100 1101
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 1100 1110
  N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 1100 1111

  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 1101 0000
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 1101 0001
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 1101 0010
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 1101 0011
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 1101 0100
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 1101 0101
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 1101 0110
  N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 1101 0111
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 1101 1000
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 1101 1001
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 1101 1010
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 1101 1011
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 1101 1100
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 1101 1101
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 1101 1110
  N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, // 1101 1111

  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_1, // 1110 0000
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, P_LTE16QAM_3, // 1110 0001
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_1, // 1110 0010
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, P_LTE16QAM_3, // 1110 0011
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_1, // 1110 0100
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, N_LTE16QAM_3, // 1110 0101
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_1, // 1110 0110
  N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, N_LTE16QAM_3, // 1110 0111
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_1, // 1110 1000
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, P_LTE16QAM_3, // 1110 1001
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_1, // 1110 1010
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, P_LTE16QAM_3, // 1110 1011
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_1, // 1110 1100
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, N_LTE16QAM_3, // 1110 1101
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_1, // 1110 1110
  N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, N_LTE16QAM_3, // 1110 1111

  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_1, // 1111 0000
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, P_LTE16QAM_3, // 1111 0001
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_1, // 1111 0010
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, P_LTE16QAM_3, // 1111 0011
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_1, // 1111 0100
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, N_LTE16QAM_3, // 1111 0101
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_1, // 1111 0110
  N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, N_LTE16QAM_3, // 1111 0111
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_1, // 1111 1000
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, P_LTE16QAM_3, // 1111 1001
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_1, // 1111 1010
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, P_LTE16QAM_3, // 1111 1011
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_1, // 1111 1100
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, N_LTE16QAM_3, // 1111 1101
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_1, // 1111 1110
  N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3, N_LTE16QAM_3 // 1111 1111
    };

//*****************************************************************************
//**********                         64-QAM                          **********
//*****************************************************************************

// SYMBOL SIZE: 6-BIT
// DATA POINTS: +-1, +-3, +-5, +-7

// !!!!!!!!!! DATA POINT SCALING IN STANDARD SPECIFIES x 1/SQRT(42) !!!!!!!!!!


#define P_LTE64QAM_7    17696
#define P_LTE64QAM_5    12640
#define P_LTE64QAM_3    7584
#define P_LTE64QAM_1    2528

#define N_LTE64QAM_7   -17696
#define N_LTE64QAM_5   -12640
#define N_LTE64QAM_3   -7584
#define N_LTE64QAM_1   -2528

const short MODTAB_64QAM_64[128] =
{

//  Real Part,    Imag Part       // B0 B1 B2 B3 B4 B5
  P_LTE64QAM_3, P_LTE64QAM_3, //  0  0  0  0  0  0      3,  3,    //000000
  P_LTE64QAM_3, P_LTE64QAM_1, //  0  0  0  0  0  1      3,  1,    //000001
  P_LTE64QAM_1, P_LTE64QAM_3, //  0  0  0  0  1  0      1,  3,    //000010
  P_LTE64QAM_1, P_LTE64QAM_1, //  0  0  0  0  1  1      1,  1,    //000011
  P_LTE64QAM_3, P_LTE64QAM_5, //  0  0  0  1  0  0      3,  5,    //000100
  P_LTE64QAM_3, P_LTE64QAM_7, //  0  0  0  1  0  1      3,  7,    //000101
  P_LTE64QAM_1, P_LTE64QAM_5, //  0  0  0  1  1  0      1,  5,    //000110
  P_LTE64QAM_1, P_LTE64QAM_7, //  0  0  0  1  1  1      1,  7,    //000111

  P_LTE64QAM_5, P_LTE64QAM_3, //  0  0  1  0  0  0      5,  3,    //001000
  P_LTE64QAM_5, P_LTE64QAM_1, //  0  0  1  0  0  1      5,  1,    //001001
  P_LTE64QAM_7, P_LTE64QAM_3, //  0  0  1  0  1  0      7,  3,    //001010
  P_LTE64QAM_7, P_LTE64QAM_1, //  0  0  1  0  1  1      7,  1,    //001011
  P_LTE64QAM_5, P_LTE64QAM_5, //  0  0  1  1  0  0      5,  5,    //001100
  P_LTE64QAM_5, P_LTE64QAM_7, //  0  0  1  1  0  1      5,  7,    //001101
  P_LTE64QAM_7, P_LTE64QAM_5, //  0  0  1  1  1  0      7,  5,    //001110
  P_LTE64QAM_7, P_LTE64QAM_7, //  0  0  1  1  1  1      7,  7,    //001111

  P_LTE64QAM_3, N_LTE64QAM_3, //  0  1  0  0  0  0      3, -3,    //010000
  P_LTE64QAM_3, N_LTE64QAM_1, //  0  1  0  0  0  1      3, -1,    //010001
  P_LTE64QAM_1, N_LTE64QAM_3, //  0  1  0  0  1  0      1, -3,    //010010
  P_LTE64QAM_1, N_LTE64QAM_1, //  0  1  0  0  1  1      1, -1,    //010011
  P_LTE64QAM_3, N_LTE64QAM_5, //  0  1  0  1  0  0      3, -5,    //010100
  P_LTE64QAM_3, N_LTE64QAM_7, //  0  1  0  1  0  1      3, -7,    //010101
  P_LTE64QAM_1, N_LTE64QAM_5, //  0  1  0  1  1  0      1, -5,    //010110
  P_LTE64QAM_1, N_LTE64QAM_7, //  0  1  0  1  1  1      1, -7,    //010111

  P_LTE64QAM_5, N_LTE64QAM_3, //  0  1  1  0  0  0      5, -3,    //011000
  P_LTE64QAM_5, N_LTE64QAM_1, //  0  1  1  0  0  1      5, -1,    //011001
  P_LTE64QAM_7, N_LTE64QAM_3, //  0  1  1  0  1  0      7, -3,    //011010
  P_LTE64QAM_7, N_LTE64QAM_1, //  0  1  1  0  1  1      7, -1,    //011011
  P_LTE64QAM_5, N_LTE64QAM_5, //  0  1  1  1  0  0      5, -5,    //011100
  P_LTE64QAM_5, N_LTE64QAM_7, //  0  1  1  1  0  1      5, -7,    //011101
  P_LTE64QAM_7, N_LTE64QAM_5, //  0  1  1  1  1  0      7, -5,    //011110
  P_LTE64QAM_7, N_LTE64QAM_7, //  0  1  1  1  1  1      7, -7,    //011111

  N_LTE64QAM_3, P_LTE64QAM_3, //  1  0  0  0  0  0     -3,  3,    //100000
  N_LTE64QAM_3, P_LTE64QAM_1, //  1  0  0  0  0  1     -3,  1,    //100001
  N_LTE64QAM_1, P_LTE64QAM_3, //  1  0  0  0  1  0     -1,  3,    //100010
  N_LTE64QAM_1, P_LTE64QAM_1, //  1  0  0  0  1  1     -1,  1,    //100011
  N_LTE64QAM_3, P_LTE64QAM_5, //  1  0  0  1  0  0     -3,  5,    //100100
  N_LTE64QAM_3, P_LTE64QAM_7, //  1  0  0  1  0  1     -3,  7,    //100101
  N_LTE64QAM_1, P_LTE64QAM_5, //  1  0  0  1  1  0     -1,  5,    //100110
  N_LTE64QAM_1, P_LTE64QAM_7, //  1  0  0  1  1  1     -1,  7,    //100111

  N_LTE64QAM_5, P_LTE64QAM_3, //  1  0  1  0  0  0     -5,  3,    //101000
  N_LTE64QAM_5, P_LTE64QAM_1, //  1  0  1  0  0  1     -5,  1,    //101001
  N_LTE64QAM_7, P_LTE64QAM_3, //  1  0  1  0  1  0     -7,  3,    //101010
  N_LTE64QAM_7, P_LTE64QAM_1, //  1  0  1  0  1  1     -7,  1,    //101011
  N_LTE64QAM_5, P_LTE64QAM_5, //  1  0  1  1  0  0     -5,  5,    //101100
  N_LTE64QAM_5, P_LTE64QAM_7, //  1  0  1  1  0  1     -5,  7,    //101101
  N_LTE64QAM_7, P_LTE64QAM_5, //  1  0  1  1  1  0     -7,  5,    //101110
  N_LTE64QAM_7, P_LTE64QAM_7, //  1  0  1  1  1  1     -7,  7,    //101111

  N_LTE64QAM_3, N_LTE64QAM_3, //  1  1  0  0  0  0     -3, -3,    //110000
  N_LTE64QAM_3, N_LTE64QAM_1, //  1  1  0  0  0  1     -3, -1,    //110001
  N_LTE64QAM_1, N_LTE64QAM_3, //  1  1  0  0  1  0     -1, -3,    //110010
  N_LTE64QAM_1, N_LTE64QAM_1, //  1  1  0  0  1  1     -1, -1,    //110011
  N_LTE64QAM_3, N_LTE64QAM_5, //  1  1  0  1  0  0     -3, -5,    //110100
  N_LTE64QAM_3, N_LTE64QAM_7, //  1  1  0  1  0  1     -3, -7,    //110101
  N_LTE64QAM_1, N_LTE64QAM_5, //  1  1  0  1  1  0     -1, -5,    //110110
  N_LTE64QAM_1, N_LTE64QAM_7, //  1  1  0  1  1  1     -1, -7,    //110111

  N_LTE64QAM_5, N_LTE64QAM_3, //  1  1  1  0  0  0     -5, -3,    //111000
  N_LTE64QAM_5, N_LTE64QAM_1, //  1  1  1  0  0  1     -5, -1,    //111001
  N_LTE64QAM_7, N_LTE64QAM_3, //  1  1  1  0  1  0     -7, -3,    //111010
  N_LTE64QAM_7, N_LTE64QAM_1, //  1  1  1  0  1  1     -7, -1,    //111011
  N_LTE64QAM_5, N_LTE64QAM_5, //  1  1  1  1  0  0     -5, -5,    //111100
  N_LTE64QAM_5, N_LTE64QAM_7, //  1  1  1  1  0  1     -5, -7,    //111101
  N_LTE64QAM_7, N_LTE64QAM_5, //  1  1  1  1  1  0     -7, -5,    //111110
  N_LTE64QAM_7, N_LTE64QAM_7 //  1  1  1  1  1  1     -7, -7,    //111111
    };

