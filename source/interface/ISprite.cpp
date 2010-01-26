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
	: ITransformable2D()
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
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
	, fAspectHalfWidth(0.0f)
	, fAspectHalfHeight(0.0f)
	, fAspectWidth(0.0f)
	, fAspectHeight(0.0f)
	, iCurrentAnimation(0)
	, iCurrentFrame(0)
	, iFrames(0)
	//, iCurrentFrameTime(0)
	, fCurrentFrameRate(0.0f)
	, fFrameTime(0.0f)
	//, iFrameCount(0)
	//, iVertexColor(0)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, iNumVertices(4)
	, arCustomVertexData(NULL)
	, arCurrentVertexData(NULL)
	, iNumCustomCoords(0)
	, arCustomCoordsData(NULL)
	, nMeshType(Seed::TriangleStrip)
	, pRes(NULL)
	, pPool(NULL)
	, pFilename(NULL)
{
}

ISprite::~ISprite()
{
	this->Unload();
}

INLINE void ISprite::Reset()
{
	if (pFrameImage)
		pFrameImage->Release();
		
	if (pSprite)
		pSprite->Release();

	this->pSprite = NULL;
	this->pAnimation	= NULL;
	this->pFrame		= NULL;
	this->pFrameImage	= NULL;
	this->pAnimationFrames = NULL;
	this->bInitialized	= FALSE;
	this->bChanged 		= FALSE;
	this->bAnimation 	= FALSE;
	this->bLoop 		= FALSE;
	this->bVisible 		= TRUE;
	this->bPlaying 		= FALSE;

	this->iCurrentFrame = 0;
	this->iFrames 		= 0;
	//this->iCurrentFrameTime = SPRITE_GLOBAL_FRAME_TIME;
	fCurrentFrameRate	= 1.0f / SPRITE_GLOBAL_FRAME_TIME;
	//this->iFrameCount 	= 0;
	this->fFrameTime	= 0.0f;
	this->iWidth 		= 0;
	this->iHeight 		= 0;
	//this->iVertexColor 	= 0;
	this->iHalfWidth 	= 0;
	this->iHalfHeight 	= 0;

	this->SetPriority(0);
}

INLINE BOOL ISprite::Unload()
{
	if (pFrameImage)
		pFrameImage->Release();
		
	if (pSprite)
		pSprite->Release();

	this->pSprite = NULL;
	this->pAnimation	= NULL;
	this->pFrame		= NULL;
	this->pFrameImage	= NULL;
	this->pAnimationFrames = NULL;
	this->bInitialized = FALSE;

	return TRUE;
}

INLINE BOOL ISprite::Load(const char *filename, IMemoryPool *pool)
{
	return this->Load(filename, &glResourceManager, pool);
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
	//iFrameCount = 0;
	fFrameTime = 0.0f;

	pFrame = &pAnimationFrames[iCurrentFrame]; //&pAnimation->pFrame[iCurrentFrame];
	if (pFrameImage)
		pFrameImage->Release();
	pFrameImage = static_cast<IImage *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectImage, pPool));

	this->ReconfigureFrame();
}

// WARNING: pFrameImage must be valid here.
// FIXME: This can be simplified, do it someday.
INLINE void ISprite::ReconfigureFrame()
{
	ASSERT_NULL(pFrameImage);
	
	//this->iCurrentFrameTime = pFrame->iTime;
	this->fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime);

	if (this->bAnimation)
		this->Play();

	if (pFrame->iWidth == 0)
		this->iWidth = this->GetWidthInPixel();
	else
		this->iWidth = static_cast<u16>(pFrame->iWidth);

	if (pFrame->iHeight == 0)
		this->iHeight = this->GetHeightInPixel();
	else
		this->iHeight = static_cast<u16>(pFrame->iHeight);

	#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
		ITransformable2D::SetWidth((f32)this->iWidth);
		ITransformable2D::SetHeight((f32)this->iHeight);

		u32 iX = pFrame->iX;
		u32 iY = pFrame->iY;

		// Normalized Pixel Half Width/Height for pixel based vertex rendering
		this->iHalfWidth = static_cast<s32>(this->iWidth / 2);
		this->iHalfHeight = static_cast<s32>(this->iHeight / 2);

		f32 rInvWidth = 1.0F / this->GetWidthInPixel(); // full width from image, not only frame area
		f32 rInvHeight = 1.0F / this->GetHeightInPixel(); // full height from image, not only frame area

		this->fTexS0 = static_cast<f32>((iX) * rInvWidth);
		this->fTexS1 = static_cast<f32>((iX + iWidth) * rInvWidth);
		this->fTexT0 = static_cast<f32>((iY) * rInvHeight);
		this->fTexT1 = static_cast<f32>((iY + iHeight) * rInvHeight);

	#else
		f32 aspectW = 1;
		f32 aspectH = static_cast<f32>(pScreen->GetHeight()) / static_cast<f32>(pScreen->GetWidth()) * aspectW;

		f32 w = (iWidth / static_cast<f32>(pFrame->iResolutionWidth)); //static_cast<f32>(pScreen->GetWidth());
		f32 h = (iHeight / static_cast<f32>(pFrame->iResolutionHeight)); // static_cast<f32>(pScreen->GetHeight());
	//	f32 w = (iWidth / static_cast<f32>(pScreen->GetWidth()));
	//	f32 h = (iHeight / static_cast<f32>(pScreen->GetHeight()));

		ITransformable2D::SetWidth(w); // set normalized width
		ITransformable2D::SetHeight(h); // set normalized height

		this->fAspectWidth = w * aspectW;
		this->fAspectHeight = h * aspectH;
		this->fAspectHalfWidth = this->fAspectWidth / 2.0f;
		this->fAspectHalfHeight = this->fAspectHeight / 2.0f;

		u32 iX = pFrame->iX;
		u32 iY = pFrame->iY;

		f32 rInvWidth = 1.0F / this->GetWidthInPixel(); // full width from image, not only frame area
		f32 rInvHeight = 1.0F / this->GetHeightInPixel(); // full height from image, not only frame area

		// Normalized Pixel Half Width/Height for pixel based vertex rendering
		this->iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
		this->iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));
		//this->iHalfWidth = static_cast<u16>(iWidth >> 1);
		//this->iHalfHeight = static_cast<u16>(iHeight >> 1);

		this->fTexS0 = static_cast<f32>((iX) * rInvWidth);
		this->fTexS1 = static_cast<f32>((iX + iWidth) * rInvWidth);
		this->fTexT0 = static_cast<f32>((iY) * rInvHeight);
		this->fTexT1 = static_cast<f32>((iY + iHeight) * rInvHeight);
	#endif

	this->bChanged = TRUE;
}

