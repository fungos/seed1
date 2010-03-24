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

namespace Seed {

ITransformable2D::ITransformable2D()
	: bTransformationChanged(TRUE)
	, pParent(NULL)
	, ptPos(0.0f, 0.0f)
	, ptLocalPos(0.0f, 0.0f)
	, fWidth(0.0f)
	, fHeight(0.0f)
	, fScaleX(1.0f)
	, fScaleY(1.0f)
	, fRotation(0.0f)
{
}

ITransformable2D::~ITransformable2D()
{
	this->Reset();
}

void ITransformable2D::Reset()
{
	this->ptPos.x		= 0.0f;
	this->ptPos.y		= 0.0f;
	this->ptLocalPos.x	= 0.0f;
	this->ptLocalPos.y	= 0.0f;
	this->fWidth 		= 0.0f;
	this->fHeight 		= 0.0f;
	this->fScaleX 		= 1.0f;
	this->fScaleY 		= 1.0f;
	this->fRotation 	= 0.0f;

	this->pParent		= NULL;

	this->bTransformationChanged = TRUE;

	IRenderable::Reset();
}

INLINE void ITransformable2D::SetWidth(f32 w)
{
	if (w == this->fWidth)
		return;

	this->fWidth = w;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetHeight(f32 h)
{
	if (h == this->fHeight)
		return;

	this->fHeight = h;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetX(f32 x)
{
	if (x == this->ptPos.x)
		return;

	this->ptPos.x = x;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetY(f32 y)
{
	if (y == this->ptPos.y)
		return;

	this->ptPos.y = y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddX(f32 value)
{
	if (value == 0)
		return;

	this->ptPos.x += value;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddY(f32 value)
{
	if (value == 0)
		return;

	this->ptPos.y += value;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetPosition(f32 x, f32 y)
{
	if (this->ptPos.x == x && this->ptPos.y == y)
		return;

	this->ptPos.x = x;
	this->ptPos.y = y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetPosition(const Point<f32> &pos)
{
	if (this->ptPos == pos)
		return;

	this->ptPos = pos;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	this->ptPos.x += x;
	this->ptPos.y += y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddPosition(const Point<f32> &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y)
		return;

	this->ptPos += pos;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalX(f32 x)
{
	if (x == this->ptLocalPos.x)
		return;

	this->ptLocalPos.x = x;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalY(f32 y)
{
	if (y == this->ptLocalPos.y)
		return;

	this->ptLocalPos.y = y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalX(f32 value)
{
	if (value == 0)
		return;

	this->ptLocalPos.x += value;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalY(f32 value)
{
	if (value == 0)
		return;

	this->ptLocalPos.y += value;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalPosition(f32 x, f32 y)
{
	if (this->ptLocalPos.x == x && this->ptLocalPos.y == y)
		return;

	this->ptLocalPos.x = x;
	this->ptLocalPos.y = y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetLocalPosition(const Point<f32> &pos)
{
	if (this->ptLocalPos == pos)
		return;

	this->ptLocalPos = pos;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	this->ptLocalPos.x += x;
	this->ptLocalPos.y += y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddLocalPosition(const Point<f32> &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y)
		return;

	this->ptLocalPos += pos;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetRotation(f32 rot)
{
	if (this->fRotation == rot)
		return;

	this->fRotation = rot;

	if (this->fRotation >= 360) this->fRotation -= 360;
	if (this->fRotation < 0) this->fRotation += 360;

	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddRotation(f32 rot)
{
	if (rot == 0)
		return;

	this->fRotation += rot;

	if (this->fRotation >= 360) this->fRotation -= 360;
	if (this->fRotation < 0) this->fRotation += 360;

	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScaleX(f32 scaleX)
{
	if (this->fScaleX == scaleX)
		return;

	this->fScaleX = scaleX;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScaleY(f32 scaleY)
{
	if (this->fScaleY == scaleY)
		return;

	this->fScaleY = scaleY;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(f32 scale)
{
	if (this->fScaleX == scale && this->fScaleY == scale)
		return;

	this->fScaleX = scale;
	this->fScaleY = scale;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(const Point<f32> &scale)
{
	if (this->fScaleX == scale.x && this->fScaleY == scale.y)
		return;

	this->fScaleX = scale.x;
	this->fScaleY = scale.y;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::SetScale(f32 scaleX, f32 scaleY)
{
	if (this->fScaleX == scaleX && this->fScaleY == scaleY)
		return;

	this->fScaleX = scaleX;
	this->fScaleY = scaleY;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScaleX(f32 scaleX)
{
	if (0.0f == scaleX)
		return;

	this->fScaleX += scaleX;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScaleY(f32 scaleY)
{
	if (0.0f == scaleY)
		return;

	this->fScaleY += scaleY;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(f32 scaleX, f32 scaleY)
{
	if (0.0f == scaleX && 0.0f == scaleY)
		return;

	this->fScaleX += scaleX;
	this->fScaleY += scaleY;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(f32 scale)
{
	if (0.0f == scale)
		return;

	this->fScaleX += scale;
	this->fScaleY += scale;
	this->bTransformationChanged = TRUE;
}

INLINE void ITransformable2D::AddScale(const Point<f32> &scale)
{
	if (0.0f == scale.x && 0.0f == scale.y)
		return;

	this->fScaleX += scale.x;
	this->fScaleY += scale.y;
	this->bTransformationChanged = TRUE;
}

INLINE f32 ITransformable2D::GetRotation() const
{
	f32 r = 0;
	if (pParent)
		r = pParent->GetRotation();

	return r + this->fRotation;
}

INLINE f32 ITransformable2D::GetScaleX() const
{
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleX();

	return s * this->fScaleX;
}

INLINE f32 ITransformable2D::GetScaleY() const
{
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleY();

	return s * this->fScaleY;
}

INLINE f32 ITransformable2D::GetWidth() const
{
	return this->fWidth;
}

INLINE f32 ITransformable2D::GetHeight() const
{
	return this->fHeight;
}

INLINE f32 ITransformable2D::GetX() const
{
	f32 x = 0;
	if (pParent)
		x = pParent->GetX();

	return x + this->ptPos.x;
}

INLINE f32 ITransformable2D::GetY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetY();

	return y + this->ptPos.y;
}

INLINE Point<f32> ITransformable2D::GetPosition() const
{
	Point<f32> p(0.0f, 0.0f);
	if (pParent)
		p = pParent->GetPosition();

	return (this->ptPos + p);
}

INLINE u32 ITransformable2D::GetPriority() const
{
	u32 p = 0;
	if (pParent)
		p = pParent->GetPriority();

	return p + this->iPriority;
}

INLINE f32 ITransformable2D::GetLocalX() const
{
	f32 x = 0;
	if (pParent)
		x = pParent->GetLocalX();

	return x + this->ptLocalPos.x;
}

INLINE f32 ITransformable2D::GetLocalY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetLocalY();

	return y + this->ptLocalPos.y;
}

INLINE Point<f32> ITransformable2D::GetLocal() const
{
	Point<f32> p(0.0f, 0.0f);
	if (pParent)
		p = pParent->GetLocal();

	return (this->ptLocalPos + p);
}

INLINE BOOL ITransformable2D::ContainsPoint(f32 x, f32 y)
{
	if (x > (GetX() + GetWidth()))
	{
		return FALSE;
	}
	else if	(x < GetX())
	{
		return FALSE;
	}
	if (y > (GetY() + GetHeight()))
	{
		return FALSE;
	}
	else if	(y < GetY())
	{
		return FALSE;
	}

	return TRUE;
}

INLINE BOOL ITransformable2D::ContainsPoint(const Point<f32> &pos)
{
	return this->ContainsPoint(pos.x, pos.y);
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
