//-------------------------------------------------------------------------------------------
/** @file debug.h
 *
 * @brief Defines variables required for testing and debugging
 * @author Mindspeed Technologies
 * @version $Revision: 1.112 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef CEVA_INTRINSICS
#ifdef _WIN32
//DEBUG_PRINT: Enables all debug file outputs.

//#define DEBUG_PRINT 1  //<-- manually enable DEBUG_PRINT, normally
//disabled for Release build.   Already enabled for Debug build.
//
/////////////////////////////////////////////////////////////////
//#define for PUSCH and PUCCH Fixed Point logging of data, set to 1 if you want to log
/////////////////////////////////////////////////////////////////
#define LOG_BS_FFTIN_FX                 0
#define LOG_BS_FFTOUT_FX                0
#define LOG_BS_ADET_FX                  0
#define LOG_BS_ULPILOTS_PUSCH_FX        0
#define LOG_BS_ULPILOTS_PUCCH_FX        0
#define LOG_BS_CHANESTIN_FX             0
#define LOG_BS_CHANEST_STAGE1_FX        0
#define LOG_BS_TIMING_FX                0
#define LOG_BS_DEROTATION_FX            0
#define LOG_BS_CHANEST_STAGE2_FX        0
#define LOG_BS_BEFORE_NOISEAVG_FX       0
#define LOG_BS_NOISEAVG_P1_FX           0
#define LOG_BS_NOISEAVG_P2_FX           0
#define LOG_BS_NOISEAVG_P3_FX           0
#define LOG_BS_NOISEAVG_P4_FX           0
#define LOG_BS_SNR_CALC_FX              0
#define LOG_BS_SRS_SNR_PERRB_FX         0
#define LOG_BS_CHANESTP1_PUSCH_FX       0
#define LOG_BS_CHANESTP2_PUSCH_FX       0
#define LOG_BS_CHANESTP3_PUSCH_FX       0
#define LOG_BS_CHANESTP4_PUSCH_FX       0
#define LOG_BS_CHANEST_PUCCH_P0_FX      0
#define LOG_BS_CHANEST_PUCCH_FX         0
#define LOG_BS_IDFTIN_FX                0
#define LOG_BS_MRCOUT_FX                0
#define LOG_BS_MRC_CHANPOW_FX           0
#define LOG_BS_DEMAPPERIN_PUSCH_FX      0
#define LOG_BS_DEMAPPEROUT_PUSCH_FX     0
#define LOG_BS_DEMAPPERIN_PUCCH_FX      0
#define LOG_BS_DEMAPPEROUT_PUCCH_FX     0

/////////////////////////////////////////////////////////////////
//#define for PUSCH and PUCCH Floating Point logging of data, set to 1 if you want to log
/////////////////////////////////////////////////////////////////
#define LOG_BS_FFTIN_FL                 0
#define LOG_BS_FFTOUT_FL                0
#define LOG_BS_ADET_FL                  0
#define LOG_BS_ULPILOTS_PUSCH_FL        0
#define LOG_BS_ULPILOTS_PUCCH_FL        0
#define LOG_BS_CHANESTIN_FL             0
#define LOG_BS_CHANEST_STAGE1_FL        0
#define LOG_BS_TIMING_FL                0
#define LOG_BS_DEROTATION_FL            0
#define LOG_BS_FREQUENCY_FL             0
#define LOG_BS_CHANEST_STAGE2_FL        0
#define LOG_BS_BEFORE_NOISEAVG_FL       0
#define LOG_BS_NOISEAVG_P1_FL           0
#define LOG_BS_NOISEAVG_P2_FL           0
#define LOG_BS_NOISEAVG_P3_FL           0
#define LOG_BS_NOISEAVG_P4_FL           0
#define LOG_BS_SNR_CALC_FL              0
#define LOG_BS_SRS_SNR_FL               0
#define LOG_BS_SRS_SNR_PERRB_FL         0
#define LOG_BS_CHANESTP0_PUSCH_FL       0
#define LOG_BS_CHANESTP1_PUSCH_FL       0
#define LOG_BS_CHANESTP2_PUSCH_FL       0
#define LOG_BS_CHANESTP3_PUSCH_FL       0
#define LOG_BS_CHANESTP4_PUSCH_FL       0
#define LOG_BS_CHANESTP5_PUSCH_FL       0
#define LOG_BS_CHANEST_PUCCH_FL         0
#define LOG_BS_IDFTIN_FL                0
#define LOG_BS_SNR_PERRB_FL             0
#define LOG_BS_SNR_FL                   0
#define LOG_BS_MMSE_NOISEVAR_AVG_FL     0
#define LOG_BS_MRCOUT_FL                0
#define LOG_BS_MRC_CHANPOW_FL           0
#define LOG_BS_MRCOUT_PUCCH_FL          0
#define LOG_BS_MRC_CHANPOW_PUCCH_FL     0
#define LOG_BS_DEMAPPERIN_PUSCH_FL      0
#define LOG_BS_DEMAPPEROUT_PUSCH_FL     0
#define LOG_BS_DEMAPPERIN_PUCCH_FL      0
#define LOG_BS_DEMAPPEROUT_PUCCH_FL     0
#endif
#endif

#ifdef DEBUG_PRINT
extern short debugExpo[14][4][10];
extern short numDebugExpo;
#endif

/******************************************************************************

 This include file defines variables required for testing and debugging.

 ******************************************************************************/
