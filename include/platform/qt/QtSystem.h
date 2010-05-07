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

/*! \file QtSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System QT Implementation
*/

#ifndef __QT_SYSTEM_H__
#define __QT_SYSTEM_H__

#if defined(_QT_)

#include <QWidget>
#include "interface/ISystem.h"
#include "Singleton.h"

namespace Seed { namespace QT {

class System : public ISystem
{
	SEED_SINGLETON_DECLARE(System);
	public:
		// ISystem
		virtual void GoToMenu();
		virtual void Sleep();
		virtual void OnHomeCalled();
		virtual void WaitForRetrace(ISystem::eFrameRate rate);
		virtual void SetFrameRate(ISystem::eFrameRate rate);
		virtual void HangUp();
		virtual void GoToDataManager();

		virtual ISystem::eFrameRate GetFrameRate();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;
		virtual BOOL IsSleeping() const;

		virtual void DisableHome();
		virtual void EnableHome();
		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;
		virtual BOOL InitializeHome();

		virtual void WriteOut(const char *msg);
		virtual void WriteErr(const char *msg);
		virtual void SetWidget(QWidget *widget);

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	private:
		SEED_DISABLE_COPY(System);

	private:
		u32 		iRetraceCount;
		eFrameRate 	iFrameRate;
		BOOL 		bShutdown;
		f32 		fElapsedTime;
		u64 		iLastFrameTime;
		u64 		iFpsTime;

		QWidget		*pWidget;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(System);
}

}} // namespace

#else // _QT_
	#error "Include 'System.h' instead 'platform/qt/QtSystem.h' directly."
#endif // _QT_
#endif // __QT_SYSTEM_H__
