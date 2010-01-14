/*! \file Log.h
	\author	Danny Angelo Carminati Grein
	\brief Debug out, assert and halt functionality
*/


#ifndef __LOG_H__
#define __LOG_H__


#include "Defines.h"


extern void Info(const char *pMessage, ...);

#ifdef DEBUG
	extern void Log(const char *pMessage, ...);
	extern void Dbg(const char *pMessage, ...);
#else
	#if defined(__GNUC__) || defined(_WII_)
		#define Log(...)
		#define Dbg(...)
	#else
		#define Log
		#define Dbg
	#endif // __GNUC__ || _WII_
#endif // DEBUG


#endif // __LOG_H__
