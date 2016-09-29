//////////////////////////////////////////////////////////////////////////////////
// @file    wih_oamInterface.h
// @brief Class definition for OAM Interface @@ Singleton
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 21-09-2010
// Change History : 21-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __OAM_INTERFACE__
#define __OAM_INTERFACE__

#include "wih.h"

/////////////////// Class Definition /////////////////

class COamInterface 
{
	private:
		//Attributes
		static COamInterface *mInstance;

		// Methods
		void processPrmHelloMsg(Msg *);
		void processSetLogConfig(Msg *msgIn);
		void processModuleExitReq();
		void processInactTimerMsg();

	protected:
		COamInterface();
		virtual ~COamInterface();

	public:
		void wihProcessRecvIpcMsg(Msg *msgIn);
		static COamInterface & getInstance(void);

};


#endif // __OAM_INTERFACE__