#define         MAX_OUTPUT_BLK_SIZE 100800 // Tracing
extern FILE *ff_tst[10];
extern FILE *f_data1; /* Decision output  */
extern FILE *f_data_codeword0;
extern FILE *f_data_codeword1;
extern FILE *f_data_feca0;
extern FILE *f_data_feca1;
extern FILE *f_data_fecb0;
extern FILE *f_data_fecb1;
extern FILE *f_data_fecc0;
extern FILE *f_data_fecc1;
extern FILE *f_data_fecd0;
extern FILE *f_data_fecd1;
extern FILE *f_data_fecf0;
extern FILE *f_data_fecf1;
extern FILE *f_data_mod0_b0;
extern FILE *f_data_mod1_b_0;
extern FILE *f_data_cfi_mod0_b0;
extern FILE *f_data_cfi_mod1_b_0;
extern FILE *f_data_ctl_mod0_b0;
extern FILE *f_data_ctl_mod1_b_0;
extern FILE *f_data_mod2_d0;
extern FILE *f_data_mod0_b1;
extern FILE *f_data_mod1_b_1;
extern FILE *f_data_mod2_d1;
extern FILE *f_data_mod3_x[4];
extern FILE *f_data_mod4_y[4];
extern FILE *f_data_k_l;
extern FILE *f_data_k_l_dec;
extern FILE *f_data_mod5_unscaled_a[4];
extern FILE *f_data_mod5a[4];
extern FILE *f_data_imod5a[4];
extern FILE *f_data_mod5c_ctl_a[4];
extern FILE *f_data_mod5d_bcst_a[4];
extern FILE *f_data_mod5f_a[4];
extern FILE *f_data_mod_bfps_a[4];
extern FILE *f_data_mod5e_sync_a;
extern FILE *f_data_mod6_s;
extern FILE *f_data_i16_mod6_s;

//DL/UL RX
extern FILE *f_data_demod5a_Nfft_a[4];
extern FILE *f_data_demod5_a[4];
extern FILE *f_data_demod_data_b_all;
extern FILE *f_data_demod_data_b_0;
extern FILE *f_data_demod4_z[4];
extern FILE *f_data_demod1_u0_b;

extern short deci[MAX_OUTPUT_BLK_SIZE];

////////////////////////////////////////////////////
//Uplink Rx debug files used to for comparing
//fixed point and floating point
////////////////////////////////////////////////////
#ifdef _WIN32
extern FILE *fFFT_In;
extern FILE *fFFT_Out;
extern FILE *fTcbIOBuf;
extern FILE *fAdet;
extern FILE *fUl_Pilots;
extern FILE *fUl_PilotsPucch;
extern FILE *fChanEst_pucch;
extern FILE *fChanEst_p0;
extern FILE *fChanEst_p1;
extern FILE *fChanEst_p2;
extern FILE *fChanEst_p3;
extern FILE *fChanEst_p4;
extern FILE *fChanEst_p5;
extern FILE *fMmseNoiseVarAvg;
extern FILE *fChanEstIn;
extern FILE *fChanEstStage1;
extern FILE *fChanEstStage2;
extern FILE *fTiming;
extern FILE *fDerotation;
extern FILE *fFrequency;
extern FILE *fChanEst_BeforeNoiseAvg;
extern FILE *fNoiseAvg_p1;
extern FILE *fNoiseAvg_p2;
extern FILE *fNoiseAvg_p3;
extern FILE *fNoiseAvg_p4;
extern FILE *fSnrPerRb;
extern FILE *fSnr;
extern FILE *fSnr_Calc;
extern FILE *fSrs_Snr_PerRB;
extern FILE *fMrc_Out;
extern FILE *fMrc_ChanPower;
extern FILE *fMrc_Out_pucch;
extern FILE *fMrc_ChanPower_pucch;
extern FILE *fIdft_In;
extern FILE *fRxMapperIn;
extern FILE *fRxMapperOut;
extern FILE *fRxMapperIn_pucch;
extern FILE *fRxMapperOut_pucch;

