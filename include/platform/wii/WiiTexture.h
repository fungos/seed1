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

/*! \file WiiTexture.h
	\author	Danny Angelo Carminati Grein
	\brief Texture implementation
*/

#ifndef __WII_TEXTURE_H__
#define __WII_TEXTURE_H__

#include "Defines.h"

#if defined(_WII_)

#include "interface/ITexture.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "File.h"
#include "SeedInit.h"

namespace Seed { namespace WII {

IResource *TextureResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

class Texture : public ITexture
{
	friend IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool = pDefaultPool);
	friend class Sprite;

	public:
		Texture();
		virtual ~Texture();

		// ITexture
		//virtual void Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);

		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		virtual void Reset();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		File 	stFile;

	private:
		SEED_DISABLE_COPY(Texture);
		void LoadPalette(const char *name);

	private:
		WiiTexObj pImage;

		u8 	*pImageBuffer;

		f32 fWidth;
		f32 fHeight;

		s32 iHalfWidth;
		s32 iHalfHeight;

		u16 iWidth;
		u16 iHeight;
};

}} // namespace

#else // _WII_
	#error "Include 'Texture.h' instead 'platform/wii/WiiTexture.h' directly."
#endif // _WII_
#endif // __WII_TEXTURE_H__
