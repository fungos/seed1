/*! \file Button.cpp
	\author Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief GUI Button Widget
*/


#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Rect.h"
#include "gui/Button.h"
#include "gui/EventWidget.h"
#include "Screen.h"
#include "Formats.h"
#include "CollisionMask.h"
#include "RendererDevice.h"

#include <math.h>

#define ROUND(x) ((x - static_cast<u32>(x)) > 0.5 ? static_cast<u32>(x) + 1 : static_cast<u32>(x))

namespace Seed {

IResource *ButtonResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Button *btn = New(Button());
	btn->Load(filename, res, pool);

	return btn;
}

Button::Button()
	: IWidget()
	, cSprite()
	, cLabel()
	, pPreviousEnabledAnimation("idle")
	, eButtonCollision(CollisionNone)
	, bDisableHover(FALSE)
	, bFrameControl(TRUE)
	, bSpriteBased(FALSE)
	, bLabelBased(FALSE)
	, bButtonChanged(TRUE)
	, bSpriteAutoUpdate(FALSE)
	, bLabelAutoUpdate(FALSE)
	, bCenterDrag(TRUE)
	, bOffsetPressed(FALSE)
	, iDraggingPriority(0)
	, iOldPriority(0)
	, iLabelPressColor(0)
	, iLabelHoverColor(0)
	, iLabelDisabledColor(0)
	, iLabelColor(0)
	, eLabelBlendOperation(BlendNone)
	, fLabelOffsetX(0.0f)
	, fLabelOffsetY(0.0f)
	, fLabelPressOffsetX(0.0f)
	, fLabelPressOffsetY(0.0f)
	, fLabelHoverOffsetX(0.0f)
	, fLabelHoverOffsetY(0.0f)
	, fSpriteOffsetX(0.0f)
	, fSpriteOffsetY(0.0f)
	, fSpritePressOffsetX(0.0f)
	, fSpritePressOffsetY(0.0f)
	, fSpriteHoverOffsetX(0.0f)
	, fSpriteHoverOffsetY(0.0f)
	, fDragOffsetX(0)
	, fDragOffsetY(0)
	, eLabelHAlignment(HorizontalAlignCenter)
	, eLabelVAlignment(VerticalAlignCenter)
	, pstrSpriteFilename(NULL)
	, stFile()
	, pMask(NULL)
{
	iId = 0;

	cSprite.SetParent(this);
	cLabel.SetParent(this);
}

Button::~Button()
{
	this->Unload();
	if (Private::bInitialized)
		pGuiManager->Remove(this);
}

BOOL Button::Unload()
{
	if (bSpriteBased)
	{
		cSprite.Unload();
		bSpriteBased = FALSE;
		pPreviousEnabledAnimation = "idle";
	}

	if (bLabelBased)
	{
		cLabel.ReleaseText();
		bLabelBased = FALSE;
	}

	if (pMask)
		pMask->Release();

	pMask = NULL;
	bLoaded = FALSE;

	return TRUE;
}

void Button::Initialize(u32 id, eCollisionType type)
{
	iId				= id;
	bDisableHover	= FALSE;
	bFrameControl	= TRUE;
	eButtonCollision= type;
	bOffsetPressed	= FALSE;

	this->Update(0.0f);
}

void Button::Initialize(u32 id, f32 posX, f32 posY, f32 width, f32 height)
{
	iId = id;
	this->SetWidth(width);
	this->SetHeight(height);
	this->SetX(posX);
	this->SetY(posY);

	bDisableHover	= TRUE;
	bFrameControl	= TRUE;
	bSpriteBased 	= FALSE;
	bLabelBased		= FALSE;
	bOffsetPressed	= FALSE;
}