extern FILE *fecout;
extern FILE *fCtrlOutput;
extern FILE *f_bits_out;
extern FILE *fBsSinr;
extern FILE *fBsFreqOffset;

extern FILE *fTxFecInput;
extern FILE *fTxFecOutput;
extern FILE *fTxiFFTInput;
extern FILE *fScramblerOut;
extern FILE *fTxMapperIn;
extern FILE *fTxMapperOut;
#endif



// APITHREAD
#define PID_APITHREAD_CASE                              1000
#define PID_APITHREAD_NO_API                            1001
#define PID_APITHREAD_LOG_AND_FREE                      1002
#define PID_APITHREAD_COMPLETE                          1003
#define PID_APITHREAD_FRACTIONAL_TIMER                  1004
#define PID_APITHREAD_PHY_INIT                          1005
#define PID_APITHREAD_PHY_START                         1006
#define PID_APITHREAD_PHY_STOP                          1007
#define PID_APITHREAD_PHY_PAUSE                         1008
#define PID_APITHREAD_PHY_RESUME                        1009
#define PID_APITHREAD_PHY_SHUTDOWN                      1010
#define PID_APITHREAD_PHY_RECONFIG                      1011
#define PID_APITHREAD_PHY_LOGS_ENABLE                   1012
#define PID_APITHREAD_START_PBCH_CEVA                   1013
#define PID_APITHREAD_CLEANUP_CTX0                      1014
#define PID_APITHREAD_CLEANUP_CTX1                      1015
#define PID_APITHREAD_CLEANUP_CTX2                      1016
#define PID_APITHREAD_CLEANUP_CTX3                      1017
#define PID_APITHREAD_PHYSTATS_CTX0                     1018
#define PID_APITHREAD_PHYSTATS_CTX1                     1019
#define PID_APITHREAD_PHYSTATS_CTX2                     1020
#define PID_APITHREAD_PHYSTATS_CTX3                     1021


// PHYSTATS
#define PID_PHYSTATS_STREAM                             1030
#define PID_PHYSTATS_CLEAR                              1031

// TXSDU
#define PID_TXSDU_BCH                                   1040
#define PID_TXSDU_BCH_SCRAMBLER_DONE                    1041
#define PID_TXSDU_CONTROL                               1042
#define PID_TXSDU_HARQ                                  1043
#define PID_TXSDU_DCI0                                  1044
#define PID_TXSDU_DISPATCH_DATASDU                      1045
#define PID_TXSDU_DATA_CRC                              1046
#define PID_TXSDU_DATA_FEC_BUILD                        1047
#define PID_TXSDU_DATA_FEC                              1048
#define PID_TXSDU_DATA_SCRAMBLER                        1049
#define PID_TXSDU_DATA_COMPLETE                         1050

// TXLIST
#define PID_TXVECTORPROC_CONTROL                        1060
#define PID_TXVECTORPROC_DATA                           1061
#define PID_TXLIST_UPDATE_DL_CONTROL_TASKLIST           1062
#define PID_TXLIST_UNLOCK_SETUP_BUFS                    1063
#define PID_TXLIST_IFFT_CALLBACK                        1064
#define PID_TXLIST_IFFT_UPDATE_DESC                     1065
#define PID_TXLIST_MISSED_RUN                           1066
#define PID_TXLIST_RUN_LIST                             1067
#define PID_TXLIST_COMPLETE_LIST                        1068


// API ERROR CHECK
#define PID_API_ERRORCHECK_USE_EMPTY_CHANNELS           1070
#define PID_API_ERRORCHECK_COMPLETE                     1071
#define PID_API_PREPARE_CRC_INP                         1072
#define PID_API_START_PDSCH_CRC                         1073
#define PID_API_ERRORCHECK_CB                           1074
#define PID_API_ERRORCHECK_SUCCESSIVE_API_MISSING       1075


