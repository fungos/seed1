/*! \file IScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Screen class interface
*/


#include "interface/IScreen.h"


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
}

INLINE BOOL IScreen::HasWindowedMode() const
{
	return FALSE;
}

INLINE BOOL IScreen::IsFullscreen() const
{
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