BOOL Button::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		pRes = res;
		pPool = pool;
		pFilename = filename;

		/* Isso deveria ser um objeto como SpriteObject -> ButtonObject, para que o .button seja reaproveitado. */
		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Could not open button file.");

		const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, BUTTON_OBJECT_MAGIC, BUTTON_OBJECT_VERSION), "Invalid block header for button.");

		/*
		We check if any property is already set so we WON'T change it. This will allow the user to do Set* things befora doing the Load.
		If the user want that the object properties are loaded it will need call Reset before Load.
		*/
		if (!iId)
			READ_U32(iId, ptr);

		if (!iPriority)
			READ_U32(iPriority, ptr);

		f32 x = 0;
		f32 y = 0;

		READ_F32(x, ptr);
		READ_F32(y, ptr);

		if (!this->GetX())
			this->SetX(x);

		if (!this->GetY())
			this->SetY(y);

		u32 masktype = 0;
		READ_U32(masktype, ptr);

		u32 labelid = 0;
		READ_U32(labelid, ptr);

		//const char *sprite = NULL;
		//READ_STR(sprite, ptr);
		u32 sprFileId = 0;
		READ_U32(sprFileId, ptr);
		if (sprFileId != SEED_INVALID_ID)
			this->SetSprite(_F(sprFileId), res, pool);

		u32 mask = 0;
		if (masktype == 1)
		{
			READ_U32(mask, ptr);
			if (mask != SEED_INVALID_ID && eButtonCollision == CollisionNone)
				this->SetMask(_F(mask), res, pool);
		}

		bLoaded = TRUE;
	}

	return bLoaded;
}

void Button::Update(f32 dt)
{
	UNUSED(dt);

	// abstract
	//IWidget::Update(dt);
	if (bButtonChanged || bTransformationChanged)
	{
		if (bSpriteBased)
		{
			this->UpdateSprite();
		}

		if (bLabelBased)
		{
			this->UpdateLabel();
		}

		bButtonChanged = FALSE;
	}

	bTransformationChanged = FALSE;
	cSprite.Update(dt);
}

INLINE void Button::UpdateSprite()
{
	cSprite.SetX(fSpriteOffsetX);
	cSprite.SetY(fSpriteOffsetY);

	if (bSpriteAutoUpdate)
	{
		cSprite.SetRotation(this->GetRotation());
		cSprite.SetScaleX(this->GetScaleX());
		cSprite.SetScaleY(this->GetScaleY());
		cSprite.SetLocalX(this->GetLocalX());
		cSprite.SetLocalY(this->GetLocalY());
	}
}

INLINE void Button::UpdateLabel()
{
	if (bLabelAutoUpdate)
	{
		cLabel.SetRotation(GetRotation());
		cLabel.SetScaleX(GetScaleX());
		cLabel.SetScaleY(GetScaleY());
		cLabel.SetLocalX(GetLocalX());
		cLabel.SetLocalY(GetLocalY());
	}

	cLabel.SetY(0.0f);
	switch (eLabelVAlignment)
	{
		case VerticalAlignTop:
		{
		}
		break;

		case VerticalAlignCenter:
		{
			cLabel.SetY((this->GetHeight() - cLabel.GetHeight()) / 2.0f);
		}
		break;

		case VerticalAlignBottom:
		{
			cLabel.SetY(this->GetHeight() - cLabel.GetHeight());
		}
		break;

		default:
			cLabel.SetY(fLabelOffsetY);
		break;
	}

	cLabel.SetX(0.0f);
	switch (eLabelHAlignment)
	{
		case HorizontalAlignLeft:
		{
		}
		break;

		case HorizontalAlignCenter:
		{
			cLabel.SetX((this->GetWidth() - cLabel.GetWidth()) / 2.0f);
		}
		break;

		case HorizontalAlignRight:
		{
			cLabel.SetX(this->GetWidth() - cLabel.GetWidth());
		}
		break;

		default:
			cLabel.SetX(fLabelOffsetX);
		break;
	}
}

INLINE BOOL Button::ContainsPoint(const Point2f &pos) const
{
	return this->ContainsPoint(pos.x, pos.y);
}

INLINE BOOL Button::ContainsPoint(f32 x, f32 y) const
{
	BOOL ret = FALSE;

	if (!bSpriteBased && !bLabelBased)
	{
		ret = IWidget::ContainsPoint(x, y);
	}
	else if (bSpriteBased)
	{
		ret = cSprite.ContainsPoint(x, y);
		if (ret)
		{
			if (eButtonCollision == CollisionByPixel || eButtonCollision == CollisionByMask)
				ret = this->CheckPixel(x, y);
		}
	}

	if (!ret && bLabelBased)
	{
		ret = cLabel.ContainsPoint(x, y);
	}

	return ret;
}

void Button::SetBlending(eBlendMode op)
{
	cSprite.SetBlending(op);
	cLabel.SetBlending(op);
}

