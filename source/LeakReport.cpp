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

/*! \file LeakReport.cpp
	\author	Danny Angelo Carminati Grein
	\brief Leak Report
*/

#if defined(DEBUG)

#include "LeakReport.h"
#include "Defines.h"
#include "Log.h"

#include <map>

#define TAG "[LeakReport] "

namespace Seed {

LeakReport LeakReport::instance;
LeakReport *const pLeakReport = &LeakReport::instance;

LeakReport::LeakReport()
{
}

LeakReport::~LeakReport()
{
}

bool LeakReport::LogNew(void *ptr, const char *func, int line)
{
	char buf[1024];
	memset(buf, '\0', 1024);
	snprintf(buf, 1024, "%s:%d", func, line);

	mapAddress[ptr] = strdup(buf);

	return false;
}

void LeakReport::LogDelete(void *ptr)
{/*
	if (ptr)
	{
		char *str = mapAddress[ptr];

		if (str)
		{
			free(str);
		}

		mapAddress[ptr] = NULL;

		PointerMapIterator it = mapAddress.find(ptr);
		mapAddress.erase(it);
	}*/
}

void LeakReport::Print()
{
	Log(TAG "LeakReport: %d leaks.", mapAddress.size());

	PointerMapIterator it = mapAddress.begin();
	PointerMapIterator end = mapAddress.end();

	for (; it != end; ++it)
	{
		Log(TAG "\t[0x%8x] %s", (*it).first, (*it).second);
	}
}

}; // namespace

#endif // DEBUG
