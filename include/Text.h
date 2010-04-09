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

/*! \file Text.h
	\author	Danny Angelo Carminati Grein
	\brief A text class
*/

#ifndef __TEXT_H__
#define __TEXT_H__

#include "Font.h"
#include "MemoryManager.h"
#include "TString.h"


#include <stdarg.h>

namespace Seed {


class Text
{
	public:
		Text();
		virtual ~Text();

		void Reset();

		void SetText(u32 dictStr);
		void SetText(const WideString str);
		void SetText(const String &str);

		void Draw(u32 index = 0, u32 size = 0);

		f32 GetWidth(u32 index, u32 size);
		u32 GetLengthNonBreakMaxWidth(u32 *index, f32 maxWidth, f32 *lineWidth);

		virtual f32 GetFontTracking() const;
		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual f32 GetX() const;
		virtual f32 GetY() const;

		virtual void SetPosition(f32 x, f32 y);
		virtual void SetFont(Font *f);
		virtual void SetBlending(IRenderable::eBlendMode op);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL px);
		virtual void SetScale(f32 scaleX, f32 scaleY);

		virtual const WideString GetText() const;

	private:
		SEED_DISABLE_COPY(Text);

	private:
		Font		*pFont;
		const WideString	pStrData;

		f32 fPosX;
		f32 fPosY;
		f32 fWidth;
		f32 fHeight;
		f32 fScaleX;
		f32 fScaleY;

		PIXEL iColor;
		IRenderable::eBlendMode iOperation;
};


} // namespace


#endif // __TEXT_H__
