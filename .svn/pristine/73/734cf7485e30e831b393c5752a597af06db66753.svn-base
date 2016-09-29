#ifndef __TS_H__
#define __TS_H__

#ifdef PWAV_TS
extern U32             gSf_b1;
extern U32             gSf_b2;

extern U32             gRtt_a;
extern U32             gRtt_b;

typedef struct s_uehTfuSetTsReq{
  U32       a1;
} SetTsReq;

typedef struct s_tfuUehSetTsRsp{
  U32        a1;
  U32        b1;
} SetTsRsp;

typedef struct s_uehTfuSetTsCfm{
  U32        b1;
  U32        a2;
  U32        rtt_a;
} SetTsCfm;

S16 uehSndCtfSetTsReq(void);
S16 uehSndCtfSetTsCfm(SetTsRsp *setTsRsp);
S16 YsUiUehSetTsRsp(SetTsReq *setTsReq);
S16 UehLiCtfSetTsRsp(Pst *pst, SetTsRsp *setTsRsp);
S16 YsUiUehSetTsReq(Pst *pst, SetTsReq *setTsReq);
S16 YsUiUehSetTsCfm(Pst *pst, SetTsCfm *setTsCfm);
S16 cmPkAndSndSetTsReq ( Pst *pst, SetTsReq *setTsReq);
S16 cmPkAndSendSetTsCfm ( Pst *pst, SetTsCfm *setTsCfm);

/* Set Time stamp Request from UEH to CL. */
typedef S16 (*CtfSetTsReq) ARGS(( Pst *pst, SetTsReq *setTsReq));
/* Set Time stamp respones from CL to UEH. */
typedef S16 (*CtfSetTsRsp) ARGS(( Pst *pst, SetTsRsp *setTsRsp));
/* Set Time stamp confirm from UEH to CL. */
typedef S16 (*CtfSetTsCfm) ARGS(( Pst *pst, SetTsCfm *setTsCfm));
#endif
#endif /* __TS_H__ */
