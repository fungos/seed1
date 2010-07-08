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

/*! \file Label.h
	\author Danny Angelo Carminati Grein
	\brief GUI Label Widget
*/

#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

#include "Text.h"
#include "GuiManager.h"
#include "../interface/IWidget.h"

namespace Seed {

class SEED_CORE_API Label : public IWidget
{
	public:
		Label();
		virtual ~Label();

		virtual void Reset();
		virtual void Update(f32 dt);

		virtual void SetAutoAdjust(BOOL b);
		virtual BOOL IsAutoAdjust() const;
		virtual void SetAlignment(eHorizontalAlignment align);
		virtual void SetText(const WideString str);
		virtual void SetText(const String &str);
		virtual void SetFont(const Font *font);
		virtual void ReleaseText();

		f32 GetTextWidth();

		// IRenderable
		virtual void Render();

		// ITransformable2D
		virtual void SetWidth(f32 w);
		virtual void SetScaleX(f32 scaleX);
		virtual void SetScaleY(f32 scaleY);
		virtual void SetScale(f32 scale);
		virtual void SetScale(f32 scaleX, f32 scaleY);
		virtual void SetScale(const Point<f32> &scale);

		virtual void AddScaleX(f32 scaleX);
		virtual void AddScaleY(f32 scaleY);
		virtual void AddScale(f32 scale);
		virtual void AddScale(f32 scaleX, f32 scaleY);
		virtual void AddScale(const Point<f32> &scale);

		virtual f32 GetScaleX() const;
		virtual f32 GetScaleY() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		eHorizontalAlignment	iAlign;
		BOOL		bAutoAdjust;
		f32			fTextScaleX;
		f32			fTextScaleY;
		Text		cText;
		const Font	*pFont;

	private:
		SEED_DISABLE_COPY(Label);
};

} // namespace

#endif // __LABEL_H__
