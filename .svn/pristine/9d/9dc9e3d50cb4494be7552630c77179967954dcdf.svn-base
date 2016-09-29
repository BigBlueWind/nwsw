/* 
 * File:   tr_nh.h
 * Author: Raghavendra 
 *
 * Created on 26 June 2012, 13:06
 */

#define LOG_NhuRbId(rbId, rbTyp)\
{\
     LOG_ELEM ( "RBID ", rbId, \
                          TR_ELEM_CONT);\
   \
     LOG_ELEM ( "RBType", rbTyp, \
                          TR_ELEM_LAST);\
}

#define LOG_NhuRachCfg(buf, ptr, flag)\
{\
   BeginStruct("NhuRachCfg()", buf);\
\
   LOG_ELEM("maxMsg3Tx", \
                   ptr->maxMsg3Tx, \
                   flag);\
   EndStruct(buf);\
}

#define LOG_SuId(buf, val, string)\
{\
   LOG_ELEM(string, \
                   val, \
                   TR_ELEM_CONT);\
}

#ifdef RRC_PHY_CFG_SUPP

#define LOG_NhuTxSchmCfg(buf, ptr, flag)\
{\
   BeginStruct("NhuTxSchmCfg", buf);\
\
   LOG_ELEM("scSpacing ", \
                   ptr->scSpacing, \
                   flag);\
\
   EndStruct(buf);\
}
#endif 

#define LOG_NhuPrachCfg(buf, ptr, flag)\
{\
   BeginStruct("NhuPrachCfg", buf);\
\
   LOG_ELEM("rootSequenceIndex", \
                   ptr->rootSequenceIndex, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("prachCfgIndex", \
                   ptr->prachCfgIndex, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("zeroCorrelationZoneCfg", \
                   ptr->zeroCorrelationZoneCfg, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("highSpeedFlag", \
                   ptr->highSpeedFlag, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("prachFreqOffset", \
                   ptr->prachFreqOffset, \
                   TR_ELEM_CONT);\
   EndStruct(buf);\
}

#define LOG_NhuCmnPdschCfg(buf, ptr, flag)\
{\
   BeginStruct("NhuCmnPdschCfg", buf);\
\
   LOG_ELEM("refSigPower",\
                   ptr->refSigPower,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("pb", \
                   ptr->pb,\
                   TR_ELEM_LAST);\
    EndStruct(buf);\
}

#define LOG_NhuCmnPuschCfg(buf, ptr, flag)\
{\
   BeginStruct("NhuCmnPuschCfg", buf);\
\
   LOG_ELEM("nSB",\
                   ptr->nSB,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("hoppingMode",\
                   ptr->hoppingMode,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("puschHoppingOffset",\
                   ptr->puschHoppingOffset,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("enable64QAM",\
                   ptr->enable64QAM,\
                   TR_ELEM_CONT);\
\
  LOG_NhuUlRefSignPUSCH(buf, (&ptr->refSig), flag);\
\
   EndStruct(buf);\
}

#define LOG_NhuUlRefSignPUSCH(buf, ptr, flag)\
{\
\
   BeginStruct("NhuUlRefSigPUSCH", buf);\
\
   LOG_ELEM("grpHoppngEnb",\
                   ptr->grpHoppngEnb,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("grpAssignPUSCH",\
                   ptr->grpAssignPUSCH,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("seqHoppingEnb",\
                   ptr->seqHoppingEnb,\
                   TR_ELEM_CONT);\
\
   LOG_ELEM("cyclicShift",\
                   ptr->cyclicShift,\
                   TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_NhuPhichCfg(buf, ptr, flag)\
{\
\
   BeginStruct("NhuPhichCfg", buf);\
\
   LOG_ELEM("resrc", \
                   ptr->resrc, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("duration", \
                   ptr->duration, \
                   TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_NhuPucchCfg(buf, ptr, flag)\
{\
\
   BeginStruct("NhuPhucchCfg", buf);\
\
   LOG_ELEM("pucchFixBw", \
                   ptr->pucchFixBw, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("n1PucchAn", \
                   ptr->n1PucchAn, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("deltaShift", \
                   ptr->deltaShift, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("cyclShift", \
                   ptr->cyclShift, \
                   TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_NhuTddSfCfgInfo(ptr, buf, flag)\
{\
   BeginStruct("NhuTddSfCfgInfo", buf);\
\
   LOG_ELEM("sfAssignment", \
                   ptr->sfAssignment, \
                   TR_ELEM_CONT);\
\
   LOG_ELEM("spclSfPatterns", \
                   ptr->spclSfPatterns, \
                   TR_ELEM_LAST);\
\
   EndStruct(buf);\
} 