// RXLIST
#define PID_RXVECTORPROC                                1100
#define PID_RXLIST_NUM_CONT_SYMS_PUSCH_M                1101
#define PID_RXLIST_MUX_CONTROL_FINDPOS                  1102
#define PID_RXLIST_GROUP_HOP                            1103
#define PID_RXLIST_PRSBITS                              1104
#define PID_RXLIST_UPDATE_TASKLIST                      1105
#define PID_RXLIST_PREPROC                              1106
#define PID_RXLIST_GEN_IDFT_TABLES                      1107
#define PID_RXLIST_IDFT_QUEUE                           1108
#define PID_RXLIST_FFT_CALLBACK                         1109
#define PID_RXLIST_FFT_UPDATE_DESC                      1110
#define PID_RXLIST_IDFT_FFT_UPDATE_DESC_IN_CEVA         1111
#define PID_RXLIST_MISSED_RUN                           1112
#define PID_RXLIST_RUN_LIST                             1113
#define PID_RXLIST_POSTPROCESS                          1114
#define PID_RXLIST_COMPLETE_LIST                        1115
#define PID_WAIT_RXVECTORPROC_COMPLETE                  1116

// PRACH LIST
#define PID_PRACHLIST_CALC_AND_SEND                     1120
#define PID_PRACHLIST_MISSED_RUN                        1121
#define PID_PRACHLIST_RUN_LIST                          1122
#define PID_PRACHLIST_COMPLETE_LIST                     1123

// SRS THREAD
#define PID_SRSLIST_CALC_AND_SEND                       1130
#define PID_SRSLIST_MISSED_RUN                          1131
#define PID_SRSLIST_COMPLETE_LIST                       1132
#define PID_SRSLIST_RUN_LIST                            1133

// LTE MSG MGR
#define PID_MSGMGR_PRACH_BUILD_MSG                      1140
#define PID_MSGMGR_PRACH_SEND_MSG                       1141
#define PID_MSGMGR_SRS_BUILD_MSG                        1142
#define PID_MSGMGR_SRS_SEND_MSG                         1143
#define PID_MSGMGR_RXEND_BUILD_MSG                      1144
#define PID_MSGMGR_RXEND_ADD_MSG                        1145
#define PID_MSGMGR_RXSDU_BUILD_MSG                      1146
#define PID_MSGMGR_RXSTATUS_BUILD_MSG                   1147
#define PID_MSGMGR_DELAYED_API_RESPONSE_BUILD_MSG       1148
#define PID_MSGMGR_SEND_LIST                            1149
#define PID_MSGMGR_OTHER_BUILD_MSG                      1150
#define PID_MSGMGR_OTHER_SEND_MSG                       1151

// GENERAL
#define PID_SCHEDULE_PHY_THREAD                         1160
#define PID_API_SEND_TO_LINUX_MAC                       1161
#define PID_API_SEND_TO_4GMX_MAC                        1162
#define PID_PHYDI_TX_CALLBACK                           1163
#define PID_PHYDI_RX_CALLBACK                           1164
#define PID_PHYDI_DLIQ_MDMA_COPY                        1165
#define PID_PHYDI_ULIQ_MDMA_COPY                        1166
#define PID_PHYDI_COPYIQ_TIMER_MODE                     1167
#define PID_PHYDI_DISTOP                                1168
#define PID_APILIST_SEND_TO_LINUX_MAC                       1171
#define PID_APILIST_SEND_TO_4GMX_MAC                        1172
#define PID_NMMAPI_SEND_TO_LINUX_MAC                       1181
#define PID_NMMAPI_SEND_TO_4GMX_MAC                        1182


// SUPERVISOR
#define PID_SVSR_RX_API_HANDLER                         1200
#define PID_SVSR_RX_API_HANDLER_MSG                     1201

// PUSH / POP
#define PID_APITHREAD_PUSH_BASE                                1250
#define PID_APITHREAD_POP_BASE                                 1260
#define PID_APITHREAD_POP_NO_FIFO_BASE                         1270
#define PID_APITHREAD_POP_SILENT_BASE                          1280

#define PID_APITHREAD_PUSH_MAC_TO_PHY_API                      1250
#define PID_APITHREAD_PUSH_PHY_LOG_AND_CLEAN_API               1251
#define PID_APITHREAD_PUSH_PHY_DELAYED_API_RESPONSE            1252

#define PID_APITHREAD_POP_MAC_TO_PHY_API                       1260
#define PID_APITHREAD_POP_PHY_LOG_AND_CLEAN_API                1261
#define PID_APITHREAD_POP_PHY_DELAYED_API_RESPONSE             1262

#define PID_APITHREAD_POP_NO_FIFO_MAC_TO_PHY_API               1270
#define PID_APITHREAD_POP_NO_FIFO_PHY_LOG_AND_CLEAN_API        1271
#define PID_APITHREAD_POP_NO_FIFO_PHY_DELAYED_API_RESPONSE     1272

