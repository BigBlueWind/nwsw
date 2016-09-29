//////////////////////////////////////////////////////////////////////////////////
// @file    wih_wsInterface.h
// @brief Class definition for Web Server Interface @@ Singleton
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __WS_INTERFACE__
#define __WS_INTERFACE__

/////////////////// Class Definition /////////////////

class CWebInterface 
{
	private:
		//Attributes
		static CWebInterface *mInstance;

		// Methods

	protected:
		CWebInterface();
		virtual ~CWebInterface();

	public:
		void acceptNewRequest(void);
		static CWebInterface & getInstance(void);

};


#endif // __WS_INTERFACE__

