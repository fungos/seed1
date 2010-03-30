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

/*! \file WiiHomeButton.h
	\author	Rafael Eduardo gonchor
	\brief Wii Home Button Manager
*/

#ifndef __WII_HOMEBUTTON_H__
#define __WII_HOMEBUTTON_H__

#if defined(_WII_)

#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "File.h"

namespace Seed { namespace WII {

class HomeButton : public IModule, public IUpdatable
{
	public:
		virtual ~HomeButton() {};

		//IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		//IUpdatable
		virtual BOOL Update(f32 delta);
		virtual BOOL Render(f32 delta);

		void Start();

		inline void Stop()
		{
			bFinished = FALSE;
			bRunning  = FALSE;
		}

		inline BOOL IsRunning()
		{
			return bRunning;
		}

		inline BOOL IsFinished()
		{
			return bFinished;
		}

		inline void Disable()
		{
			bDisabled = TRUE;
		}

		inline void Enable()
		{
			bDisabled = FALSE;
		}

		inline virtual BOOL IsEnabled() const
		{
			return !bDisabled;
		}

	public:
		static HomeButton instance;

	private:
		SEED_DISABLE_COPY(HomeButton);

		void InitControllerData();
		void InitHomeButtonInfo();
		void SetAdjustValue();
		void InitSound();
		void SetProjection();
		void LoadBanIcon();
		void DrawBanIcon(u8 alpha);

		static int SoundCallback(int evt, int arg);

	private:
		BOOL 				bRunning;
		BOOL 				bFadeCalled;
		BOOL				bFinished;
		BOOL				bDisabled;
		WiiDataInfo			m_hbmInfo;
		File				stFileHBMInfoLayout;
		File				stFileHBMSpkSe;
		File				stFileHBMBanIcon;
		File				stFileHBMMsg;
		File				stFileHBMCfg;
		File				stFileHBMSoundData;
		WiiControllerData	m_conData;
		WiiRenderModeObj	pRenderMode;

		BOOL				m_banIconSwitch;
		s8					m_banIconMode;
		WiiTick				m_banIconTime;
		u8					m_banIconAlpha;
		WiiPalettePtr		sIconTpl;
		WiiTexObj			texObj;

		enum
		{
			eAlphaInIcon = 0,
			ePauseIcon,
			eAlphaOutIcon
		};

		/* Allocate a buffer for sound */
		u8 *pSoundBuffer;

		/* Read destination for sound data from an optical disc */
		u8 *pSoundData;
};

HomeButton *const pHBM = &HomeButton::instance;

}} // namespace

#else // _WII_
	#error "Cannot include 'WiiHomeButton.h' in a non-Wii platform."
#endif // _WII_
#endif // __WII_HOMEBUTTON_H__
