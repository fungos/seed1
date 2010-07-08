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

/*! \file SdlTexture.h
	\author	Danny Angelo Carminati Grein
	\brief Texture SDL Implementation
*/

#ifndef __SDL_TEXTURE_H__
#define __SDL_TEXTURE_H__

#if defined(_SDL_)

#include "Defines.h"
#include "File.h"
#include "interface/ITexture.h"
#include "SeedInit.h"

namespace Seed { namespace SDL {

IResource *TextureResourceLoader(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

class SEED_CORE_API Texture : public ITexture
{
	friend IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	public:
		Texture();
		virtual ~Texture();

		// ITexture
		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual u32 GetBytesPerPixel() const;
		virtual void *GetTextureName() const;

		virtual void Update(PIXEL *buffer);
		virtual void Reset();
		virtual BOOL Load(u32 width, u32 height, PIXEL *buffer, u32 atlasWidth = 0, u32 atlasHeight = 0, IMemoryPool *pool = pDefaultPool); // O que acontece no Reload?

		virtual u32 GetAtlasWidthInPixel() const;
		virtual u32 GetAtlasHeightInPixel() const;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		int LoadTexture();
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Texture);

	private:
		SDL_Surface *pSurface;
		void *pData;
		void *pTextureId;

		u32 iBytesPerPixel;
		u32 iPitch;

		u32 iAtlasWidth;
		u32 iAtlasHeight;
};

}} // namespace

#else // _SDL_
	#error "Include 'Texture.h' instead 'platform/sdl/SdlTexture.h' directly."
#endif // _SDL_
#endif // __SDL_TEXTURE__