void Button::SetColor(u32 r, u32 g, u32 b, u32 a)
{
	IWidget::SetColor(r, g, b, a);
	cSprite.SetColor(r, g, b, a);
	cLabel.SetColor(r, g, b, a);
}

void Button::SetColor(f32 r, f32 g, f32 b, f32 a)
{
	IWidget::SetColor(r, g, b, a);
	cSprite.SetColor(r, g, b, a);
	cLabel.SetColor(r, g, b, a);
}

void Button::SetColor(PIXEL color)
{
	IWidget::SetColor(color);
	cSprite.SetColor(color);
	cLabel.SetColor(color);
}

INLINE BOOL Button::CheckPixel(f32 x, f32 y) const
{
	BOOL ret = FALSE;

	u32 pX, pY, iPxTargetX, iPxTargetY;

	#if defined(SEED_USE_REAL_COORDINATE_SYSTEM)
		pX = static_cast<u32>(x);
		pY = static_cast<u32>(y);

		iPxTargetX = pX - static_cast<u32>(cSprite.GetX());
		iPxTargetY = pY - static_cast<u32>(cSprite.GetY());
	#else
		pX = static_cast<u32>(ROUND(x * pScreen->GetWidth()));
		pY = static_cast<u32>(ROUND(y * pScreen->GetHeight()));

		iPxTargetX = pX - static_cast<u32>(ROUND(cSprite.GetX() * pScreen->GetWidth()));
		iPxTargetY = pY - static_cast<u32>(ROUND(cSprite.GetY() * pScreen->GetHeight()));
	#endif

	if (eButtonCollision == CollisionByPixel)
	{
		ret = (cSprite.GetPixelAlpha(iPxTargetX, iPxTargetY) != 0);
	}
	else if (pMask)
	{
		ret = pMask->CheckPixel(iPxTargetX, iPxTargetY);
	}

	return ret;
}

void Button::OnWidgetRollOver(const EventWidget *ev)
{
	UNUSED(ev);

	if (!bDisableHover && !bSelected && bFrameControl)
	{
		if (bSpriteBased)
		{
			cSprite.SetAnimation("rollover");
			cSprite.AddPosition(fSpriteHoverOffsetX, fSpriteHoverOffsetY);
		}

		if (bLabelBased)
		{
			cLabel.AddPosition(fLabelHoverOffsetX, fLabelHoverOffsetY);
			if (iLabelHoverColor)
			{
				cLabel.SetColor(iLabelHoverColor);
				cLabel.SetBlending(eLabelBlendOperation);
			}
		}
	}
}

void Button::OnWidgetRollOut(const EventWidget *ev)
{
	UNUSED(ev);

	if (!bDisableHover && !bSelected)
	{
		if (bFrameControl)
		{
			if (bSpriteBased)
			{
				if (!bSelected)
					cSprite.SetAnimation("rollout");
				else
					cSprite.SetAnimation("selected");

				cSprite.AddPosition(fSpriteHoverOffsetX * -1.0f, fSpriteHoverOffsetY * -1.0f);
			}

			if (bLabelBased)
			{
				cLabel.AddPosition(fLabelHoverOffsetX * -1.0f, fLabelHoverOffsetY * -1.0f);
				if (iLabelHoverColor)
				{
					cLabel.SetBlending(BlendNone);
					if (iLabelColor)
					{
						cLabel.SetColor(iLabelColor);
						cLabel.SetBlending(eLabelBlendOperation);
					}
				}
			}
		}
	}
}

void Button::OnWidgetPress(const EventWidget *ev)
{
	UNUSED(ev);

	// drag n drop
	iOldPriority = this->GetPriority();

	if (bDraggable)
	{
		fDragOffsetX = ev->GetX() - this->GetX();
		fDragOffsetY = ev->GetY() - this->GetY();
	}

	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			bOffsetPressed = TRUE;
			cSprite.SetAnimation("press");
			cSprite.AddPosition(fSpritePressOffsetX, fSpritePressOffsetY);
		}

		if (bLabelBased)
		{
			cLabel.AddPosition(fLabelPressOffsetX, fLabelPressOffsetY);
			if (iLabelPressColor)
			{
				cLabel.SetColor(iLabelPressColor);
				cLabel.SetBlending(eLabelBlendOperation);
			}
		}
	}
}

