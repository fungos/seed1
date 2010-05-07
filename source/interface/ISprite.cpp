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

/*! \file ISprite.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sprite class interface
*/

#include "Defines.h"
#include "interface/ISprite.h"
#include "SpriteObject.h"
#include "Screen.h"
#include "Log.h"
#include "Enum.h"
#include "StringCache.h"

#define TAG "[ISprite] "

namespace Seed {

ISprite::ISprite()
	: IBasicMesh()
	, pSprite(NULL)
	, pAnimation(NULL)
	, pAnimationFrames(NULL)
	, pFrame(NULL)
	, pFrameImage(NULL)
	, bInitialized(FALSE)
	, bChanged(FALSE)
	, bAnimation(FALSE)
	, bLoop(FALSE)
	, bPlaying(FALSE)
	, fAspectHalfWidth(0.0f)
	, fAspectHalfHeight(0.0f)
	, fAspectWidth(0.0f)
	, fAspectHeight(0.0f)
	, iCurrentAnimation(0)
	, iCurrentFrame(0)
	, iFrames(0)
	, fCurrentFrameRate(0.0f)
	, fFrameTime(0.0f)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, pRes(NULL)
	, pPool(NULL)
	, pFilename(NULL)
{
	iNumVertices = 4;
}

ISprite::~ISprite()
{
	this->Unload();
}

INLINE void ISprite::Reset()
{
	sRelease(pFrameImage);
	sRelease(pSprite);

	pAnimation		= NULL;
	pFrame			= NULL;
	pAnimationFrames = NULL;
	bInitialized	= FALSE;
	bChanged 		= FALSE;
	bAnimation		= FALSE;
	bLoop			= FALSE;
	bVisible 		= TRUE;
	bPlaying 		= FALSE;

	iCurrentFrame = 0;
	iFrames 		= 0;
	fCurrentFrameRate	= 1.0f / SPRITE_GLOBAL_FRAME_TIME;
	fFrameTime	= 0.0f;
	iWidth 		= 0;
	iHeight 		= 0;
	iHalfWidth 	= 0;
	iHalfHeight 	= 0;

	this->SetPriority(0);
}

INLINE BOOL ISprite::Unload()
{
	sRelease(pFrameImage);
	sRelease(pSprite);

	pAnimation		= NULL;
	pFrame			= NULL;
	pAnimationFrames = NULL;
	bInitialized	= FALSE;

	return TRUE;
}

INLINE BOOL ISprite::Load(const char *filename, IMemoryPool *pool)
{
	return this->Load(filename, pResourceManager, pool);
}

BOOL ISprite::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(pool);
	ASSERT_NULL(filename);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		this->pFilename = filename;
		this->pPool = pool;
		this->pRes = res;

		this->pSprite = static_cast<SpriteObject *>(res->Get(filename, Seed::ObjectSprite, pool));
		this->SetRotation(0);
		this->SetAnimation((u32)0);

		this->bChanged = TRUE;
		this->bInitialized = TRUE;

		this->pAnimation = pSprite->GetAnimation(iCurrentAnimation);
		this->pAnimationFrames = pSprite->GetFrames(pAnimation);
		this->pFrame = &pAnimationFrames[iCurrentFrame]; //&pAnimation->pFrame[iCurrentFrame];
	}

	return TRUE;
}

INLINE void ISprite::Initialize()
{
	this->ReconfigureAnimation();
	bInitialized = TRUE;
}

INLINE const IImage *ISprite::GetImage() const
{
	return pFrameImage; //pFrame->pImage;
}

INLINE void ISprite::ReconfigureAnimation()
{
	iFrames = pAnimation->iFrames;
	bLoop = ((pAnimation->iFlags & Seed::FlagLooped) == Seed::FlagLooped);
	bAnimation = ((pAnimation->iFlags & Seed::FlagAnimated) == Seed::FlagAnimated);
	fFrameTime = 0.0f;

	pFrame = &pAnimationFrames[iCurrentFrame];

	sRelease(pFrameImage);
	pFrameImage = static_cast<IImage *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectImage, pPool));

	this->ReconfigureFrame();

	if (this->bAnimation)
		this->Play();
}

