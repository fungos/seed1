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
#include "RendererDevice.h"

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
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
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

	iCurrentFrame	= 0;
	iFrames 		= 0;
	fCurrentFrameRate = 1.0f / SPRITE_GLOBAL_FRAME_TIME;
	fFrameTime		= 0.0f;
	iWidth 			= 0;
	iHeight 		= 0;
	iHalfWidth 		= 0;
	iHalfHeight 	= 0;
	fTexS0			= 0;
	fTexS1			= 0;
	fTexT0			= 0;
	fTexT1			= 0;

	this->SetPriority(0);
}

INLINE BOOL ISprite::Unload()
{
	sRelease(pFrameImage);
	sRelease(pSprite);

	pAnimation = NULL;
	pFrame	= NULL;
	pAnimationFrames = NULL;
	bInitialized = FALSE;

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
		pFilename = filename;
		pPool = pool;
		pRes = res;

		pSprite = static_cast<SpriteObject *>(res->Get(filename, Seed::ObjectSprite, pool));
		this->SetRotation(0);
		this->SetAnimation(0u);

		bChanged = TRUE;
		bInitialized = TRUE;

		pAnimation = pSprite->GetAnimation(iCurrentAnimation);
		pAnimationFrames = pSprite->GetFrames(pAnimation);
		pFrame = &pAnimationFrames[iCurrentFrame];
	}

	return TRUE;
}

INLINE void ISprite::Initialize()
{
	this->ReconfigureAnimation();
	bInitialized = TRUE;
}

INLINE void ISprite::ReconfigureAnimation()
{
	iFrames = pAnimation->iFrames;
	bLoop = ((pAnimation->iFlags & Seed::FlagLooped) == Seed::FlagLooped);
	bAnimation = ((pAnimation->iFlags & Seed::FlagAnimated) == Seed::FlagAnimated);
	fFrameTime = 0.0f;

	pFrame = &pAnimationFrames[iCurrentFrame];

	sRelease(pFrameImage);
	pFrameImage = static_cast<ITexture *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectTexture, pPool));

	this->ReconfigureFrame();

	if (this->bAnimation)
		this->Play();
}

// WARNING: pFrameImage must be valid here.
// FIXME: This can be simplified, do it someday.
INLINE void ISprite::ReconfigureFrame()
{
	ASSERT_NULL(pFrameImage);

	fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime);
	if (pFrame->iWidth == 0)
		iWidth = pFrameImage->GetWidthInPixel();
	else
		iWidth = static_cast<u16>(pFrame->iWidth);

	if (pFrame->iHeight == 0)
		iHeight = pFrameImage->GetHeightInPixel();
	else
		iHeight = static_cast<u16>(pFrame->iHeight);

	f32 w = (iWidth / static_cast<f32>(pFrame->iResolutionWidth));
	f32 h = (iHeight / static_cast<f32>(pFrame->iResolutionHeight));

	ITransformable2D::SetWidth(w); // set normalized width
	ITransformable2D::SetHeight(h); // set normalized height

	fAspectWidth = w;
	fAspectHeight = h * pScreen->GetAspectRatio();
	fAspectHalfWidth = fAspectWidth / 2.0f;
	fAspectHalfHeight = fAspectHeight / 2.0f;

	u32 iX = pFrame->iX;
	u32 iY = pFrame->iY;

	f32 rInvWidth = 1.0F / pFrameImage->GetAtlasWidthInPixel(); // full width from image, not only frame area
	f32 rInvHeight = 1.0F / pFrameImage->GetAtlasHeightInPixel(); // full height from image, not only frame area

	// Normalized Pixel Half Width/Height for pixel based vertex rendering
	iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
	iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));

	/*
	FIXME: 0.1f from hell
	*/
	fTexS0 = static_cast<f32>((iX + 0.1f) * rInvWidth);
	fTexS1 = static_cast<f32>((iX + 0.1f + iWidth) * rInvWidth);
	fTexT0 = static_cast<f32>((iY + 0.1f) * rInvHeight);
	fTexT1 = static_cast<f32>((iY + 0.1f + iHeight) * rInvHeight);

	bChanged = TRUE;
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
		ret = Str(pAnimation->iAnimationId); //pAnimation->pName;

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

		if (fFrameTime > fCurrentFrameRate)
		{
			fFrameTime -= fCurrentFrameRate;

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

			u32 oldId = pFrame->iFileId;
			pFrame = &pAnimationFrames[iCurrentFrame];

			if (oldId != pFrame->iFileId)
			{
				sRelease(pFrameImage);
				pFrameImage = static_cast<ITexture *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectTexture, pPool));
			}

			this->ReconfigureFrame();
		}
	}

	if (!bChanged && !this->IsChanged())
		return;

	uPixel p = iColor;
	p.rgba.r = iColor.argb.r;
	p.rgba.g = iColor.argb.g;
	p.rgba.b = iColor.argb.b;
	p.rgba.a = iColor.argb.a;

	if (!arCustomVertexData)
	{
		arCurrentVertexData = &vert[0];

		vert[0].cVertex = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[0].iColor = p;
		vert[0].cCoords = Point2f(fTexS0, fTexT0);

		vert[1].cVertex = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[1].iColor = p;
		vert[1].cCoords = Point2f(fTexS1, fTexT0);

		vert[2].cVertex = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[2].iColor = p;
		vert[2].cCoords = Point2f(fTexS0, fTexT1);

		vert[3].cVertex = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[3].iColor = p;
		vert[3].cCoords = Point2f(fTexS1, fTexT1);
	}
	else
	{
		arCurrentVertexData = arCustomVertexData;
	}

	f32 ratio = pScreen->GetAspectRatio();
	f32 x = fAspectHalfWidth + ISprite::GetX();
	f32 y = fAspectHalfHeight + ISprite::GetY() * ratio;

	f32 lx = ISprite::GetLocalX();
	f32 ly = ISprite::GetLocalY() * ratio;

	Matrix4x4f t1, t2, r, s;
	t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
	r = RotationMatrix(AxisZ, DegToRad(ISprite::GetRotation()));
	s = ScaleMatrix(ISprite::GetScaleX(), ISprite::GetScaleY(), 1.0f);
	t2 = TranslationMatrix(-lx, -ly, 0.0f);
	Matrix4x4f transform = ((t1 * r) * s) * t2;

	if (bTransformationChanged || !arCustomVertexData)
	{
		for (u32 i = 0; i < iNumVertices; i++)
		{
			transform.Transform(arCurrentVertexData[i].cVertex);
		}
	}

	bChanged = FALSE;
	bTransformationChanged = FALSE;
}

u32 ISprite::GetWidthInPixel() const
{
	return iWidth;
}

u32 ISprite::GetHeightInPixel() const
{
	return iHeight;
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

INLINE ITexture *ISprite::GetTexture() const
{
	return pFrameImage;
}

void ISprite::Render()
{
	if (!this->bInitialized)
		return;

	ASSERT(pFrameImage);

	RendererPacket packet;
	packet.iSize = iNumVertices;
	packet.nMeshType = nMeshType;
	packet.pVertexData = arCurrentVertexData;
	packet.pTexture = pFrameImage;
	packet.nBlendMode = this->eBlendOperation;
	packet.iColor = this->iColor;
	pRendererDevice->UploadData(&packet);

	if (pConfiguration->bDebugSprite)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(255, 255, 255, 255));
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
