/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file LeakReport.h
	\author	Danny Angelo Carminati Grein
	\brief Leak Report
*/

#ifndef __LEAK_REPORT_H__
#define __LEAK_REPORT_H__

#if defined(DEBUG)

#include "Config.h"
#include "Defines.h"
#include "Log.h"
#include "Singleton.h"
#include "MemoryManager.h"

#include <map>

#define SEED_LEAK_MAX			2048

#define New(T)					pLeakReport->LogNew((new T), #T, __FILE__, __LINE__, __FUNC__)
#define Delete(ptr)				pLeakReport->LogDelete(ptr);

#define LeakReportPrint			pLeakReport->Print();

namespace Seed {

class LeakReport
{
	SEED_SINGLETON_DECLARE(LeakReport);

	public:
		template <class T>
		T *LogNew(T* ptr, const char *call, const char *file, int line, const char *func)
		{
			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (arInfo[i].ptrAddr == NULL)
				{
					arInfo[i].ptrAddr = ptr;

					strncpy(arInfo[i].strCall, call, 64);
					strncpy(arInfo[i].strFile, file, 128);
					strncpy(arInfo[i].strFunc, func, 256);

					arInfo[i].iLine = line;

					//Log("New: [0x%8x] %s:%d: %s -> %s", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					break;
				}
			}

			return ptr;
		}

		template <class T>
		void LogDelete(T *ptr)
		{
			void *addr = (void *)ptr;

			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (addr == arInfo[i].ptrAddr)
				{
					//Log("Delete: [0x%8x] %s:%d: %s -> %s", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					arInfo[i].ptrAddr = NULL;

					memset(arInfo[i].strCall, '\0', 64);
					memset(arInfo[i].strFile, '\0', 128);
					memset(arInfo[i].strFunc, '\0', 256);

					arInfo[i].iLine = 0;
					break;
				}
			}

			if (ptr)
			{
				delete ptr;
				ptr = NULL;
			}
		}

		void Print();

	private:
		SEED_DISABLE_COPY(LeakReport);

		struct PointerInfo
		{
			void *ptrAddr;
			char strCall[64];
			char strFile[128];
			char strFunc[256];
			int  iLine;
		};

		PointerInfo arInfo[SEED_LEAK_MAX];
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(LeakReport);
}

}; // namespace

#else

#define New(T)					new T
#define Delete(ptr)				if (ptr) \
									delete ptr; \
								ptr = NULL;

#define LeakReportPrint

#endif // DEBUG

#endif // __LEAK_REPORT_H__