void Button::OnWidgetRelease(const EventWidget *ev)
{
	UNUSED(ev);

	if (bDraggable)
	{
		fDragOffsetX = 0.0f;
		fDragOffsetY = 0.0f;
		this->SetPriority(iOldPriority);
	}

	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			cSprite.SetAnimation("release");
			cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
		}

		if (bLabelBased)
		{
			cLabel.AddPosition(fLabelPressOffsetX * -1.0f, fLabelPressOffsetY * -1.0f);
			if (iLabelPressColor)
			{
				cLabel.SetBlending(BlendNone);
				if (iLabelColor)
				{
					cLabel.SetColor(iLabelColor);
					cLabel.SetBlending(eLabelBlendOperation);
				}
			}
		}
	}
}

void Button::OnWidgetReleaseOut(const EventWidget *ev)
{
	UNUSED(ev);

	if (bDraggable)
	{
		fDragOffsetX = 0.0f;
		fDragOffsetY = 0.0f;
		this->SetPriority(iOldPriority);
	}

	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			if (bSelected)
				cSprite.SetAnimation("selected");
			else
				cSprite.SetAnimation("releaseout");
			cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
		}

		if (bLabelBased)
		{
			cLabel.AddPosition(fLabelPressOffsetX * -1.0f, fLabelPressOffsetY * -1.0f);
			if (iLabelPressColor)
			{
				cLabel.SetBlending(BlendNone);
				if (iLabelColor)
				{
					cLabel.SetColor(iLabelColor);
					cLabel.SetBlending(eLabelBlendOperation);
				}
			}
		}
	}
}

INLINE void Button::OnWidgetDrag(const EventWidget *ev)
{
	UNUSED (ev)

	if (bDisabled)
		return;

	if (bDraggable)
	{
		this->SetPriority(iDraggingPriority);

		f32 offsetXbtn = fDragOffsetX;
		f32 offsetYbtn = fDragOffsetY;

		f32 offsetXlbl = fDragOffsetX;
		f32 offsetYlbl = fDragOffsetY;

		if (bCenterDrag)
		{
			offsetXbtn = (cSprite.GetWidth() / 2.0f);
			offsetYbtn = (cSprite.GetHeight() / 2.0f);

			offsetXlbl = (cLabel.GetWidth() / 2.0f);
			offsetYlbl = (cLabel.GetHeight() / 2.0f);
		}

		if (bSpriteBased)
		{
			this->SetX(pInput->GetX() - offsetXbtn);
			this->SetY(pInput->GetY() - offsetYbtn);

			bButtonChanged = TRUE;
		}
		else if (bLabelBased)
		{
			this->SetX(pInput->GetX() - offsetXlbl);
			this->SetY(pInput->GetY() - offsetYlbl);

			bButtonChanged = TRUE;
		}
	}
}

INLINE void Button::OnWidgetDrop(const EventWidget *ev)
{
	UNUSED (ev)

	if (bDisabled)
		return;

	if (bDraggable)
	{
		this->SetPriority(iOldPriority);
	}
}

INLINE void Button::Select()
{
	bSelected = TRUE;

	if (bSpriteBased && bFrameControl && !bDisabled)
	{
		cSprite.SetAnimation("selected");
	}

	// what to do with label?
}

INLINE void Button::Unselect()
{
	bSelected = FALSE;

	if (bSpriteBased && bFrameControl && !bDisabled)
	{
		cSprite.SetAnimation("idle");
	}

	// what to do with label?
}

