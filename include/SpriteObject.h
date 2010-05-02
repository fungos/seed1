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

/*! \file SpriteObject.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite Object information
*/

#ifndef __SPRITE_OBJECT_H__
#define __SPRITE_OBJECT_H__

/*
TODO: 2008-09-10 | FEATURE | HIGH | CORE | Multiple resource by sprite object need faster load times. | Danny Angelo Carminati Grein

The use of SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION will enable the SDK to load single tpl files per animation frame
(thus multiple tpl per sprite object). This will slowdown load times but is more portable.

- Why use single file per frame?
  Some platforms may or don't support a native "packed" file as Wii suports the .TPL files.
  Currently, the use of single .TPL file is a "hack" to get it working fast on Wii. Some future
  implementations must consider pipeline tools to create a platform independent pack (compressed or not)
  file or on memory flat filesystem.

*/

#include "interface/IResource.h"
#include "interface/IImage.h"
#include "interface/ISprite.h"
#include "SeedInit.h"
#include "MemoryManager.h"
#include "File.h"
#include "Formats.h"

#include <map>
#include <vector>

//#define SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION

namespace Seed {

IResource *SpriteResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

class SEED_CORE_API SpriteObject : public IResource
{
	friend IResource *SpriteResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	public:
		//typedef std::map<const char *, ISprite::Animation *, LowerThanStringComparator>   AnimationMap;
		//typedef AnimationMap::iterator 													AnimationIterator;

		//typedef std::map<u32, ISprite::Animation *>  AnimationMap;
		//typedef AnimationMap::iterator					AnimationMapIt;

	public:
		SpriteObject();
		virtual ~SpriteObject();

		const ISprite::Animation *GetAnimation(const char *anim) const;
		const ISprite::Animation *GetAnimation(u32 index) const;

		const ISprite::Frame *GetFrames(const ISprite::Animation *anim) const;

		u32 GetNumAnimations() const;

		// IResource
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		// IResource
		virtual BOOL Unload();
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

		SEED_DISABLE_INSTANCING;

	private:
		SEED_DISABLE_COPY(SpriteObject);

	private:
		File					stFile;

#ifndef _IPHONE_
#ifndef SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION
		char					*pTplFile;
		u32						iTplFileId;
#endif // SEED_USE_MULTIPLE_IMAGES_PER_ANIMATION
#endif // _IPHONE_
		//u8					*pData;

		u32						iAnimations;
		u32						iMemory;
		ISprite::Animation		*pAnimations;
		ISprite::Frame			*pFrames;
		ISprite::Frame			**pAnimationFrames;

		//AnimationMap			mapAnim;
};

} // namespace

#endif // __SPRITE_OBJECT_H__