#define PID_APITHREAD_POP_SILENT_MAC_TO_PHY_API                1280
#define PID_APITHREAD_POP_SILENT_PHY_LOG_AND_CLEAN_API         1281
#define PID_APITHREAD_POP_SILENT_PHY_DELAYED_API_RESPONSE      1282

// RXSDU CTX 0
#define PID_RXSDU_CTX0_DATA_DESCRAMBLER_SEQ                    1300
#define PID_RXSDU_CTX0_DATA_FEC                                1301
#define PID_RXSDU_CTX0_DATA_DISPATCH_CRC                       1302
#define PID_RXSDU_CTX0_DATA_CRC                                1303
#define PID_RXSDU_CTX0_DATA_UPDATE_TA_CQI                      1304
#define PID_RXSDU_CTX0_DATA_SEND_LIST                          1305
#define PID_RXSDU_CTX0_MUX_PUSCH_CQI_SEND_LIST                 1306
#define PID_RXSDU_CTX0_MUX_PUSCH_ACKNACK_SEND_LIST             1307
#define PID_RXSDU_CTX0_CONTROL_SEND_LIST                       1308
#define PID_RXSDU_CTX0_CONTROL_UPDATE_TA_CQI                   1309
#define PID_RXSDU_CTX0_UPDATE_TA_CQI                           1310
#define PID_RXSDU_CTX0_DATA_DISPATCH_FEC                       1311
#define PID_RXSDU_CTX0_DATA_MISSED_DISPATCH_FEC                1312
#define PID_RXSDU_CTX0_PUCCH_SDU_CLEANUP                       1313
#define PID_RXSDU_CTX0_PUSCH_SDU_CLEANUP                       1314
#define PID_RXSDU_CTX0_MUX_PUSCH_SDU_CLEANUP                   1315
#define PID_RXSDU_CTX0_PREPROC_PART1                           1316
#define PID_RXSDU_CTX0_PREPROC_PART2                           1317
#define PID_RXSDU_CTX0_PREPROC_PART3                           1318
#define PID_RXSDU_CTX0_SEND_TO_MAC_PUSCH                       1319
#define PID_RXSDU_CTX0_SEND_TO_MAC_PUCCH                       1320
#define PID_RXSDU_CTX0_SEND_TO_MAC_OTHER                       1321


// RXSDU CTX 1
#define PID_RXSDU_CTX1_DATA_DESCRAMBLER_SEQ                    1330
#define PID_RXSDU_CTX1_DATA_FEC                                1331
#define PID_RXSDU_CTX1_DATA_DISPATCH_CRC                       1332
#define PID_RXSDU_CTX1_DATA_CRC                                1333
#define PID_RXSDU_CTX1_DATA_UPDATE_TA_CQI                      1334
#define PID_RXSDU_CTX1_DATA_SEND_LIST                          1335
#define PID_RXSDU_CTX1_MUX_PUSCH_CQI_SEND_LIST                 1336
#define PID_RXSDU_CTX1_MUX_PUSCH_ACKNACK_SEND_LIST             1337
#define PID_RXSDU_CTX1_CONTROL_SEND_LIST                       1338
#define PID_RXSDU_CTX1_CONTROL_UPDATE_TA_CQI                   1339
#define PID_RXSDU_CTX1_UPDATE_TA_CQI                           1340
#define PID_RXSDU_CTX1_DATA_DISPATCH_FEC                       1341
#define PID_RXSDU_CTX1_DATA_MISSED_DISPATCH_FEC                1342
#define PID_RXSDU_CTX1_PUCCH_SDU_CLEANUP                       1343
#define PID_RXSDU_CTX1_PUSCH_SDU_CLEANUP                       1344
#define PID_RXSDU_CTX1_MUX_PUSCH_SDU_CLEANUP                   1345
#define PID_RXSDU_CTX1_PREPROC_PART1                           1346
#define PID_RXSDU_CTX1_PREPROC_PART2                           1347
#define PID_RXSDU_CTX1_PREPROC_PART3                           1348
#define PID_RXSDU_CTX1_SEND_TO_MAC_PUSCH                       1349
#define PID_RXSDU_CTX1_SEND_TO_MAC_PUCCH                       1350
#define PID_RXSDU_CTX1_SEND_TO_MAC_OTHER                       1351

