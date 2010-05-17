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

/*! \file WiiSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System init, VBlank and Interupt functionality
*/

#ifndef __WII_SYSTEM_H__
#define __WII_SYSTEM_H__

#if defined(_WII_)

#include "interface/ISystem.h"
#include "Singleton.h"

namespace Seed { namespace WII {

class System : public ISystem
{
	SEED_SINGLETON_DECLARE(System);
	public:
		virtual void OnHomeCalled();
		virtual void GoToMenu();
		virtual void GoToDataManager();
		virtual void HangUp();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;

		virtual void Sleep();
		virtual BOOL IsSleeping() const;

		virtual void WaitForRetrace(ISystem::eFrameRate rate);
		virtual void SetFrameRate(ISystem::eFrameRate rate);
		virtual ISystem::eFrameRate GetFrameRate();

		virtual void DisableHome();
		virtual void EnableHome();

		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;

		virtual BOOL InitializeHome();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IUpdatable
		virtual BOOL Update(f32 delta);

	private:
		SEED_DISABLE_COPY(System);

		static void System::ResetCallback();
		static void System::PowerCallback();

	private:
		u32 retraceCount;
		eFrameRate frameRate;
		static BOOL g_Reset;
		static BOOL g_Power;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(System);
}

}} // namespace

#else // _WII_
	#error "Include 'System.h' instead 'platform/wii/WiiSystem.h' directly."
#endif // _WII_
#endif // __WII_SYSTEM_H__
