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
		static HomeButton instance;


	public:
		virtual ~HomeButton() {};

		//IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		//IUpdatable
		virtual BOOL Update();
		virtual BOOL Render();

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
		WiiControllerData		m_conData;
		WiiRenderModeObj		pRenderMode;

		BOOL     			m_banIconSwitch;
		s8      			m_banIconMode;
		WiiTick  				m_banIconTime;
		u8      			m_banIconAlpha;
		WiiPalettePtr 			sIconTpl;
		WiiTexObj 				texObj;


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


	private:
		void InitControllerData();
		void InitHomeButtonInfo();
		void SetAdjustValue();
		void InitSound();
		void SetProjection();
		void LoadBanIcon();
		void DrawBanIcon(u8 alpha);

		static int SoundCallback(int evt, int arg);
};


HomeButton *const pHBM = &HomeButton::instance;


}} // namespace


#else // _WII_

	#error "Cannot include 'WiiHomeButton.h' in a non-Wii platform."

#endif // _WII_
#endif // __WII_HOMEBUTTON_H__