// RXSDU CTX 2
#define PID_RXSDU_CTX2_DATA_DESCRAMBLER_SEQ                    1360
#define PID_RXSDU_CTX2_DATA_FEC                                1361
#define PID_RXSDU_CTX2_DATA_DISPATCH_CRC                       1362
#define PID_RXSDU_CTX2_DATA_CRC                                1363
#define PID_RXSDU_CTX2_DATA_UPDATE_TA_CQI                      1364
#define PID_RXSDU_CTX2_DATA_SEND_LIST                          1365
#define PID_RXSDU_CTX2_MUX_PUSCH_CQI_SEND_LIST                 1366
#define PID_RXSDU_CTX2_MUX_PUSCH_ACKNACK_SEND_LIST             1367
#define PID_RXSDU_CTX2_CONTROL_SEND_LIST                       1368
#define PID_RXSDU_CTX2_CONTROL_UPDATE_TA_CQI                   1369
#define PID_RXSDU_CTX2_UPDATE_TA_CQI                           1370
#define PID_RXSDU_CTX2_DATA_DISPATCH_FEC                       1371
#define PID_RXSDU_CTX2_DATA_MISSED_DISPATCH_FEC                1372
#define PID_RXSDU_CTX2_PUCCH_SDU_CLEANUP                       1373
#define PID_RXSDU_CTX2_PUSCH_SDU_CLEANUP                       1374
#define PID_RXSDU_CTX2_MUX_PUSCH_SDU_CLEANUP                   1375
#define PID_RXSDU_CTX2_PREPROC_PART1                           1376
#define PID_RXSDU_CTX2_PREPROC_PART2                           1377
#define PID_RXSDU_CTX2_PREPROC_PART3                           1378
#define PID_RXSDU_CTX2_SEND_TO_MAC_PUSCH                       1379
#define PID_RXSDU_CTX2_SEND_TO_MAC_PUCCH                       1380
#define PID_RXSDU_CTX2_SEND_TO_MAC_OTHER                       1381

// RXSDU CTX 3
#define PID_RXSDU_CTX3_DATA_DESCRAMBLER_SEQ                    1390
#define PID_RXSDU_CTX3_DATA_FEC                                1391
#define PID_RXSDU_CTX3_DATA_DISPATCH_CRC                       1392
#define PID_RXSDU_CTX3_DATA_CRC                                1393
#define PID_RXSDU_CTX3_DATA_UPDATE_TA_CQI                      1394
#define PID_RXSDU_CTX3_DATA_SEND_LIST                          1395
#define PID_RXSDU_CTX3_MUX_PUSCH_CQI_SEND_LIST                 1396
#define PID_RXSDU_CTX3_MUX_PUSCH_ACKNACK_SEND_LIST             1397
#define PID_RXSDU_CTX3_CONTROL_SEND_LIST                       1398
#define PID_RXSDU_CTX3_CONTROL_UPDATE_TA_CQI                   1399
#define PID_RXSDU_CTX3_UPDATE_TA_CQI                           1400
#define PID_RXSDU_CTX3_DATA_DISPATCH_FEC                       1401
#define PID_RXSDU_CTX3_DATA_MISSED_DISPATCH_FEC                1402
#define PID_RXSDU_CTX3_PUCCH_SDU_CLEANUP                       1403
#define PID_RXSDU_CTX3_PUSCH_SDU_CLEANUP                       1404
#define PID_RXSDU_CTX3_MUX_PUSCH_SDU_CLEANUP                   1405
#define PID_RXSDU_CTX3_PREPROC_PART1                           1406
#define PID_RXSDU_CTX3_PREPROC_PART2                           1407
#define PID_RXSDU_CTX3_PREPROC_PART3                           1408
#define PID_RXSDU_CTX3_SEND_TO_MAC_PUSCH                       1409
#define PID_RXSDU_CTX3_SEND_TO_MAC_PUCCH                       1410
#define PID_RXSDU_CTX3_SEND_TO_MAC_OTHER                       1411

// RX MEASUREMENTS
#define PID_RXMEAS_CTX0_RIP                                    1450
#define PID_RXMEAS_CTX0_THERMAL_NOISE                          1451
#define PID_RXMEAS_CTX0_MEASUREMENTS                           1452

#define PID_RXMEAS_CTX1_RIP                                    1460
#define PID_RXMEAS_CTX1_THERMAL_NOISE                          1461
#define PID_RXMEAS_CTX1_MEASUREMENTS                           1462

#define PID_RXMEAS_CTX2_RIP                                    1470
#define PID_RXMEAS_CTX2_THERMAL_NOISE                          1471
#define PID_RXMEAS_CTX2_MEASUREMENTS                           1472

#define PID_RXMEAS_CTX3_RIP                                    1480
#define PID_RXMEAS_CTX3_THERMAL_NOISE                          1481
#define PID_RXMEAS_CTX3_MEASUREMENTS                           1482