// WARNING: pFrameImage must be valid here.
// FIXME: This can be simplified, do it someday.
INLINE void ISprite::ReconfigureFrame()
{
	ASSERT_NULL(pFrameImage);

	this->fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime); // FIXME: iTime default value must be 1 not 0. <- division by zero

	//if (this->bAnimation)
	//	this->Play();

	if (pFrame->iWidth == 0)
		this->iWidth = this->GetWidthInPixel();
	else
		this->iWidth = static_cast<u16>(pFrame->iWidth);

	if (pFrame->iHeight == 0)
		this->iHeight = this->GetHeightInPixel();
	else
		this->iHeight = static_cast<u16>(pFrame->iHeight);

	#if defined(SEED_USE_REAL_COORDINATE_SYSTEM)
		ITransformable2D::SetWidth((f32)this->iWidth);
		ITransformable2D::SetHeight((f32)this->iHeight);

		u32 iX = pFrame->iX;
		u32 iY = pFrame->iY;

		// Normalized Pixel Half Width/Height for pixel based vertex rendering
		this->iHalfWidth = static_cast<s32>(this->iWidth / 2);
		this->iHalfHeight = static_cast<s32>(this->iHeight / 2);

		f32 rInvWidth = 1.0F / this->GetWidthInPixel(); // full width from image, not only frame area
		f32 rInvHeight = 1.0F / this->GetHeightInPixel(); // full height from image, not only frame area

		fTexS0 = static_cast<f32>((iX) * rInvWidth);
		fTexS1 = static_cast<f32>((iX + iWidth) * rInvWidth);
		fTexT0 = static_cast<f32>((iY) * rInvHeight);
		fTexT1 = static_cast<f32>((iY + iHeight) * rInvHeight);
	#else
		f32 aspectH = pScreen->GetAspectRatio();

		f32 w = (iWidth / static_cast<f32>(pFrame->iResolutionWidth));
		f32 h = (iHeight / static_cast<f32>(pFrame->iResolutionHeight));

		ITransformable2D::SetWidth(w); // set normalized width
		ITransformable2D::SetHeight(h); // set normalized height

		this->fAspectWidth = w;
		this->fAspectHeight = h * aspectH;
		this->fAspectHalfWidth = this->fAspectWidth / 2.0f;
		this->fAspectHalfHeight = this->fAspectHeight / 2.0f;

		u32 iX = pFrame->iX;
		u32 iY = pFrame->iY;

		f32 rInvWidth = 1.0F / pFrameImage->GetAtlasWidthInPixel(); // full width from image, not only frame area
		f32 rInvHeight = 1.0F / pFrameImage->GetAtlasHeightInPixel(); // full height from image, not only frame area

		// Normalized Pixel Half Width/Height for pixel based vertex rendering
		this->iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
		this->iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));
		//this->iHalfWidth = static_cast<u16>(iWidth >> 1);
		//this->iHalfHeight = static_cast<u16>(iHeight >> 1);

		fTexS0 = static_cast<f32>((iX) * rInvWidth);
		fTexS1 = static_cast<f32>((iX + iWidth) * rInvWidth);
		fTexT0 = static_cast<f32>((iY) * rInvHeight);
		fTexT1 = static_cast<f32>((iY + iHeight) * rInvHeight);
	#endif

	this->bChanged = TRUE;
}

INLINE BOOL ISprite::SetAnimation(u32 index)
{
	BOOL ret = FALSE;
	if (pSprite)
	{
		const Animation *pNewAnimation = pSprite->GetAnimation(index);
		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			pAnimationFrames = pSprite->GetFrames(pAnimation);
			iCurrentAnimation = index;
			iCurrentFrame = 0;
			this->ReconfigureAnimation();

			ret = TRUE;
		}
		else
		{
			//Log(TAG "Warning animation %d not found.", index);
		}
	}
	return ret;
}

INLINE BOOL ISprite::SetAnimation(const char *name)
{
	BOOL ret = FALSE;
	if (pSprite)
	{
		const Animation *pNewAnimation = pSprite->GetAnimation(name);
		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			pAnimationFrames = pSprite->GetFrames(pAnimation);
			iCurrentAnimation = pAnimation->iIndex;
			iCurrentFrame = 0;
			this->ReconfigureAnimation();

			ret = TRUE;
		}
		else
		{
			//Log(TAG "Warning animation '%s' not found.", name);
		}
	}
	return ret;
}

INLINE u32 ISprite::GetNumAnimations() const
{
	u32 ret = 0;
	if (this->pSprite)
		ret = pSprite->GetNumAnimations();

	return ret;
}