INLINE void Button::SetDisabled(BOOL b)
{
	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			if (b)
			{
				if (strcmp(cSprite.GetAnimationName(), pPreviousEnabledAnimation) && strcmp(cSprite.GetAnimationName(), "disabled"))
					pPreviousEnabledAnimation = cSprite.GetAnimationName();

				cSprite.SetAnimation("disabled");
			}
			else
			{
				if (bOffsetPressed)
				{
					//cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
					bOffsetPressed = FALSE;
				}

				//cSprite.SetAnimation("idle");
				if (!strcmp(pPreviousEnabledAnimation, "selected"))
				{
					cSprite.SetAnimation("selected");
				}
				else
				{
					BOOL hasAnim = FALSE;
					hasAnim = cSprite.SetAnimation("idle");
					if (hasAnim)
					{
						pPreviousEnabledAnimation = "idle";
					}
					else
					{
						hasAnim = cSprite.SetAnimation("rollout");
						pPreviousEnabledAnimation = "rollout";
					}

					if (!hasAnim)
					{
						cSprite.SetAnimation(pPreviousEnabledAnimation);
					}
				}
			}
		}

		if (bLabelBased && iLabelDisabledColor)
		{
			if (b)
			{
				cLabel.SetColor(iLabelDisabledColor);
				cLabel.SetBlending(eLabelBlendOperation);
			}
			else
			{
				cLabel.SetBlending(BlendNone);
				if (iLabelColor)
				{
					cLabel.SetColor(iLabelColor);
					cLabel.SetBlending(eLabelBlendOperation);
				}
			}
		}
	}

	IWidget::SetDisabled(b);
}

INLINE void Button::Render()
{
	if (bSpriteBased)
	{
		cSprite.Render();
	}

	if (bLabelBased)
	{
		cLabel.Render();
	}

	if (pConfiguration->bDebugButton)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(0, 0, 255, 255));
}

void Button::SetSprite(const char *spriteName, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(spriteName);
	pstrSpriteFilename = spriteName;
	if (!pRes)
	{
		pRes = res;
		cSprite.Load(spriteName, pRes, pool);
	}
	else
	{
		cSprite.Load(spriteName, res, pool);
	}

	cSprite.SetPosition(0.0f, 0.0f);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
 
	if (bLabelBased)
	{
		maxW = cLabel.GetWidth();
		maxH = cLabel.GetHeight();
	}

	if (maxW < cSprite.GetWidth())
		maxW = cSprite.GetWidth();

	if (maxH < cSprite.GetHeight())
		maxH = cSprite.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);

	bSpriteBased = TRUE;
	bButtonChanged = TRUE;

	BOOL hasAnim = FALSE;
	if (!bSelected)
	{
		hasAnim = cSprite.SetAnimation("idle");
		pPreviousEnabledAnimation = "idle";
	}
	else
	{
		hasAnim = cSprite.SetAnimation("selected");
		pPreviousEnabledAnimation = "selected";
	}

	if (!hasAnim)
	{
		hasAnim = cSprite.SetAnimation("rollout");
		pPreviousEnabledAnimation = "rollout";
	}

	if (!hasAnim)
		cSprite.SetAnimation(0u);
}

INLINE void Button::SetVisible(BOOL b)
{
	IWidget::SetVisible(b);
	cSprite.SetVisible(b);
	cLabel.SetVisible(b);
}

INLINE void Button::SetFrameControl(BOOL b)
{
	bFrameControl = b;
}

INLINE void Button::SetMask(const char *maskName, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(pool);

	sRelease(pMask);

	if (!pRes)
		pRes = res;

	pMask = static_cast<CollisionMask *>(pRes->Get(maskName, Seed::ObjectCollisionMask, pool));
	eButtonCollision = CollisionByMask;
}

INLINE void Button::SetCollisionType(eCollisionType type)
{
	if (pMask && type != CollisionByMask)
	{
		sRelease(pMask);
	}

	eButtonCollision = type;
}

INLINE eCollisionType Button::GetCollisionType() const
{
	return eButtonCollision;
}

INLINE void Button::SetEnableHover(BOOL b)
{
	bDisableHover = b;
}

INLINE BOOL Button::IsHoverDisabled() const
{
	return bDisableHover;
}

INLINE void Button::SetPriority(u32 p)
{
	IWidget::SetPriority(p);
	if (iDraggingPriority < iPriority)
	{
		iDraggingPriority = iPriority;
	}
	cSprite.SetPriority(p);
	cLabel.SetPriority(p + 1);
}

INLINE void Button::SetDraggable(BOOL b)
{
	bDraggable = b;
}

INLINE void Button::SetDraggingPriority(u32 i)
{
	iDraggingPriority = i;
}

INLINE u32 Button::GetDraggingPriority() const
{
	return iDraggingPriority;
}

INLINE void Button::SetPressOffset(f32 x, f32 y)
{
	fLabelPressOffsetX = fSpritePressOffsetX = x;
	fLabelPressOffsetY = fSpritePressOffsetY = y;
}