INLINE void ISprite::SetAnimation(u32 index)
{
	if (pSprite)
	{
		const Animation *pNewAnimation = pSprite->GetAnimation(index);
		if (!pNewAnimation)
		{
			//Log(TAG "Warning animation %d not found.", index);
			return;
		}

		pAnimation = pNewAnimation;
		pAnimationFrames = pSprite->GetFrames(pAnimation);
		iCurrentAnimation = index;
		iCurrentFrame = 0;
		this->ReconfigureAnimation();
	}
}

INLINE void ISprite::SetAnimation(const char *name)
{
	if (pSprite)
	{
		const Animation *pNewAnimation = pSprite->GetAnimation(name);
		if (!pNewAnimation)
		{
			//Log(TAG "Warning animation '%s' not found.", name);
			return;
		}

		pAnimation = pNewAnimation;
		pAnimationFrames = pSprite->GetFrames(pAnimation);
		iCurrentAnimation = pAnimation->iIndex;
		iCurrentFrame = 0;
		this->ReconfigureAnimation();
	}
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

/*
INLINE void ISprite::SetColor(u8 r, u8 g, u8 b, u8 a)
{
	iVertexColor = static_cast<u32>((r << 24) | (g << 16) | (b << 8) | a);
}

INLINE void ISprite::SetColor(PIXEL px)
{
	u8 mR = static_cast<u8>((px & PIXEL_R_MASK) >> PIXEL_R_SHIFT);
	u8 mG = static_cast<u8>((px & PIXEL_G_MASK) >> PIXEL_G_SHIFT);
	u8 mB = static_cast<u8>((px & PIXEL_B_MASK) >> PIXEL_B_SHIFT);
	u8 mA = static_cast<u8>((px & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

	this->SetColor(mR, mG, mB, mA);
}
*/

void ISprite::Update(f32 delta)
{
	if (bPlaying && bAnimation)
	{
		//iFrameCount++;

		fFrameTime += delta;

		if (fFrameTime > this->fCurrentFrameRate)
		{
			fFrameTime -= this->fCurrentFrameRate;

			if (iCurrentFrame + 1 == iFrames)
			{
				if (bLoop)
					iCurrentFrame = 0;
				else
					bChanged = FALSE;
			}
			else
				iCurrentFrame++;

			//iFrameCount = 0;
			pFrame = &pAnimationFrames[iCurrentFrame]; //&pAnimation->pFrame[iCurrentFrame];
			if (pFrameImage)
				pFrameImage->Release();
			pFrameImage = static_cast<IImage *>(pRes->Get(_F(pFrame->iFileId), Seed::ObjectImage, pPool));
			
			this->ReconfigureFrame();
		}
	}
}

void ISprite::Render(f32 delta)
{
	UNUSED(delta);
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
		//ret = pFrame->pImage->GetPixel(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

INLINE u8 ISprite::GetPixelAlpha(u32 x, u32 y) const
{
	u8 ret = 0;
	if (pFrame && pFrameImage) //pFrame->pImage)
		ret = pFrameImage->GetPixelAlpha(x + pFrame->iX, y + pFrame->iY);
		//ret = pFrame->pImage->GetPixelAlpha(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

INLINE const void *ISprite::GetData() const
{
	return pFrameImage->GetData(); //pFrame->pImage->GetData();
}

INLINE IImage *ISprite::GetTexture() const
{
	return pFrameImage;
}

INLINE void ISprite::SetCustomVertexDataArray(Vector3f *myVertexData, u32 qty, eMeshType type)
{
	if (qty > 0)
	{
		iNumVertices = qty;
		arCustomVertexData = myVertexData;
		nMeshType = type;
	}
	else
	{
		iNumVertices = 4;
		nMeshType = Seed::TriangleStrip;
	}
}

INLINE void ISprite::SetCustomCoordsDataArray(f32 *myCoordsData, u32 qty)
{
	if (qty > 0 && qty % 2 == 0)
	{
		iNumCustomCoords = qty;
		arCustomCoordsData = myCoordsData;
	}
}

INLINE const char *ISprite::GetObjectName() const
{
	return "ISprite";
}

INLINE int ISprite::GetObjectType() const
{
	return Seed::ObjectSprite;
}

void *ISprite::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

void ISprite::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}


} // namespace



