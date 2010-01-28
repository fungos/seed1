/*! \file ISprite.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sprite class interface
*/

#ifndef __ISPRITE_H__
#define __ISPRITE_H__

#include "Defines.h"
#include "interface/IObject.h"
#include "interface/IRenderable.h"
#include "interface/ITransformable2D.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "Image.h"
#include "SeedInit.h"
#include "MathUtil.h"

#define		SPRITE_GLOBAL_FRAME_TIME		60.0f


namespace Seed {


class ResourceManager;
class SpriteObject;
class IImage;


class ISprite : public ITransformable2D
{
	public:
		friend class ResourceLoader;


	public:
		struct Frame
		{
			//ObjectHeader header;
			u32		iIndex;
			u32		iTime;
			u32		iX;
			u32		iY;
			u32		iWidth;
			u32		iHeight;
			u32		iResolutionWidth;
			u32		iResolutionHeight;
			u32		iId; // for pack files with no sequential frames ie. wii tpl format
			u32		iNameId;
			u32		iFileId;
			//char	*pName;
			//char	*pImageFile;
			//IImage	*pImage;

			//Frame() : iTime(0), iX(0), iY(0), iWidth(0), iHeight(0), iId(0), pName(NULL), pImageFile(NULL), pImage(NULL) {}
		};

		struct Animation
		{
			//ObjectHeader header;
			u32		iIndex;
			u32		iFrames;
			eAnimationFlag	iFlags;
			u32		iAnimationId;
			//char	*pName;
			//Frame	*pFrame;

			//Animation() : iIndex(0), iFrames(0), iFlags(Seed::FlagNone), pName(NULL), pFrame(NULL) {}
		};


	public:
		ISprite();
		virtual ~ISprite();

		virtual BOOL Load(const char *filename, IMemoryPool *pool);
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		virtual const IImage *GetImage() const;
		virtual const void *GetData() const;

		//virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		//virtual void SetColor(PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual void SetLoop(BOOL loop);
		virtual BOOL SetAnimation(u32 index);
		virtual BOOL SetAnimation(const char *name);
		virtual void SetCurrentFrame(u32 frame);
		virtual void GotoAndStop(u32 frame);
		virtual void GotoAndPlay(u32 frame);
		virtual void Stop();
		virtual void Play();
		virtual void NextFrame();
		virtual void PreviousFrame();
		virtual u32  GetAnimation() const;
		virtual const char *GetAnimationName() const;
		virtual u32 GetNumAnimations() const;
		virtual u32  GetNumFrames() const;
		virtual u32  GetCurrentFrame() const;
		virtual const Frame *GetCurrentFrameInfo() const;
		virtual const Frame *GetAnimationFrameInfo(u32 animation, u32 frame) const;
		virtual BOOL IsLoop() const;
		virtual BOOL IsAnimated() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		virtual void SetCustomVertexDataArray(Vector3f *myVertexData, u32 qty, eMeshType type = Seed::TriangleStrip);
		virtual void SetCustomCoordsDataArray(f32 *myCoordsData, u32 qty);

		virtual void Reset();
		virtual void Initialize();

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render(f32 delta);
		virtual IImage *GetTexture() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;		
	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

		virtual void ReconfigureAnimation();
		virtual void ReconfigureFrame();

	protected:
		SpriteObject 	 *pSprite;
		const Animation	 *pAnimation;
		const Frame		 *pAnimationFrames;
		const Frame		 *pFrame;
		IImage	 		 *pFrameImage;

		BOOL bInitialized;
		BOOL bChanged;
		BOOL bAnimation;
		BOOL bLoop;
		BOOL bPlaying;

		f32 fTexS0;
		f32 fTexS1;
		f32 fTexT0;
		f32 fTexT1;

		// Frame related width and height *proportional to view port aspect ratio* for rendering only
		f32 fAspectHalfWidth; // real half width
		f32 fAspectHalfHeight; // real half height
		f32 fAspectWidth; // real width
		f32 fAspectHeight; // real height

		u32 iCurrentAnimation;
		u32 iCurrentFrame;
		u32 iFrames;
		f32 fCurrentFrameRate;
		f32 fFrameTime;
		//u32 iVertexColor;

		// Frame related width and heigth used for rendering only
		s32 iHalfWidth; // half width in pixel
		s32 iHalfHeight; // half height in pixel
		u32 iWidth; // width in pixel
		u32 iHeight; // height in pixel

		//Custom vertex and uv map
		u32 iNumVertices;
		Vector3f *arCustomVertexData;
		Vector3f *arCurrentVertexData;

		u32 iNumCustomCoords;
		f32 *arCustomCoordsData;

		eMeshType	nMeshType;

		ResourceManager *pRes;
		IMemoryPool		*pPool;
		const char		*pFilename;

	private:
		SEED_DISABLE_COPY(ISprite);
};


} // namespace


#endif // __ISPRITE_H__
