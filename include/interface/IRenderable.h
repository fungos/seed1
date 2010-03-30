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


namespace Seed {


class IRenderer;
class IImage;


/// Drawable Interface
/**
Interface for renderables objects. Things that will be rendered to the screen.
*/
class IRenderable : public IObject
{
	friend struct IRenderableAscendingPrioritySort;
	friend struct IRenderableDescendingPrioritySort;
	public:
		/*
		All operations following this will use these variables:

		Variables:
			- C is the color component (RGB);
			- A is the alpha component;

		Subscripts:
			- r is the rasterized output (result);
			- t is the texture source component input (pixel);
			- d is the texture destiny component input (pixel);
			- f is the color fragment intput (glColor*);


		* GL_REPLACE operations *

		Just will replace the rasterized fragment with the newly computed one.


		IRenderable::NONE

		Will blend the incoming texture with the background respecting the texture alpha
		and not the background alfa.

		Cr = Ct * At + Cr * 0
		Ar = At * At + Ar * 0


		* GL_BLEND operations *

		All operations from here are blending between two textures:
		- the source texture (currently selected texture);
		- the destiny texture or rasterized texture (whatever is in the framebuffer / background);


		IRenderable::BLEND

		Cr = Ct * 1.0f + Cd * 1.0f = Ct + Cd
		Ar = At * 1.0f + Ad * 1.0f = At + Ad


		IRenderable::MERGE

		It's an average between source and destiny (Morpho's screen):

		Cr = Ct * At + Cd * Ad
		Ar = At * At + Ad * Ad (this is correct?)


		IRenderable::SCREEN

		This is based in this article:
		http://gmc.yoyogames.com/index.php?s=321b708f77c5e17d0fca772ef7dcd6f9&showtopic=254433&st=0

		Where it try to simulate photo shop screen blending.

		Cr = Ct + Cd * (1.0f - Ct)
		Ar = At + Ad * (1.0f - At)


		* GL_MODULATE operations *

		Modulate is applied based in a Texture Pixel with a Color Fragment (glColor*) based
		in the following formula:


		IRenderable::OVERLAY

		Cr = Ct * Cr + Cf * 1.0f
		Ar = At * Ar + Af * 1.0f


		IRenderable::LIGHTEN

		Cr = Ct * (1.0f - At) + Cf * (1.0f - At)
		Ar = At * (1.0f - At) + Af * (1.0f - At)


		IRenderable::COLOR_DODGE

		Cr = Ct + Cf
		Ar = At + Af


		IRenderable::MODULATE_ALPHA

		Cr = Ct * At +  1 * (1.0f - At)
		Ar = At * At + Af * (1.0f - At)


		IRenderable::MODULATE

		Cr = Ct * At + Cf * (1.0f - At)
		Ar = At * At + Af * (1.0f - At)
		*/
		enum eBlendMode
		{
			NONE = 0,
			BLEND,
			MERGE,
			SCREEN,
			OVERLAY,
			LIGHTEN,
			COLOR_DODGE,
			DECAL_OVERLAY,
			MODULATE_ALPHA,
			MODULATE,
			ADDITIVE
		};

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

		virtual void SetBlending(IRenderable::eBlendMode op);
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


struct IRenderableAscendingPrioritySort
{
	bool operator()(IRenderable * const &left, IRenderable * const &right)
	{
		//return (left->GetPriority() < right->GetPriority());
		return (left->iPriority < right->iPriority);
	}
};

struct IRenderableDescendingPrioritySort
{
	bool operator()(IRenderable * const &left, IRenderable * const &right)
	{
		//return (left->GetPriority() > right->GetPriority());
		return (left->iPriority > right->iPriority);
	}
};

} // namespace


#endif // __IRENDERABLE_H__
