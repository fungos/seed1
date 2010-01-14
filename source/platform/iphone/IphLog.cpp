/*! \file IphLog.cpp
	\author	Danny Angelo Carminati Grein
	\brief Log Iphone Implementation
*/

#ifdef _IPHONE_

#include "Defines.h"
#include "Log.h"
#include <stdio.h>

#include <UIKit/UIKit.h>


void Info(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	fprintf(stdout, "%s\n", t);
}

#ifdef DEBUG

#define HALT	do {} while (1);


/*
FIXME: 15-10-2008 | BUG | MINOR | CORE | Log va_list can't parse %lld correcly. | Danny Angelo Carminati Grein

Log("%lld", (u64)1); != printf("%lld", (u64)1);

*/
void Log(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	fprintf(stdout, "%s\n", t);
}

void Dbg(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	fprintf(stdout, "%s\n", t);
}

/*
void DebugAssert(const char *pMessage, ...)
{
	char t[2048];
	va_list ap;

	va_start(ap, pMessage);
	vsnprintf(t, 2048, pMessage, ap);
	va_end(ap);

	fprintf(stdout, "%s\n", t);

	DebugHalt();
}

void DebugHalt()
{
	NSLog(@"HALT!");
	HALT;
}
*/
#endif // DEBUG

#endif // _IPHONE_
