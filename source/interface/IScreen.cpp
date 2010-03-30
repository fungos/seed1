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

/*! \file IScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Screen class interface
*/

#include "interface/IScreen.h"
#include "Log.h"

namespace Seed {

IScreen::IScreen()
	: bFading(FALSE)
	, iMode(0)
	, pRenderer(NULL)
	, fAspectRatio(static_cast<f32>(PLATFORM_MAX_SCREEN_HEIGHT)/static_cast<f32>(PLATFORM_MAX_SCREEN_WIDTH))
	, iHeight(PLATFORM_MAX_SCREEN_HEIGHT)
	, iWidth(PLATFORM_MAX_SCREEN_WIDTH)
{
}

IScreen::~IScreen()
{
}

INLINE BOOL IScreen::IsFading() const
{
	return this->bFading;
}

INLINE void IScreen::SetMode(u32 mode)
{
	this->iMode = mode;
}

INLINE u32 IScreen::GetMode() const
{
	return this->iMode;
}

INLINE void IScreen::ToggleFullscreen()
{
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL IScreen::HasWindowedMode() const
{
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE BOOL IScreen::IsFullscreen() const
{
	SEED_ABSTRACT_METHOD;
	return TRUE;
}

INLINE void IScreen::SetRenderer(IRenderer *renderer)
{
	this->pRenderer = renderer;
}

INLINE u32 IScreen::GetHeight() const
{
	return this->iHeight;
}

INLINE u32 IScreen::GetWidth() const
{
	return this->iWidth;
}

INLINE f32 IScreen::GetAspectRatio() const
{
	return this->fAspectRatio;
}

INLINE BOOL IScreen::IsRequired() const
{
	return TRUE;
}

INLINE const char *IScreen::GetObjectName() const
{
	return "Screen";
}

} // namespace

