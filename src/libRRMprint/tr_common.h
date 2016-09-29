

#define LOG_CmLteCellId(cellId, buf, flag)\
{\
      LOG_ELEM("CellId", \
                               cellId, \
                               flag);\
}

#define LOG_CmLteLcId(lcId, buf, flag)\
{\
      LOG_ELEM("LcId", \
                               lcId, \
                               flag);\
}

#define LOG_CmLteLcType(lcType, buf, flag)\
{\
      LOG_ELEM("LcType", \
                               lcType, \
                               flag);\
}

#define LOG_CmLteTimingInfo(time, buf)\
{\
  BeginStruct("CmLteTimingInfo", buf);\
\
  LOG_ELEM("SFN", time.sfn, TR_ELEM_CONT); \
\
  LOG_ELEM("Subframe", time.subframe, TR_ELEM_LAST);\
\
  EndStruct(buf);\
}

#define LOG_CmLteRnti(ueId, buf, flag)\
{\
   LOG_ELEM("ueId", ueId, flag);\
\
}

#define LOG_CmLteUeCategory(var, buf, flag)\
{\
  LOG_ELEM("ueCategory", var, flag); \
}

#define LOG_Inst(Inst, buf, flag)\
{\
   LOG_ELEM("Inst", \
                   Inst,\
                   flag);\
}