// API PLAYER
#define PID_API_PLAYER_RXSDU_IND                        2000
#define PID_API_PLAYER_RXSTATUS_IND                     2001
#define PID_API_PLAYER_SEND_MESSAGE_TO_PHY              2002

// NMM
#define PID_NMM_MAIN_GLOBAL                             3000
#define PID_NMM_MAIN_PRE_TASKS                          3001
#define PID_NMM_MAIN_POST_TASKS                         3002
#define PID_NMM_SYNC_SEARCH_GLOBAL                      3003
#define PID_NMM_MIB_GLOBAL                              3004
#define PID_NMM_MIB_PBCH_EXTRACT                        3005
#define PID_NMM_MIB_PBCH_DECODE                         3006
#define PID_NMM_SIB1_GLOBAL                             3007
#define PID_NMM_SIB1_PCFI                               3008
#define PID_NMM_SIB1_PHICH_IDX                          3009
#define PID_NMM_SIB1_PDCCH_IDX                          3010
#define PID_NMM_SIB1_PDCCH_EXTRACT                      3011
#define PID_NMM_SIB1_PDCCH_DECODE                       3012
#define PID_NMM_SIB1_DCI                                3013
#define PID_NMM_SIB1_PDSCH_EXTRACT                      3014
#define PID_NMM_SIB1_PDSCH_DECODE                       3015
#define PID_NMM_API_THREAD                              3016


// Logging Harware Clock register values
#define PID_CEVA_CLK                10000
#define PID_MAP_CLK                 10001
#define PID_ARM_CLK                 10002
#define PID_FECUL_CLK               10003
#define PID_FECDL_CLK               10004
#define PID_AXI_CLK                 10005
#define PID_MDMA_CLK                10006

// Logging Task List Execution Time
#define PID_TASKLIST_EXEC_00         20000
#define PID_TSKLS_PUSCH_CXT_0        20000
#define PID_TSKLS_PUSCH_CXT_1        20001
#define PID_TSKLS_PDSCH_CXT_0        20002
#define PID_TSKLS_PDSCH_CXT_1        20003
#define PID_TSKLS_PRACH_CXT_0        20004
#define PID_TSKLS_PRACH_CXT_1        20005
#define PID_TSKLS_PRACH_CXT_2        20006
#define PID_TSKLS_PRACH_CXT_3        20007
#define PID_TSKLS_SRS_CXT_0          20008
#define PID_TSKLS_SRS_CXT_1          20009
#define PID_TSKLS_UL_MUX_CTRL_CXT_0  20010
#define PID_TSKLS_UL_MUX_CTRL_CXT_1  20011
#define PID_TSKLS_UL_MUX_CTRL_CXT_2  20012
#define PID_TSKLS_UL_MUX_CTRL_CXT_3  20013
#define PID_TSKLS_DL_SET_CTX_0       20014
#define PID_TSKLS_DL_SET_CTX_1       20015
#define PID_TSKLS_EXEC_NMM_TASKLIST0 20016
#define PID_TSKLS_EXEC_NMM_TASKLIST1 20017
#define PID_TSKLS_EXEC_NMM_TASKLIST2 20018
#define PID_TSKLS_EXEC_NMM_TASKLIST3 20019
#define PID_TSKLS_EXEC_NMM_TASKLIST4 20020









/* The below range from 30001 - 30099 is for 4GMX SSI and common */
#define PID_SVSRALLOC_ICMSG           30001
#define PID_SVSRSEND_ICMSG            30002
#define PID_SVSRALLOC_MSG             30003
#define PID_SSI_CORE3                 30004
#define PID_SSI_CORE2                 30005
#define PID_SSI_ICORE_MSG             30006
#define PID_SSI_ICPU_MSG              30007


/* The below range from 30101 - 30199 is for CL */
#define PID_CRC_IND_DUMMY             30101
#define PID_CRC_IND_REAL              30102
#define PID_CL_CNTRL_REQ              30103
#define PID_CL_DAT_REQ                30104
#define PID_CL_RECP_REQ               30105
#define PID_CL_SEND_TO_PHY            30106
#define PID_CL_RXEND_IND              30107
#define PID_CL_PHY_MSG                30108
#define PID_CL_RXSDU_IND              30109
#define PID_CL_RXSDUANDEND_IND        30110
#define PID_CL_FILL_DLCHNL_DESC       30111
#define PID_CL_FILL_TXSDU             30112
#define PID_CL_SND_VECT_TO_PHY        30113
#define PID_CL_PHICH_DAT_REQ          30114
#define PID_CL_PDCCH_DAT_REQ          30115
#define PID_CL_PDSCH_DAT_REQ          30116
#define PID_CL_PBCH_DAT_REQ           30117