INLINE void Button::SetHoverOffset(f32 x, f32 y)
{
	fLabelHoverOffsetX = fSpriteHoverOffsetX = x;
	fLabelHoverOffsetY = fSpriteHoverOffsetY = y;
}

INLINE void Button::SetLabelWidth(f32 w)
{
	cLabel.SetWidth(w);
}

INLINE void Button::SetLabelHeight(f32 h)
{
	cLabel.SetHeight(h);
}

INLINE void Button::SetLabelX(f32 x)
{
	//cLabel.SetX(IWidget::GetX() + x);
	fLabelOffsetX = x;
	eLabelHAlignment = HorizontalAlignNone;
}

INLINE void Button::SetLabelY(f32 y)
{
	//cLabel.SetY(IWidget::GetY() + y);
	fLabelOffsetY = y;
	eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::AddLabelX(f32 value)
{
	//cLabel.SetX(IWidget::GetX() + cLabel.GetX() + value);
	fLabelOffsetX += value;
	eLabelHAlignment = HorizontalAlignNone;
}

INLINE void Button::AddLabelY(f32 value)
{
	//cLabel.SetY(IWidget::GetY() + cLabel.GetY() + value);
	fLabelOffsetY += value;
	eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::SetLabelPosition(f32 x, f32 y)
{
	//cLabel.SetPosition(IWidget::GetX() + x, IWidget::GetY() + y);
	fLabelOffsetX = x;
	fLabelOffsetY = y;
	eLabelHAlignment = HorizontalAlignNone;
	eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::AddLabelPosition(f32 x, f32 y)
{
	//cLabel.SetPosition(IWidget::GetX() + cLabel.GetX() + x, IWidget::GetY() + cLabel.GetY() + y);
	fLabelOffsetX += x;
	fLabelOffsetY += y;
	eLabelHAlignment = HorizontalAlignNone;
	eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::SetLabelRotation(f32 rot)
{
	cLabel.SetRotation(rot);
}

INLINE void Button::AddLabelRotation(f32 rot)
{
	cLabel.AddRotation(rot);
}


INLINE void Button::SetLabelScaleX(f32 scaleX)
{
	cLabel.SetScaleX(scaleX);

	f32 max = 0.0f;
	if (bSpriteBased)
		max = cSprite.GetWidth();

	if (max < cLabel.GetWidth())
		max = cLabel.GetWidth();

	this->cLabel.SetLocalX(0.5f);

	this->SetWidth(max);
}

INLINE void Button::SetLabelScaleY(f32 scaleY)
{
	cLabel.SetScaleY(scaleY);

	f32 max = 0.0f;
	if (bSpriteBased)
		max = cSprite.GetHeight();

	if (max < cLabel.GetHeight())
		max = cLabel.GetHeight();

	this->cLabel.SetLocalY(0.5f);

	this->SetHeight(max);
}

INLINE void Button::SetLabelScale(f32 scale)
{
	cLabel.SetScale(scale);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);
}

INLINE void Button::SetLabelScale(f32 scaleX, f32 scaleY)
{
	cLabel.SetScale(scaleX, scaleY);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);
}

INLINE void Button::AddLabelScaleX(f32 scaleX)
{
	cLabel.AddScaleX(scaleX);

	f32 max = 0.0f;
	if (bSpriteBased)
		max = cSprite.GetWidth();

	if (max < cLabel.GetWidth())
		max = cLabel.GetWidth();

	this->SetWidth(max);
}

INLINE void Button::AddLabelScaleY(f32 scaleY)
{
	cLabel.AddScaleY(scaleY);

	f32 max = 0.0f;
	if (bSpriteBased)
		max = cSprite.GetHeight();

	if (max < cLabel.GetHeight())
		max = cLabel.GetHeight();

	this->SetHeight(max);
}

INLINE void Button::AddLabelScale(f32 scale)
{
	cLabel.AddScale(scale);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);
}

INLINE void Button::AddLabelScale(f32 scaleX, f32 scaleY)
{
	cLabel.AddScale(scaleX, scaleY);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);
}

INLINE f32 Button::GetLabelWidth() const
{
	return cLabel.GetWidth();
}

INLINE f32 Button::GetLabelHeight() const
{
	return cLabel.GetHeight();
}

