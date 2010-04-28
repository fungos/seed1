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

/*! \file IRenderable.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderable object interface
*/

#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include "Defines.h"
#include "interface/IObject.h"
#include "Enum.h"

namespace Seed {

class IRenderer;
class IImage;

/// Drawable Interface
/**
Interface for renderables objects. Things that will be rendered to the screen.
*/
class SEED_CORE_API IRenderable : public IObject
{
	friend struct IRenderableAscendingPrioritySort;
	friend struct IRenderableDescendingPrioritySort;

	public:
		IRenderable();
		virtual ~IRenderable();

		virtual void Render(f32 delta);
		virtual void Update(f32 delta);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(f32 r, f32 g, f32 b, f32 a);
		virtual void SetColor(PIXEL px);
		virtual PIXEL GetColor() const;

		virtual void SetPriority(u32 prio);
		virtual u32 GetPriority() const;

		virtual void SetBlending(eBlendMode op);
		virtual void SetVisible(BOOL b);
		BOOL IsVisible() const;

		virtual void SetMask(BOOL b);
		BOOL IsMask() const;

		virtual void SetMasked(BOOL b);
		BOOL IsMasked() const;

		virtual void SetSpecial(BOOL b);
		BOOL IsSpecial() const;

		void SetPersistent(BOOL b);
		BOOL IsPersistent() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		eBlendMode	eBlendOperation;
		u32			iPriority;
		PIXEL		iColor;
		BOOL		bPersistent;
		BOOL		bVisible;
		BOOL		bMask;
		BOOL		bMasked;
		BOOL		bSpecial;
		f32			fPriority;

		virtual void Reset();

	private:
		SEED_DISABLE_COPY(IRenderable);
};

struct SEED_CORE_API IRenderableAscendingPrioritySort
{
	bool operator()(IRenderable * const &left, IRenderable * const &right)
	{
		//return (left->GetPriority() < right->GetPriority());
		return (left->iPriority < right->iPriority);
	}
};

struct SEED_CORE_API IRenderableDescendingPrioritySort
{
	bool operator()(IRenderable * const &left, IRenderable * const &right)
	{
		//return (left->GetPriority() > right->GetPriority());
		return (left->iPriority > right->iPriority);
	}
};

} // namespace

#endif // __IRENDERABLE_H__
