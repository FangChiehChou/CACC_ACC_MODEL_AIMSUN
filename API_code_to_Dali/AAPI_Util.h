//-*-Mode: C++;-*-
#ifndef _AAPI_Util_h_
#define _AAPI_Util_h_
//SDK
#ifdef _WIN32
	#define AAPIEXPORT __declspec(dllimport)

	#pragma warning(disable: 4251)
	#pragma warning(disable: 4786)
	#pragma warning(disable: 4503)
#else
	#define AAPIEXPORT 
#endif

#endif
