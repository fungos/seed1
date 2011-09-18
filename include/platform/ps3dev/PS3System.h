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

/*! \file PS3System.h
	\author	Danny Angelo Carminati Grein
	\brief ps3dev system implementation
*/

#ifndef __PS3DEV_SYSTEM_H__
#define __PS3DEV_SYSTEM_H__

#if defined(_PS3DEV_)

#include "interface/ISystem.h"
#include "Singleton.h"

namespace Seed { namespace PS3 {

class SEED_CORE_API System : public ISystem
{
	SEED_SINGLETON_DECLARE(System)
	public:
		// ISystem
		virtual void GoToMenu();
		virtual void Sleep();
		virtual void OnHomeCalled();
		virtual void WaitForRetrace(eSystemFrameRate rate);
		virtual void HangUp();
		virtual void GoToDataManager();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;
		virtual BOOL IsSleeping() const;

		virtual void DisableHome();
		virtual void EnableHome();
		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;
		virtual BOOL InitializeHome();

		virtual void EnableDefaultCursor(BOOL b);

		virtual const FilePath *GetUsername() const;
		virtual const FilePath *GetHomeFolder() const;
		virtual const FilePath *GetApplicationDataFolder() const;
		virtual const FilePath *GetSaveGameFolder() const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IUpdatable
		virtual BOOL Update(f32 dt);

	private:
		SEED_DISABLE_COPY(System);

	private:
		u32 		iRetraceCount;
		eSystemFrameRate iFrameRate;
		BOOL 		bShutdown;
		BOOL		bSleeping;
		BOOL		bDefaultCursorEnabled;
		f32 		fElapsedTime;
		u64 		iLastFrameTime;
		u64 		iFpsTime;
};

#define pSystem PS3::System::GetInstance()

}} // namespace

#else // _PS3DEV_
	#error "Include 'System.h' instead 'platform/ps3dev/PS3System.h' directly."
#endif // _PS3DEV_
#endif // __PS3DEV_SYSTEM_H__
