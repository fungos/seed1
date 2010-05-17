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

/*! \file SpriteObject.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sprite Object information
*/

#include "SpriteObject.h"
#include "FileSystem.h"
#include "Formats.h"
#include "Log.h"
#include "SeedInit.h"
#include "StringCache.h"

#define TAG		"[SpriteObject] "

namespace Seed {

IResource *SpriteResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	SpriteObject *spt = New(SpriteObject());
	spt->Load(filename, res, pool);

	return spt;
}

SpriteObject::SpriteObject()
	: iAnimations(0)
	, iMemory(0)
	, pAnimations(NULL)
	, pFrames(NULL)
	, pAnimationFrames(NULL)
{
}

SpriteObject::~SpriteObject()
{
	this->Unload();
}

BOOL SpriteObject::Unload()
{
	if (pAnimations)
	{
		pMemoryManager->Free(pAnimationFrames, pPool);
		pAnimations 		= NULL;
		pFrames 			= NULL;

		iMemory 	= 0;
		iAnimations = 0;
		bLoaded 	= FALSE;

		pFileSystem->Close(&stFile);

#if defined(_WII_)
	#if !defined(SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION)
		pMemoryManager->Free((void *)pTplFile, pPool);
		pTplFile = NULL;
	#endif // SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION
#endif // _WII_
	}

	return TRUE;
}

BOOL SpriteObject::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pFilename = filename;
		pRes 	= res;
		pPool 	= pool;
		iMemory = 0;

	/*
	TODO: 2008-10-29 | DESIGN | HIGH | CORE | Resource extension must be inside .sprite file. | Danny Angelo Carminati Grein
	*/

	#if defined(_WII_)
	#if !defined(SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION)
		// Flawfinder: ignore
		char tplName[128] = "\0";
		std::map<const char *, u32, LowerThanStringComparator> frameIndex;

		u32 flen = static_cast<u32>(STRLEN(filename)-6);
		if (flen > 124) flen = 124;
		STRLCPY(tplName, filename, flen); // - sprite ext.
	/*
	TODO: 2008-09-10 | DESIGN | LOW | CORE | Wii dependent code for single resource. | Danny Angelo Carminati Grein
	*/
		STRLCAT(tplName, "tpl\0", 128);

		pTplFile = (char *)pMemoryManager->Alloc(STRLEN(tplName) + 1, pool);
		ASSERT_NULL(pTplFile);

		STRLCPY(pTplFile, &tplName[0], static_cast<u32>(STRLEN(tplName)));

		u32 id = 0;
		u32 fid = 0;

		iTplFileId = pFileSystem->GetIdByFileName(pTplFile);
	#endif // SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION
	#endif // _WII_

		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Sprite Object couldn't be opened");

		// strict compiler workaround
		u8 *ptr = const_cast<u8 *>(static_cast<const u8 *>(stFile.GetData()));
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, SPRITE_OBJECT_MAGIC, SPRITE_OBJECT_VERSION), "Invalid block header for sound.");

		iMemory += stFile.GetSize();

		SpriteObjectHeader *hdr = NULL;
		READ_STRUCT(hdr, SpriteObjectHeader, ptr);

		iAnimations = hdr->animation_amount;
		pAnimations = static_cast<ISprite::Animation *>((void *)ptr);
		pAnimationFrames = (ISprite::Frame **)pMemoryManager->Alloc(iAnimations * sizeof(ISprite::Frame *), pool, "Animation Frame Pointers", "SpriteObject");

		// Skip animations
		ISprite::Animation *anim;
		for (u32 i = 0; i < iAnimations; i++)
		{
			READ_STRUCT(anim, ISprite::Animation, ptr);
		}

		// Scan frames
		ISprite::Frame *frame;
		READ_STRUCT(frame, ISprite::Frame, ptr);
		pFrames = frame;

		for (u32 i = 0; i < iAnimations; i++)
		{
			anim = &pAnimations[i];
			pAnimationFrames[i] = frame;

			for (u32 f = 0; f < anim->iFrames; f++)
			{
/*
THIS IS A HACK for Wii To use tpl as a container of images
*/
#if defined(_WII_)
	#if !defined(SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION)
				// check if it's using atlas
				if (hdr->flags == 0x01)
				{
					frame->iId = 0;
				}
				else
				{
					const char *str = _F(frame->iFileId);
					if (frameIndex.find(str) != frameIndex.end())
					{
						fid = frameIndex[str];
					}
					else
					{
						fid = id++;
						frameIndex[str] = fid;
					}

					frame->iFileId = iTplFileId;
					frame->iId = fid;
				}

	#else
				frame->iId = 0;
	#endif // SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION
#endif // _WII_
				// Force frame caching
				IResource *obj = res->Get(_F(frame->iFileId), Seed::ObjectImage, pool);
				sRelease(obj); // any garbage collect after this will wipe this object from cache

				READ_STRUCT(frame, ISprite::Frame, ptr);
			}
		}

		bLoaded = TRUE;
	}

	return bLoaded;

}

INLINE const ISprite::Animation *SpriteObject::GetAnimation(const char *anim) const
{
	const ISprite::Animation *p = NULL;

	u32 animId = pStringCache->GetIdByString(anim);
	if (animId != SEED_INVALID_ID)
	{
		for (u32 i = 0; i < iAnimations; i++)
		{
			if (pAnimations[i].iAnimationId == animId)
			{
				p = &pAnimations[i];
				break;
			}
		}
	}
	/*
#if defined(DEBUG)
	if (animId == SEED_INVALID_ID || p == NULL)
	{
		//Log(TAG "WARNING: Calling an animation '%s' that does not exists inside sprite '%s'.", anim, stFile.GetName());
	}
#endif
*/
	return p;
}

INLINE u32 SpriteObject::GetNumAnimations() const
{
	return iAnimations;
}

INLINE const ISprite::Animation *SpriteObject::GetAnimation(u32 index) const
{
	const ISprite::Animation *ret = NULL;

	if (index < iAnimations)
		ret = &pAnimations[index];

	return ret;
}

INLINE const ISprite::Frame *SpriteObject::GetFrames(const ISprite::Animation *anim) const
{
	return pAnimationFrames[anim->iIndex];
}

INLINE u32 SpriteObject::GetUsedMemory() const
{
	return iMemory + IResource::GetUsedMemory() + sizeof(this);
}

INLINE const char *SpriteObject::GetObjectName() const
{
	return "SpriteObject";
}

INLINE int SpriteObject::GetObjectType() const
{
	return Seed::ObjectSprite;
}

SEED_DISABLE_INSTANCING_IMPL(SpriteObject);

} // namespace
