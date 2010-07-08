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

/*! \file IScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Screen class interface
*/

#ifndef __ISCREEN_H__
#define __ISCREEN_H__

#include "IModule.h"
#include "Enum.h"

namespace Seed {

/// Screen Interface
/**
Any Screen implementation must implement this interface.
*/
class SEED_CORE_API IScreen : public IModule
{
	public:
		IScreen();
		virtual ~IScreen();

		/// Gets the current video mode
		virtual eVideoMode GetMode() const;

		/// Change the current video mode
		virtual void SetMode(eVideoMode mode);

		/// Start fade out
		virtual void FadeOut();

		/// Start fade in
		virtual void FadeIn();

		/// Get the screen heigth
		virtual u32 GetHeight() const;

		/// Get the screen width
		virtual u32 GetWidth() const;

		/// Get the aspect ratio
		virtual f32 GetAspectRatio() const;

		/// Verify if a fade is being done.
		virtual BOOL IsFading() const;

		/// Change between fullscreen and windowed for platforms that supports it
		virtual void ToggleFullscreen();

		/// Check if the platform support windowed video mode
		virtual BOOL HasWindowedMode() const;

		/// Check if it is fullscreen
		virtual BOOL IsFullscreen() const;

		virtual void Update();

		// IModule
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		BOOL		bFading;
		eVideoMode	nMode;
		f32			fAspectRatio;
		u32			iHeight;
		u32			iWidth;

	private:
		SEED_DISABLE_COPY(IScreen);
};

} // namespace

#endif // __ISCREEN_H__
