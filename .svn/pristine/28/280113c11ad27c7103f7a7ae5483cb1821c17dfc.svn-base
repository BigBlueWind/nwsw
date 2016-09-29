/* 
 * File:   tr_ssi.h
 * Author: avenugop
 *
 * Created on 26 June 2012, 12:11
 */

#ifndef TR_SSI_H
#define	TR_SSI_H

#ifdef	__cplusplus
extern "C" {
#endif

/* BEGIN: REMOVE after including in a common header file*/
//except for LOG_ELEM, all other arguments should be pointers (&elem)

#define TR_ELEM_CONT "|"
#define TR_ELEM_LAST ""
#define TR_MSG_END   "\n"

//These MACROS will be defined later
#define logMsg(_arg) //empty
#define LOG_ELEM(_arg1,_arg2,_arg3)
#define LOG_POINTER(_arg1,_arg2,_arg3,_arg4) //_arg2 is pointer type
#define LOG_ENUM(_arg1,_arg2,_arg3)
#define BEGIN_STRUCT(_arg1,_arg2)
#define END_STRUCT(_arg1)
#define BEGIN_UNION(_arg1,_arg2)
#define END_UNION(_arg1)
#define BEGIN_LIST(_arg1)
#define END_LIST()
#define BEGIN_ARRAY(_arg1,_arg2)\
do\
{ S32 iloop=0;

#define END_ARRAY(_arg1,_arg2)\ //_arg1 is array index, _arg2 is terminator('|')
}while(++iloop < _arg1);\
LOG_ELEM("","",_arg2); //only terminator here


/* END: REMOVE after including in a common header file*/

/* typedefs */

#define LOG_STATUS(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_TICKS(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_MSGLEN(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_ORDER(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_SIZE(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_PTROFF(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_QLEN(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_BUFQLEN(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_REGSIZE(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_DPOOLSIZE(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_RANDOM(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_SEQ(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_COREID(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_ERRCLS(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_ERRCODE(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_ERRVAL(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_VECTNMB(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_TTYPE(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)

#define LOG_SEMA(_arg1,_arg2,_arg3) LOG_ELEM(_arg1,*(_arg2),_arg3)


#ifdef	__cplusplus
}
#endif

#endif	/* TR_SSI_H */

