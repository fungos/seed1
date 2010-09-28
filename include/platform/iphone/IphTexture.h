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

/*! \file IphTexture.h
	\author	Danny Angelo Carminati Grein
	\brief Texture Iphone Implementation
*/

#ifndef __IPH_TEXTURE_H__
#define __IPH_TEXTURE_H__

#if defined(_IPHONE_)

#include "Defines.h"
#include "SeedInit.h"
#include "File.h"
#include "interface/ITexture.h"

#include <OpenGLES/ES1/gl.h>

namespace Seed { namespace iPhone {

IResource *TextureResourceLoader(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

class Texture : public ITexture
{
	friend IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Sprite;

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
		
		virtual void Close();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Texture);

		//void LoadPVRTC(const char *file);
		void LoadPNG(const char *file);

	private:
		enum eTextureFormat
		{
			kTexture2DPixelFormat_Automatic = 0,
			kTexture2DPixelFormat_RGBA8888,
			kTexture2DPixelFormat_RGB565,
			kTexture2DPixelFormat_A8,
			kTexture2DPixelFormat_RGBA2,
		};

	private:
		void	*pData;
		void	*pTextureId;

		u32 iBytesPerPixel;
		u32 iPitch;
		
		u32 iAtlasWidth;
		u32 iAtlasHeight;
		
		u32 iReserved;

		//BOOL bCompressed;
		eTextureFormat pixelFormat;
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Texture.h' instead 'platform/iphone/IphTexture.h' directly."
#endif // _IPHONE_
#endif // __IPH_TEXTURE_H__