INLINE u32 ISprite::GetAnimation() const
{
	u32 ret = 0;
	if (this->pAnimation)
		ret = pAnimation->iIndex;

	return ret;
}

INLINE const char *ISprite::GetAnimationName() const
{
	const char *ret = NULL;
	if (this->pAnimation)
		ret = _S(pAnimation->iAnimationId); //pAnimation->pName;

	return ret;
}

INLINE void ISprite::GotoAndStop(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	bPlaying = FALSE;
	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = TRUE;
}

INLINE void ISprite::GotoAndPlay(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	bPlaying = TRUE;
	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = TRUE;
}

INLINE void ISprite::Stop()
{
	bPlaying = FALSE;
}

INLINE void ISprite::Play()
{
	//iCurrentFrameTime = pFrame->iTime;
	fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime);
	bPlaying = TRUE;
	bChanged = TRUE;
}

INLINE BOOL ISprite::IsFinished() const
{
	return (iCurrentFrame == iFrames - 1);
}

INLINE void ISprite::NextFrame()
{
	if (iCurrentFrame < iFrames - 1)
		iCurrentFrame++;
	else if (bLoop)
		iCurrentFrame = 0;

	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = TRUE;
}

INLINE void ISprite::PreviousFrame()
{
	if (iCurrentFrame > 0)
		iCurrentFrame--;
	else if (bLoop)
		iCurrentFrame = iFrames - 1;

	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = TRUE;
}

INLINE void ISprite::SetCurrentFrame(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	this->ReconfigureAnimation();

	bChanged = TRUE;
}

const Seed::ISprite::Frame *ISprite::GetCurrentFrameInfo() const
{
	return this->pFrame;
}

const Seed::ISprite::Frame *ISprite::GetAnimationFrameInfo(u32 animation, u32 frame) const
{
	const Animation *pNewAnimation = pSprite->GetAnimation(animation);
	if (!pNewAnimation)
	{
		//Log(TAG "Warning animation %d not found.", index);
		return NULL;
	}
	return &pSprite->GetFrames(pNewAnimation)[frame];
}

INLINE u32 ISprite::GetCurrentFrame() const
{
	return iCurrentFrame;
}

INLINE u32 ISprite::GetNumFrames() const
{
	return iFrames;
}

INLINE void ISprite::SetLoop(BOOL loop)
{
	this->bLoop = loop;
}

INLINE BOOL ISprite::IsLoop() const
{
	return bLoop;
}

INLINE BOOL ISprite::IsAnimated() const
{
	return bAnimation;
}

void ISprite::Update(f32 delta)
{
	//IBasicMesh::Update(delta);

	if (bPlaying && bAnimation)
	{
		fFrameTime += delta;

		if (fFrameTime > this->fCurrentFrameRate)
		{
			fFrameTime -= this->fCurrentFrameRate;

			if (iCurrentFrame + 1 == iFrames)
			{
				if (bLoop)
				{
					iCurrentFrame = 0;
				}
				else
				{
					bChanged = FALSE;
				}
			}
			else
				iCurrentFrame++;

			pFrame = &pAnimationFrames[iCurrentFrame];

			sRelease(pFrameImage);
			pFrameImage = static_cast<IImage *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectImage, pPool));

			this->ReconfigureFrame();
		}
	}
}

u32 ISprite::GetWidthInPixel() const
{
	return this->iWidth;
}

u32 ISprite::GetHeightInPixel() const
{
	return this->iHeight;
}

INLINE PIXEL ISprite::GetPixel(u32 x, u32 y) const
{
	PIXEL ret = 0;

	if (pFrame && pFrameImage) //pFrame->pImage)
		ret = pFrameImage->GetPixel(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

INLINE u8 ISprite::GetPixelAlpha(u32 x, u32 y) const
{
	u8 ret = 0;
	if (pFrame && pFrameImage) //pFrame->pImage)
		ret = pFrameImage->GetPixelAlpha(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

INLINE const void *ISprite::GetData() const
{
	return pFrameImage->GetData();
}

INLINE IImage *ISprite::GetTexture() const
{
	return pFrameImage;
}

INLINE const char *ISprite::GetObjectName() const
{
	return "ISprite";
}

INLINE int ISprite::GetObjectType() const
{
	return Seed::ObjectSprite;
}

SEED_DISABLE_INSTANCING_IMPL(ISprite);

} // namespace