INLINE f32 Button::GetLabelX() const
{
	//return cLabel.GetX() - IWidget::GetX();
	return fLabelOffsetX;
}


INLINE f32 Button::GetLabelY() const
{
	//return cLabel.GetY() - IWidget::GetY();
	return fLabelOffsetY;
}

INLINE f32 Button::GetLabelRotation() const
{
	return cLabel.GetRotation();
}

INLINE f32 Button::GetLabelScaleX() const
{
	return cLabel.GetScaleX();
}

INLINE f32 Button::GetLabelScaleY() const
{
	return cLabel.GetScaleY();
}

INLINE void Button::SetFont(const Font *font)
{
	cLabel.SetFont(font);

	if (cLabel.GetWidth() > this->GetWidth())
		this->SetWidth(cLabel.GetWidth());
	if (cLabel.GetHeight() > this->GetHeight())
		this->SetHeight(cLabel.GetHeight());
}

INLINE void Button::SetLabelHorizontalAlignment(eHorizontalAlignment align)
{
	eLabelHAlignment = align;
	bButtonChanged = TRUE;
}

INLINE void Button::SetLabelVerticalAlignment(eVerticalAlignment align)
{
	eLabelVAlignment = align;
	bButtonChanged = TRUE;
}

INLINE void Button::SetLabelPressOffset(f32 x, f32 y)
{
	fLabelPressOffsetX = x;
	fLabelPressOffsetY = y;
}

INLINE void Button::SetLabelHoverOffset(f32 x, f32 y)
{
	fLabelHoverOffsetX = x;
	fLabelHoverOffsetY = y;
}

INLINE void Button::SetLabelBlending(eBlendMode blending)
{
	//cLabel.SetBlending(blending);
	eLabelBlendOperation = blending;
}

INLINE void Button::SetLabelColor(u32 r, u32 g, u32 b, u32 a)
{
	cLabel.SetBlending(eLabelBlendOperation);
	cLabel.SetColor(r, g, b, a);
}

INLINE void Button::SetLabelColor(PIXEL px)
{
	cLabel.SetBlending(eLabelBlendOperation);
	cLabel.SetColor(px);
}

