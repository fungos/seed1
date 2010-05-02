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

/*! \file CollisionMask.h
	\author Danny Angelo Carminati Grein
	\brief Collision Mask to be do fast pixel test on sprites (ie. Button)
*/

#ifndef __COLLISION_MASK_H__
#define __COLLISION_MASK_H__

#include "interface/IResource.h"
#include "MemoryManager.h"
#include "SeedInit.h"
#include "File.h"

namespace Seed {

IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

class SEED_CORE_API CollisionMask : public IResource
{
	friend IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	public:
		CollisionMask();
		virtual ~CollisionMask();

		virtual BOOL CheckPixel(u32 x, u32 y) const;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

		SEED_DISABLE_INSTANCING;

	private:
		SEED_DISABLE_COPY(CollisionMask);

	private:
		File		stFile;
		u16			iWidth;
		u16			iHeight;
		const u8	*pData;
};

} // namespace

#endif // __COLLISION_MASK_H__
