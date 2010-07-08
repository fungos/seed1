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

/*! \file ITransformable2D.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines a primitive Transformable2D class interface
*/

#include "interface/ITransformable2D.h"
#include "Point.h"
#include "Number.h"

namespace Seed {

ITransformable2D::ITransformable2D()
	: bTransformationChanged(TRUE)
	, pParent(NULL)
	, ptPos(0.0f, 0.0f)
	, ptLocalPos(0.0f, 0.0f)
	, ptScale(1.0f, 1.0f)
	, fWidth(0.0f)
	, fHeight(0.0f)
	, fRotation(0.0f)
	, iPriority(0)
{
}

ITransformable2D::~ITransformable2D()
{
	this->Reset();
}

void ITransformable2D::Reset()
{
	ptPos.x			= 0.0f;
	ptPos.y			= 0.0f;
	ptLocalPos.x	= 0.0f;
	ptLocalPos.y	= 0.0f;
	ptScale.x		= 1.0f;
	ptScale.y		= 1.0f;
	fWidth			= 0.0f;
	fHeight 		= 0.0f;
	fRotation		= 0.0f;

	pParent			= NULL;

	bTransformationChanged = TRUE;

	//IRenderable::Reset();
}

INLINE void ITransformable2D::SetWidth(f32 w)
{
	if (w == fWidth)
		return;

	fWidth = w;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetHeight(f32 h)
{
	if (h == fHeight)
		return;

	fHeight = h;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetX(f32 x)
{
	if (x == ptPos.x)
		return;

	ptPos.x = x;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetY(f32 y)
{
	if (y == ptPos.y)
		return;

	ptPos.y = y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddX(f32 value)
{
	if (value == 0)
		return;

	ptPos.x += value;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddY(f32 value)
{
	if (value == 0)
		return;

	ptPos.y += value;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetPosition(f32 x, f32 y)
{
	if (ptPos.x == x && ptPos.y == y)
		return;

	ptPos.x = x;
	ptPos.y = y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetPosition(const Point2f &pos)
{
	if (ptPos == pos)
		return;

	ptPos = pos;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	ptPos.x += x;
	ptPos.y += y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddPosition(const Point2f &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y)
		return;

	ptPos += pos;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalX(f32 x)
{
	if (x == ptLocalPos.x)
		return;

	ptLocalPos.x = x;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalY(f32 y)
{
	if (y == ptLocalPos.y)
		return;

	ptLocalPos.y = y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalX(f32 value)
{
	if (value == 0)
		return;

	ptLocalPos.x += value;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalY(f32 value)
{
	if (value == 0)
		return;

	ptLocalPos.y += value;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalPosition(f32 x, f32 y)
{
	if (ptLocalPos.x == x && ptLocalPos.y == y)
		return;

	ptLocalPos.x = x;
	ptLocalPos.y = y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalPosition(const Point2f &pos)
{
	if (ptLocalPos == pos)
		return;

	ptLocalPos = pos;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	ptLocalPos.x += x;
	ptLocalPos.y += y;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalPosition(const Point2f &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y)
		return;

	ptLocalPos += pos;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetRotation(f32 rot)
{
	if (fRotation == rot)
		return;

	fRotation = rot;

	if (fRotation >= 360)
		fRotation -= 360;

	if (fRotation < 0)
		fRotation += 360;

	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddRotation(f32 rot)
{
	if (rot == 0)
		return;

	fRotation += rot;

	if (fRotation >= 360)
		fRotation -= 360;

	if (fRotation < 0)
		fRotation += 360;

	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScaleX(f32 scaleX)
{
	if (ptScale.x == scaleX)
		return;

	ptScale.x = scaleX;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScaleY(f32 scaleY)
{
	if (ptScale.y == scaleY)
		return;

	ptScale.y = scaleY;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(f32 scale)
{
	if (ptScale.x == scale && ptScale.y == scale)
		return;

	ptScale.x = scale;
	ptScale.y = scale;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(const Point2f &scale)
{
	if (ptScale == scale)
		return;

	ptScale = scale;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(f32 scaleX, f32 scaleY)
{
	if (ptScale.x == scaleX && ptScale.y == scaleY)
		return;

	ptScale.x = scaleX;
	ptScale.y = scaleY;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScaleX(f32 scaleX)
{
	if (0.0f == scaleX)
		return;

	ptScale.x += scaleX;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScaleY(f32 scaleY)
{
	if (0.0f == scaleY)
		return;

	ptScale.y += scaleY;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(f32 scaleX, f32 scaleY)
{
	if (0.0f == scaleX && 0.0f == scaleY)
		return;

	ptScale.x += scaleX;
	ptScale.y += scaleY;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(f32 scale)
{
	if (0.0f == scale)
		return;

	ptScale.x += scale;
	ptScale.y += scale;
	bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(const Point2f &scale)
{
	if (0.0f == scale.x && 0.0f == scale.y)
		return;

	ptScale.x += scale.x;
	ptScale.y += scale.y;
	bTransformationChanged = TRUE;
}

INLINE f32 ITransformable2D::GetRotation() const
{
	f32 r = 0;
	if (pParent)
		r = pParent->GetRotation();

	return r + fRotation;
}

INLINE f32 ITransformable2D::GetScaleX() const
{
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleX();

	return s * ptScale.x;
}

INLINE f32 ITransformable2D::GetScaleY() const
{
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleY();

	return s * ptScale.y;
}

INLINE f32 ITransformable2D::GetWidth() const
{
	return fWidth * Number::Abs(this->GetScaleX());
}

INLINE f32 ITransformable2D::GetHeight() const
{
	return fHeight * Number::Abs(this->GetScaleY());
}

INLINE f32 ITransformable2D::GetX() const
{
	f32 x = 0;
	if (pParent)
		x = pParent->GetX();

	return x + ptPos.x;
}

INLINE f32 ITransformable2D::GetY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetY();

	return y + ptPos.y;
}

INLINE Point2f ITransformable2D::GetPosition() const
{
	Point2f p(0.0f, 0.0f);
	if (pParent)
		p = pParent->GetPosition();

	return (ptPos + p);
}

INLINE f32 ITransformable2D::GetLocalX() const
{
	f32 x = 0;
	if (pParent)
		x = pParent->GetLocalX();

	return x + ptLocalPos.x;
}

INLINE f32 ITransformable2D::GetLocalY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetLocalY();

	return y + ptLocalPos.y;
}

INLINE Point2f ITransformable2D::GetLocal() const
{
	Point2f p(0.0f, 0.0f);
	if (pParent)
		p = pParent->GetLocal();

	return (ptLocalPos + p);
}

INLINE BOOL ITransformable2D::ContainsPoint(f32 x, f32 y) const
{
	if (x > (this->GetX() + this->GetWidth()))
	{
		return FALSE;
	}
	else if	(x < this->GetX())
	{
		return FALSE;
	}
	if (y > (this->GetY() + this->GetHeight()))
	{
		return FALSE;
	}
	else if	(y < this->GetY())
	{
		return FALSE;
	}

	return TRUE;
}

INLINE BOOL ITransformable2D::ContainsPoint(const Point2f &pos) const
{
	return this->ContainsPoint(pos.x, pos.y);
}

INLINE void ITransformable2D::SetPriority(u32 prio)
{
	iPriority = prio;
}

INLINE u32 ITransformable2D::GetPriority() const
{
	u32 prio = iPriority;

	if (pParent)
		prio += pParent->GetPriority();

	return prio;
}

INLINE void ITransformable2D::SetParent(ITransformable2D *parent)
{
	pParent = parent;
}

INLINE ITransformable2D *ITransformable2D::GetParent() const
{
	return pParent;
}

INLINE BOOL ITransformable2D::IsChanged() const
{
	BOOL ret = bTransformationChanged;
	if (!ret && pParent)
		ret = pParent->IsChanged();

	return ret;
}

} // namespace
