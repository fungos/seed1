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

/*! \file Image.h
	\author	Danny Angelo Carminati Grein
	\brief Image
*/

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "interface/ISceneObject.h"
#include "SeedInit.h"
#include "Vertex.h"

namespace Seed {

class ITexture;

class SEED_CORE_API Image : public ISceneObject
{
	public:
		Image();
		virtual ~Image();

		virtual BOOL Load(const char *filename, IMemoryPool *pool);
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Load(ITexture *texture);
		virtual BOOL Unload();

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render();

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		SEED_DISABLE_COPY(Image);

		ITexture		*pTexture;
		ResourceManager *pRes;
		IMemoryPool		*pPool;
		const char		*pFilename;

		f32 fAspectHalfWidth; // real half width
		f32 fAspectHalfHeight; // real half height
		f32 fAspectWidth; // real width
		f32 fAspectHeight; // real height

		// Frame related width and heigth used for rendering only
		s32 iHalfWidth; // half width in pixel
		s32 iHalfHeight; // half height in pixel
		u32 iWidth; // width in pixel
		u32 iHeight; // height in pixel

		f32 fTexS0;
		f32 fTexS1;
		f32 fTexT0;
		f32 fTexT1;

		sVertex vert[4];

		BOOL bDynamic;
};

} // namespace

#endif // __IMAGE_H__
