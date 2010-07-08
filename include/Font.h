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

/*! \file Font.h
	\author	Danny Angelo Carminati Grein
	\brief A font class
*/

#ifndef __FONT_H__
#define __FONT_H__

#include "Defines.h"
#include "interface/IResource.h"
#include "Sprite.h"
#include "File.h"

namespace Seed {

IResource *FontResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

class SEED_CORE_API Font : public IResource
{
	friend IResource *FontResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Text;

	public:
		Font();
		virtual ~Font();

		const void *GetLetter(u32 letter);
		void SetLetter(u32 letter);

		f32 GetWidth() const;
		f32 GetHeight() const;
		void SetSpacing(f32 spacing);
		f32 GetSpacing() const;
		void SetSpaceWidth(f32 width);
		f32 GetSpaceWidth() const;
		void SetTracking(f32 tracking);
		f32 GetTracking();
		void SetPosition(f32 x, f32 y);
		void SetLocalPosition(f32 x, f32 y);

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		void SetBlending(eBlendMode op);
		void SetColor(PIXEL px);
		void Draw();
		void Update(f32 dt);
		void SetScale(f32 x, f32 y);

		SEED_DISABLE_INSTANCING;

	private:
		SEED_DISABLE_COPY(Font);
		u32 SelectLetter(u32 letter);

	private:
		File			stFile;
		Sprite			mFont;
		Sprite			mFontExt;
		Sprite			*pFont;

		f32				fSpacing;
		f32				fTracking;
		f32				fSpaceWidth;
		f32				fWidth;
		f32				fHeight;

		u32				iCurrent;
		u32				iTotalGlyphs;
		const u16		*pGlyphs;

		BOOL			bSpace;
};

} // namespace

#endif // __FONT_H__
