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

/*! \file ITransformable2D.h
	\author	Danny Angelo Carminati Grein
	\brief Defines a primitive Transformable2D class interface
*/

#ifndef __ITRANSFORMABLE2D_H__
#define __ITRANSFORMABLE2D_H__

#include "Defines.h"
#include "Point.h"

namespace Seed {

class SEED_CORE_API ITransformable2D
{
	public:
		ITransformable2D();
		virtual ~ITransformable2D();

		virtual void SetWidth(f32 w);
		virtual void SetHeight(f32 h);

		virtual void SetX(f32 x);
		virtual void SetY(f32 y);
		virtual void AddX(f32 value);
		virtual void AddY(f32 value);

		virtual void SetPosition(f32 x, f32 y);
		virtual void AddPosition(f32 x, f32 y);
		virtual void SetPosition(const Point2f &pos);
		virtual void AddPosition(const Point2f &pos);

		virtual void SetLocalX(f32 x);
		virtual void SetLocalY(f32 y);
		virtual void AddLocalX(f32 value);
		virtual void AddLocalY(f32 value);

		virtual void SetLocalPosition(f32 x, f32 y);
		virtual void AddLocalPosition(f32 x, f32 y);
		virtual void SetLocalPosition(const Point2f &pos);
		virtual void AddLocalPosition(const Point2f &pos);

		virtual void SetRotation(f32 rot);
		virtual void AddRotation(f32 rot);

		virtual void SetScaleX(f32 scaleX);
		virtual void SetScaleY(f32 scaleY);
		virtual void SetScale(f32 scale);
		virtual void SetScale(f32 scaleX, f32 scaleY);
		virtual void SetScale(const Point2f &scale);

		virtual void AddScaleX(f32 scaleX);
		virtual void AddScaleY(f32 scaleY);
		virtual void AddScale(f32 scale);
		virtual void AddScale(f32 scaleX, f32 scaleY);
		virtual void AddScale(const Point2f &scale);

		// Normalized Width and Height
		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;

		virtual f32 GetX() const;
		virtual f32 GetY() const;
		virtual Point2f GetPosition() const;

		virtual f32 GetLocalX() const;
		virtual f32 GetLocalY() const;
		virtual Point2f GetLocal() const;

		virtual f32 GetRotation() const;

		virtual f32 GetScaleX() const;
		virtual f32 GetScaleY() const;

		virtual BOOL ContainsPoint(f32 x, f32 y) const;
		virtual BOOL ContainsPoint(const Point2f &pos) const;

		virtual void SetPriority(u32 prio);
		virtual u32 GetPriority() const;

		/// Set a parent for this transformable
		/**
		When a Transformable2D has a parent it will inherit all it's properies.
		If you use a instanced Transformable2D and delete it, you're responsable
		to set the parent of this object to NULL or we can crash badly.
		\param Set a Transformable2D as parent for this object
		 */
		virtual void SetParent(ITransformable2D *pParent);
		virtual ITransformable2D *GetParent() const;

		virtual void Reset();

		virtual BOOL IsChanged() const;

	protected:
		BOOL bTransformationChanged;
		ITransformable2D *pParent;
		Point2f ptPos;
		Point2f ptLocalPos;
		Point2f ptScale;
		f32 fRotation;
		u32	iPriority;

	public: // FIXME: Font needs it...
		f32 fWidth;
		f32 fHeight;

	private:
		SEED_DISABLE_COPY(ITransformable2D);
};

struct SEED_CORE_API ITransformable2DAscendingPrioritySort
{
	bool operator()(ITransformable2D * const &left, ITransformable2D * const &right)
	{
		return (left->GetPriority() < right->GetPriority());
	}
};

struct SEED_CORE_API ITransformable2DDescendingPrioritySort
{
	bool operator()(ITransformable2D * const &left, ITransformable2D * const &right)
	{
		return (left->GetPriority() > right->GetPriority());
	}
};

} // namespace

#endif // __ITRANSFORMABLE2D_H__