INLINE void Button::SetLabelPressColor(u32 r, u32 g, u32 b, u32 a)
{
	iLabelPressColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelPressColor(PIXEL px)
{
	iLabelPressColor = px;
}

INLINE void Button::SetLabelHoverColor(u32 r, u32 g, u32 b, u32 a)
{
	iLabelHoverColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelHoverColor(PIXEL px)
{
	iLabelHoverColor = px;
}

INLINE void Button::SetLabelDisabledColor(u32 r, u32 g, u32 b, u32 a)
{
	iLabelDisabledColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelDisabledColor(PIXEL px)
{
	iLabelDisabledColor = px;
}

INLINE void Button::SetLabelAutoUpdate(BOOL b)
{
	bLabelAutoUpdate = b;
	bChanged = TRUE;
}

INLINE BOOL Button::IsLabelAutoUpdate() const
{
	return bLabelAutoUpdate;
}

INLINE void Button::SetText(const WideString str)
{
	cLabel.SetText(str);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;
 
	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);

	this->UpdateLabel();

	bLabelBased = TRUE;
	bButtonChanged = TRUE;
}

INLINE void Button::SetText(const String &str)
{
	cLabel.SetText(str);

	f32 maxW, maxH;
	maxW = maxH = 0.0f;

	if (bSpriteBased)
	{
		maxW = cSprite.GetWidth();
		maxH = cSprite.GetHeight();
	}

	if (maxW < cLabel.GetWidth())
		maxW = cLabel.GetWidth();

	if (maxH < cLabel.GetHeight())
		maxH = cLabel.GetHeight();

	this->SetWidth(maxW);
	this->SetHeight(maxH);

	this->UpdateLabel();

	bLabelBased = TRUE;
	bButtonChanged = TRUE;
}

INLINE void Button::SetSpriteWidth(f32 w)
{
	cSprite.SetWidth(w);
}

INLINE void Button::SetSpriteHeight(f32 h)
{
	cSprite.SetHeight(h);
}

INLINE void Button::SetSpriteX(f32 x)
{
	fSpriteOffsetX = x;
}

INLINE void Button::SetSpriteY(f32 y)
{
	fSpriteOffsetY = y;
}

INLINE void Button::AddSpriteX(f32 value)
{
	fSpriteOffsetX += value;
}

INLINE void Button::AddSpriteY(f32 value)
{
	fSpriteOffsetY += value;
}

INLINE void Button::SetSpritePosition(f32 x, f32 y)
{
	fSpriteOffsetX = x;
	fSpriteOffsetY = y;
}

INLINE void Button::AddSpritePosition(f32 x, f32 y)
{
	fSpriteOffsetX += x;
	fSpriteOffsetY += y;
}

INLINE void Button::SetSpriteRotation(f32 rot)
{
	cSprite.SetRotation(rot);
}

INLINE void Button::AddSpriteRotation(f32 rot)
{
	cSprite.AddRotation(rot);
}

INLINE void Button::SetSpriteScaleX(f32 scaleX)
{
	cSprite.SetScaleX(scaleX);
}

INLINE void Button::SetSpriteScaleY(f32 scaleY)
{
	cSprite.SetScaleY(scaleY);
}

INLINE void Button::SetSpriteScale(f32 scale)
{
	cSprite.SetScale(scale);
}

INLINE void Button::SetSpriteScale(f32 scaleX, f32 scaleY)
{
	cSprite.SetScale(scaleX, scaleY);
}

INLINE void Button::AddSpriteScaleX(f32 scaleX)
{
	cSprite.AddScaleX(scaleX);
}

INLINE void Button::AddSpriteScaleY(f32 scaleY)
{
	cSprite.AddScaleY(scaleY);
}

INLINE void Button::AddSpriteScale(f32 scale)
{
	cSprite.AddScale(scale);
}

INLINE void Button::AddSpriteScale(f32 scaleX, f32 scaleY)
{
	cSprite.AddScale(scaleX, scaleY);
}

INLINE f32 Button::GetSpriteWidth() const
{
	return cSprite.GetWidth();
}

INLINE f32 Button::GetSpriteHeight() const
{
	return cSprite.GetHeight();
}

INLINE f32 Button::GetSpriteX() const
{
	return cSprite.GetX() - IWidget::GetX();
}

INLINE f32 Button::GetSpriteY() const
{
	return cSprite.GetY() - IWidget::GetY();
}

INLINE f32 Button::GetSpriteRotation() const
{
	return cSprite.GetRotation();
}

INLINE f32 Button::GetSpriteScaleX() const
{
	return cSprite.GetScaleX();
}

INLINE f32 Button::GetSpriteScaleY() const
{
	return cSprite.GetScaleY();
}

INLINE const char* Button::GetSpriteFilename() const
{
	return pstrSpriteFilename;
}

INLINE ITexture *Button::GetSpriteTexture() const
{
	return cSprite.GetTexture();
}

INLINE void Button::SetSpriteBlending(eBlendMode op)
{
	cSprite.SetBlending(op);
}

INLINE void Button::SetSpriteColor(u32 r, u32 g, u32 b, u32 a)
{
	cSprite.SetColor(r, g, b, a);
}

INLINE void Button::SetSpriteColor(PIXEL px)
{
	cSprite.SetColor(px);
}

INLINE PIXEL Button::GetSpriteColor()
{
	return cSprite.GetColor();
}

INLINE void Button::SetSpritePressOffset(f32 x, f32 y)
{
	fSpritePressOffsetX = x;
	fSpritePressOffsetY = y;
}

INLINE void Button::SetSpriteHoverOffset(f32 x, f32 y)
{
	fSpriteHoverOffsetX = x;
	fSpriteHoverOffsetY = y;
}

INLINE void Button::SetSpriteAutoUpdate(BOOL b)
{
	bSpriteAutoUpdate = b;
}

INLINE BOOL Button::IsSpriteAutoUpdate() const
{
	return bSpriteAutoUpdate;
}

INLINE void Button::SetDragCentered(BOOL b)
{
	bCenterDrag = b;
}

INLINE const char *Button::GetObjectName() const
{
	return "Button";
}

INLINE int Button::GetObjectType() const
{
	return Seed::ObjectGuiButton;
}

SEED_DISABLE_INSTANCING_IMPL(Button);

} // namespace
