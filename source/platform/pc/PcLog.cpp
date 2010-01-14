/*! \file PcLog.cpp
	\author	Danny Angelo Carminati Grein
	\brief Log PC Implementation
*/

#include "Log.h"

#ifdef _PC_

#include <stdio.h>
#include "SeedInit.h"

void Info(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	Seed::Private::pApplication->WriteOut(t);
}

#ifdef DEBUG

void Log(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	Seed::Private::pApplication->WriteOut(t);
}

void Dbg(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	Seed::Private::pApplication->WriteDbg(t);
}

#endif // DEBUG

#endif // _PC_