/* The below range from 30200 - 30299 is for MAC/RLC/PDCP */
#define PID_SCH_TTI_IND               30200
#define PID_MAC_DAT_IND               30201
#define PID_MAC_SF_ALLOC_REQ          30202
#define PID_MAC_STA_RSP               30203
#define PID_MAC_DL_SCHD               30204
#define PID_MAC_DL_CQI_IND            30205
#define PID_MAC_UL_CQI_IND            30206
#define PID_MAC_SR_IND                30207
#define PID_MAC_HARQ_IND              30208
#define PID_MAC_TA_IND                30209
#define PID_MAC_UL_SCHD               30210
#define PID_MAC_TTI_IND               30211
#define PID_MAC_UE_CFG                30212
#define PID_MAC_UE_RECFG              30213
#define PID_MAC_UE_DEL                30214
#define PID_MAC_UE_RST                30215
#define PID_SCH_UE_CFG                30216
#define PID_SCH_UE_RECFG              30217
#define PID_SCH_UE_DEL                30218
#define PID_SCH_UE_RST                30219
#define PID_MAC_DDATREQ               30220
#define PID_MAC_FILLDREQ              30221
#define PID_MAC_TFUDREQ               30222
#define PID_SCHUTL_CMALLCEVT          30223
#define PID_TOMUTL_CMALLCEVT          30224
#define PID_TOMINF_CMALLCEVT          30225
#define PID_MACUTL_CMALLCEVT          30226

#define PID_RLC_STA_IND               30240
#define PID_RLC_REASSEMBLY            30241
#define PID_RLC_AM_SELF_PST_DL        30242
#define PID_RLC_AM_PDU_RLS            30243
#define PID_RLC_CHK_SDU_MAP           30244
#define PID_RLC_SND_DAT_CFM           30245
#define PID_RLC_AM_SELF_PDU_RSMBL     30246
#define PID_RLC_AM_RGU_DATIND         30247
#define PID_RLC_AM_ADDTOLIST_UL       30248
#define PID_RLC_AM_FREEBUF            30249
#define PID_RLC_AM_RETXLIST           30250
#define PID_RLC_AM_POST_ULSELFMSG     30251
#define PID_RLC_AM_UTL_RSMBL          30252
#define PID_RLC_AM_STA_PDU            30253
#define PID_RLC_CFG_REQ               30254
#define PID_RLC_CFG_REQ_REEST         30255
#define PID_RLC_CFG_REQ_REM           30256

#define PID_MT_AM_SVSR_HDLR           30008
#define PID_MT_AM_SVS_MSG             30009
#define PID_MAC_AM_HARQ_RLS           30010
#define PID_RLC_KWUDAT_IND            30012
#define PID_RLC_STA_IND_MEMSET        30014
#define PID_RLC_STA_IND_PRC           30015
#define PID_PDCP_DL_PKT               30280
#define PID_PDCP_DAT_IND              30281
#define PID_PDCP_CFG_REQ              30282
#define PID_PDCP_CFG_REQ_REEST        30283
#define PID_PDCP_CFG_REQ_REM          30284

/* The below range is for detailed analysis of common functions */
#define PID_SAddPstMsgMult    30600
#define PID_SAddPreMsgMult    30601
#define PID_SRemPreMsgMult    30602
#define PID_SRemPstMsgMult    30603
#define PID_SGetDataFrmMsg    30604
#define PID_SSegMsg           30605
#define PID_SCpyFixMsg        30607
#define PID_SCpyMsgFix        30608
#define PID_SCpyMsgMsg        30610
#define PID_SAddMsgRef        30609
#define PID_SCatMsg           30611
#define PID_SCompressMsg      30612
#define PID_SGetPstMsgMult    30613
#define PID_SSwapMsg          30614
#define PID_SCpyPartMsg       30615
#define PID_SRepPartMsg       30616
#define PID_SPkMsgMult        30617
#define PID_SMovPartMsg       30618

/* This captures all the processing invoked through PHY */
#define PID_PHY_RCV_PROC              30999

#ifndef _WIN32
#define GET_TICKS                                    GetTick()
#else
#define GET_TICKS                                    0
#define MLogTask(a, b, c, d) (0)
#define MLogTaskREX(a, b, c, d) (0)
#endif  //_WIN32

#endif /* _DEBUG_H */
