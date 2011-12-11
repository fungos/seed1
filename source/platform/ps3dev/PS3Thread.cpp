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

/*! \file PS3Thread.cpp
	\author	Danny Angelo Carminati Grein
	\brief ps3dev thread implementation
*/

#if defined(_PS3DEV_)

#include "Thread.h"
#include "MemoryManager.h"

#define TAG 	"[Thread] "

#define THREAD_STACK_SIZE		(16 * 1024)
#define THREAD_PRIORITY			1000

namespace Seed { namespace PS3 {

static void __seed_thread_loop_callback(void *param)
{
	Thread *pt = static_cast<Thread *>(param);
	while (pt->Run());

	pMemoryManager->DisableThreadCache();

	pt->Destroy();
}

Thread::Thread()
	: bRunning(TRUE)
	, cThread(0)
{
}

Thread::~Thread()
{
	this->Destroy();
}

INLINE void Thread::Create(s32 priority)
{
	UNUSED(priority);

	bRunning = TRUE;
	if (!cThread)
	{
		s32 ret = sysThreadCreate(&cThread, __seed_thread_loop_callback, this, THREAD_PRIORITY, THREAD_STACK_SIZE, THREAD_JOINABLE, "thread");
		ASSERT_MSG(ret == 0, TAG "Failed to create thread.");
	}
}

INLINE void Thread::Destroy()
{
	bRunning = FALSE;

	u64 retval = 0;

	if (cThread)
		sysThreadJoin(cThread, &retval);

	cThread = 0;
}

INLINE BOOL Thread::Run()
{
	return bRunning;
}

}} // namespace

#endif // _PS3DEV_
