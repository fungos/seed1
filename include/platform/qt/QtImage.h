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

/*! \file QtImage.h
	\author	Danny Angelo Carminati Grein
	\brief Image QT Implementation
*/

#ifndef __QT_IMAGE_H__
#define __QT_IMAGE_H__

#if defined(_QT_)

#include "Defines.h"
#include "File.h"
#include "interface/IImage.h"
#include "SeedInit.h"

#include <QImage>

namespace Seed { namespace QT {

IResource *ImageResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

class Image : public IImage
{
	friend IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Sprite;

	public:
		Image();
		virtual ~Image();

		// IImage
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);
		virtual BOOL Load(u32 width, u32 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		virtual BOOL Reset();

		// IResource
		virtual u32 GetUsedMemory() const;
		int LoadTexture();

	protected:
		int GetTexture();
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Image);

	private:
		IMemoryPool *pPool;
		QImage image;

		u32 iTextureId;

		s32 iHalfWidth;
		s32 iHalfHeight;
};

}} // namespace

#else // _QT_
	#error "Include 'Image.h' instead 'platform/qt/QtImage.h' directly."
#endif // _QT_
#endif // __QT_IMAGE__
