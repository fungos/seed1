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
#include "Renderer2D.h"

#include <math.h>

#define ROUND(x) ((x - static_cast<u32>(x)) > 0.5 ? static_cast<u32>(x) + 1 : static_cast<u32>(x))

#if DEBUG_ENABLE_RECT_BUTTON == 1
#define DEBUG_BUTTON_RECT DEBUG_RECT(IWidget::GetX(), IWidget::GetY(), IWidget::GetWidth(), IWidget::GetHeight(), DEBUG_RECT_COLOR_BUTTON);
#else
#define DEBUG_BUTTON_RECT
#endif

namespace Seed {

IResource *ButtonResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Button *btn = new Button();
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
	//, bSelected(FALSE)
	, bFrameControl(TRUE)
	, bSpriteBased(FALSE)
	, bLabelBased(FALSE)
	, bButtonChanged(TRUE)
	, bSpriteAutoUpdate(FALSE)
	, bCenterDrag(TRUE)
	, bOffsetPressed(FALSE)
	, iDraggingPriority(0)
	, iOldPriority(0)
	, iLabelPressColor(0)
	, iLabelHoverColor(0)
	, iLabelDisabledColor(0)
	, iLabelColor(0)
	, eLabelBlendOperation(Seed::BlendNone)
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
	this->iId = 0;

	this->cSprite.SetParent(this);
	this->cLabel.SetParent(this);
}

Button::~Button()
{
	this->Unload();
	if(Private::bInitialized)
		pGuiManager->Remove(this);
}

BOOL Button::Unload()
{
	if (bSpriteBased)
	{
		this->cSprite.Unload();
		this->bSpriteBased = FALSE;
		this->pPreviousEnabledAnimation = "idle";
	}

	if (bLabelBased)
	{
		this->cLabel.ReleaseText();
		this->bLabelBased = FALSE;
	}

	if (pMask)
		pMask->Release();

	pMask = NULL;
	bLoaded = FALSE;

	return TRUE;
}

void Button::Initialize(u32 id, eCollisionType type)
{
	this->iId				= id;
	this->bDisableHover		= FALSE;
	this->bFrameControl		= TRUE;
	this->eButtonCollision	= type;
	this->bOffsetPressed	= FALSE;

	this->Update(0.0f);
}

void Button::Initialize(u32 id, f32 posX, f32 posY, f32 width, f32 height)
{
	this->iId				= id;
	IWidget::SetWidth(width);
	IWidget::SetHeight(height);
	IWidget::SetX(posX);
	IWidget::SetY(posY);

	this->bDisableHover		= TRUE;
	this->bFrameControl		= TRUE;
	this->bSpriteBased 		= FALSE;
	this->bLabelBased		= FALSE;
	this->bOffsetPressed	= FALSE;
}

BOOL Button::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		this->pRes = res;
		this->pPool = pool;
		this->pFilename = filename;

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
		if (!this->iId)
			READ_U32(this->iId, ptr);

		if (!this->iPriority)
			READ_U32(this->iPriority, ptr);

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

		this->bLoaded = TRUE;
	}

	return this->bLoaded;
}

void Button::Update(f32 dt)
{
	UNUSED(dt);
	//IWidget::Update();

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

		this->bButtonChanged = FALSE;
	}

	bTransformationChanged = FALSE;
}

INLINE void Button::UpdateSprite()
{
	//this->cSprite.SetX(IWidget::GetX() + fSpriteOffsetX);
	//this->cSprite.SetY(IWidget::GetY() + fSpriteOffsetY);
	this->cSprite.SetX(fSpriteOffsetX);
	this->cSprite.SetY(fSpriteOffsetY);

	if (this->bSpriteAutoUpdate)
	{
		this->cSprite.SetRotation(IWidget::GetRotation());
		this->cSprite.SetScaleX(IWidget::GetScaleX());
		this->cSprite.SetScaleY(IWidget::GetScaleY());
		this->cSprite.SetLocalX(IWidget::GetLocalX());
		this->cSprite.SetLocalY(IWidget::GetLocalY());
	}
}

INLINE void Button::UpdateLabel()
{
	switch (eLabelVAlignment)
	{
		case VerticalAlignTop:
		{
			//this->cLabel.SetY(IWidget::GetY());
			this->cLabel.SetY(0.0f);
		}
		break;

		case VerticalAlignCenter:
		{
			//this->cLabel.SetY(IWidget::GetY() + ((IWidget::GetHeight() - cLabel.GetHeight()) / 2.0f));
			this->cLabel.SetY((IWidget::GetHeight() - cLabel.GetHeight()) / 2.0f);
		}
		break;

		case VerticalAlignBottom:
		{
			//this->cLabel.SetY(IWidget::GetY() + IWidget::GetHeight() - cLabel.GetHeight());
			this->cLabel.SetY(IWidget::GetHeight() - cLabel.GetHeight());
		}
		break;

		default:
			 //this->cLabel.SetY(IWidget::GetY() + fLabelOffsetY);
			this->cLabel.SetY(fLabelOffsetY);
		break;
	}

	switch (eLabelHAlignment)
	{
		case HorizontalAlignLeft:
		{
			//this->cLabel.SetX(IWidget::GetX());
			this->cLabel.SetX(0.0f);
		}
		break;

		case HorizontalAlignCenter:
		{
			//this->cLabel.SetX(IWidget::GetX() + ((IWidget::GetWidth() - cLabel.GetWidth()) / 2.0f));
			this->cLabel.SetX((IWidget::GetWidth() - cLabel.GetWidth()) / 2.0f);
		}
		break;

		case HorizontalAlignRight:
		{
			//this->cLabel.SetX(IWidget::GetX() + IWidget::GetWidth() - cLabel.GetWidth());
			this->cLabel.SetX(IWidget::GetWidth() - cLabel.GetWidth());
		}
		break;

		default:
			//this->cLabel.SetX(IWidget::GetX() + fLabelOffsetX);
			this->cLabel.SetX(fLabelOffsetX);
		break;
	}

	this->cLabel.SetRotation(IWidget::GetRotation());
	this->cLabel.SetScaleX(IWidget::GetScaleX());
	this->cLabel.SetScaleY(IWidget::GetScaleY());
	this->cLabel.SetLocalX(IWidget::GetLocalX());
	this->cLabel.SetLocalY(IWidget::GetLocalY());
}

INLINE BOOL Button::ContainsPoint(f32 x, f32 y)
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
	this->cSprite.SetBlending(op);
	this->cLabel.SetBlending(op);
}

void Button::SetColor(u8 r, u8 g, u8 b, u8 a)
{
	this->cSprite.SetColor(r, g, b, a);
	this->cLabel.SetColor(r, g, b, a);
}

void Button::SetColor(PIXEL color)
{
	this->cSprite.SetColor(color);
	this->cLabel.SetColor(color);
}

INLINE BOOL Button::CheckPixel(f32 x, f32 y)
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
	else if (this->pMask)
	{
		ret = pMask->CheckPixel(iPxTargetX, iPxTargetY);
	}

	return ret;
}

void Button::OnWidgetRollOver(const EventWidget *ev)
{
	UNUSED(ev);

	if (!bDisableHover && !bSelected)
	{
		if (bFrameControl)
		{
			if (bSpriteBased)
			{
				//this->cSprite.SetCurrentFrame(1);
				this->cSprite.SetAnimation("rollover");
				this->cSprite.AddPosition(fSpriteHoverOffsetX, fSpriteHoverOffsetY);
			}

			if (bLabelBased)
			{
				this->cLabel.AddPosition(fLabelHoverOffsetX, fLabelHoverOffsetY);
				if (iLabelHoverColor)
				{
					this->cLabel.SetColor(this->iLabelHoverColor);
					this->cLabel.SetBlending(this->eLabelBlendOperation);
				}
			}
		}
	}
}

void Button::OnWidgetRollOut(const EventWidget *ev)
{
	UNUSED(ev);

	/*if (bDisableHover || bSelected)
		return IEvent::IGNORED;*/

	if (!bDisableHover && !bSelected)
	{
		if (bFrameControl)
		{
			if (bSpriteBased)
			{
				//this->cSprite.SetCurrentFrame(static_cast<u32>((bSelected ? 1 : 0)));
				if (!bSelected)
					this->cSprite.SetAnimation("rollout");
				else
					this->cSprite.SetAnimation("selected");

				this->cSprite.AddPosition(fSpriteHoverOffsetX * -1.0f, fSpriteHoverOffsetY * -1.0f);
			}

			if (bLabelBased)
			{
				this->cLabel.AddPosition(fLabelHoverOffsetX * -1.0f, fLabelHoverOffsetY * -1.0f);
				if (iLabelHoverColor)
				{
					this->cLabel.SetBlending(Seed::BlendNone);
					if (this->iLabelColor)
					{
						this->cLabel.SetColor(this->iLabelColor);
						this->cLabel.SetBlending(eLabelBlendOperation);
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
			/*if (cSprite.GetNumFrames() < 3)
			{
				this->cSprite.SetCurrentFrame(1);
			}
			else
			{
				this->cSprite.SetCurrentFrame(2);
			}*/
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
		this->fDragOffsetX = 0.0f;
		this->fDragOffsetY = 0.0f;
		/*this->SetPriority(iOldPriority);
		this->cSprite.SetPriority(iOldPriority);
		this->cLabel.SetPriority(iOldPriority + 1);*/
		this->SetPriority(iOldPriority);
	}

	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			//this->cSprite.SetCurrentFrame(1);
			this->cSprite.SetAnimation("release");
			this->cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
		}

		if (bLabelBased)
		{
			this->cLabel.AddPosition(this->fLabelPressOffsetX * -1.0f, this->fLabelPressOffsetY * -1.0f);
			if (this->iLabelPressColor)
			{
				this->cLabel.SetBlending(Seed::BlendNone);
				if (this->iLabelColor)
				{
					this->cLabel.SetColor(this->iLabelColor);
					this->cLabel.SetBlending(eLabelBlendOperation);
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
		/*this->SetPriority(iOldPriority);
		this->cSprite.SetPriority(iOldPriority);
		this->cLabel.SetPriority(iOldPriority + 1);*/
		this->fDragOffsetX = 0.0f;
		this->fDragOffsetY = 0.0f;
		this->SetPriority(iOldPriority);
	}

	if (bFrameControl)
	{
		if (bSpriteBased)
		{
			//this->cSprite.SetCurrentFrame(static_cast<u32>((bSelected ? 1 : 0)));
			if (bSelected)
				this->cSprite.SetAnimation("selected");
			else
				this->cSprite.SetAnimation("releaseout");
			this->cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
		}

		if (bLabelBased)
		{
			this->cLabel.AddPosition(this->fLabelPressOffsetX * -1.0f, this->fLabelPressOffsetY * -1.0f);
			if (this->iLabelPressColor)
			{
				this->cLabel.SetBlending(Seed::BlendNone);
				if (this->iLabelColor)
				{
					this->cLabel.SetColor(this->iLabelColor);
					this->cLabel.SetBlending(eLabelBlendOperation);
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

		f32 offsetXbtn = this->fDragOffsetX;
		f32 offsetYbtn = this->fDragOffsetY;

		f32 offsetXlbl = this->fDragOffsetX;
		f32 offsetYlbl = this->fDragOffsetY;

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

			this->bButtonChanged = TRUE;
		}
		else if (bLabelBased)
		{
			this->SetX(pInput->GetX() - offsetXlbl);
			this->SetY(pInput->GetY() - offsetYlbl);

			this->bButtonChanged = TRUE;
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
	this->bSelected = TRUE;

	if (bSpriteBased && bFrameControl && !bDisabled)
	{
		//this->cSprite.SetCurrentFrame(1);
		this->cSprite.SetAnimation("selected");
	}

	// what to do with label?
}

INLINE void Button::Unselect()
{
	this->bSelected = FALSE;

	if (bSpriteBased && bFrameControl && !bDisabled)
	{
		//this->cSprite.SetCurrentFrame(0);
		this->cSprite.SetAnimation("idle");
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
					this->pPreviousEnabledAnimation = cSprite.GetAnimationName();

				cSprite.SetAnimation("disabled");
			}
			else
			{
				if (bOffsetPressed)
				{
					//cSprite.AddPosition(fSpritePressOffsetX * -1.0f, fSpritePressOffsetY * -1.0f);
					bOffsetPressed = FALSE;
				}

				//this->cSprite.SetAnimation("idle");
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
				this->cLabel.SetColor(iLabelDisabledColor);
				this->cLabel.SetBlending(eLabelBlendOperation);
			}
			else
			{
				this->cLabel.SetBlending(Seed::BlendNone);
				if (iLabelColor)
				{
					this->cLabel.SetColor(iLabelColor);
					this->cLabel.SetBlending(eLabelBlendOperation);
				}
			}
		}
	}

	IWidget::SetDisabled(b);
}

INLINE void Button::Render(f32 delta)
{
	UNUSED(delta);

	if (bSpriteBased)
	{
		this->cSprite.Render(delta);
	}

	if (bLabelBased)
	{
		this->cLabel.Render(delta);
	}

	//DEBUG_BUTTON_RECT;
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

	//this->cSprite.SetX(IWidget::GetX());
	//this->cSprite.SetY(IWidget::GetY());
	cSprite.SetPosition(0.0f, 0.0f);

	if (cSprite.GetWidth() > this->GetWidth())
		this->SetWidth(cSprite.GetWidth());
	if (cSprite.GetHeight() > this->GetHeight())
		this->SetHeight(cSprite.GetHeight());

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
	this->cSprite.SetVisible(b);
	this->cLabel.SetVisible(b);
}

INLINE void Button::SetFrameControl(BOOL b)
{
	this->bFrameControl = b;
}

INLINE void Button::SetMask(const char *maskName, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(pool);

	if (pMask)
		pMask->Release();

	if (!pRes)
		pRes = res;

	this->pMask = static_cast<CollisionMask *>(pRes->Get(maskName, Seed::ObjectCollisionMask, pool));
	this->eButtonCollision = CollisionByMask;
}

INLINE void Button::SetCollisionType(eCollisionType type)
{
	if (pMask && type != CollisionByMask)
	{
		pMask->Release();
		pMask = NULL;
	}

	this->eButtonCollision = type;
}

INLINE eCollisionType Button::GetCollisionType() const
{
	return this->eButtonCollision;
}

INLINE void Button::SetEnableHover(BOOL b)
{
	this->bDisableHover = b;
}

INLINE BOOL Button::IsHoverDisabled() const
{
	return this->bDisableHover;
}

INLINE void Button::SetPriority(u32 p)
{
	IWidget::SetPriority(p);
	if (this->iDraggingPriority < this->iPriority)
	{
		this->iDraggingPriority = this->iPriority;
	}
	this->cSprite.SetPriority(p);
	this->cLabel.SetPriority(p + 1);
}

INLINE void Button::SetDraggable(BOOL b)
{
	this->bDraggable = b;
}

INLINE void Button::SetDraggingPriority(u32 i)
{
	this->iDraggingPriority = i;
}

INLINE u32 Button::GetDraggingPriority() const
{
	return this->iDraggingPriority;
}

INLINE void Button::SetPressOffset(f32 x, f32 y)
{
	this->fLabelPressOffsetX = this->fSpritePressOffsetX = x;
	this->fLabelPressOffsetY = this->fSpritePressOffsetY = y;
}

INLINE void Button::SetHoverOffset(f32 x, f32 y)
{
	this->fLabelHoverOffsetX = this->fSpriteHoverOffsetX = x;
	this->fLabelHoverOffsetY = this->fSpriteHoverOffsetY = y;
}

INLINE void Button::SetLabelWidth(f32 w)
{
	this->cLabel.SetWidth(w);
}

INLINE void Button::SetLabelHeight(f32 h)
{
	this->cLabel.SetHeight(h);
}

INLINE void Button::SetLabelX(f32 x)
{
	//this->cLabel.SetX(IWidget::GetX() + x);
	this->fLabelOffsetX = x;
	this->eLabelHAlignment = HorizontalAlignNone;
}

INLINE void Button::SetLabelY(f32 y)
{
	//this->cLabel.SetY(IWidget::GetY() + y);
	this->fLabelOffsetY = y;
	this->eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::AddLabelX(f32 value)
{
	//this->cLabel.SetX(IWidget::GetX() + cLabel.GetX() + value);
	this->fLabelOffsetX += value;
	this->eLabelHAlignment = HorizontalAlignNone;
}

INLINE void Button::AddLabelY(f32 value)
{
	//this->cLabel.SetY(IWidget::GetY() + cLabel.GetY() + value);
	this->fLabelOffsetY += value;
	this->eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::SetLabelPosition(f32 x, f32 y)
{
	//this->cLabel.SetPosition(IWidget::GetX() + x, IWidget::GetY() + y);
	this->fLabelOffsetX = x;
	this->fLabelOffsetY = y;
	this->eLabelHAlignment = HorizontalAlignNone;
	this->eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::AddLabelPosition(f32 x, f32 y)
{
	//this->cLabel.SetPosition(IWidget::GetX() + cLabel.GetX() + x, IWidget::GetY() + cLabel.GetY() + y);
	this->fLabelOffsetX += x;
	this->fLabelOffsetY += y;
	this->eLabelHAlignment = HorizontalAlignNone;
	this->eLabelVAlignment = VerticalAlignNone;
}

INLINE void Button::SetLabelRotation(f32 rot)
{
	this->cLabel.SetRotation(rot);
}

INLINE void Button::AddLabelRotation(f32 rot)
{
	this->cLabel.AddRotation(rot);
}


INLINE void Button::SetLabelScaleX(f32 scaleX)
{
	this->cLabel.SetScaleX(scaleX);
}

INLINE void Button::SetLabelScaleY(f32 scaleY)
{
	this->cLabel.SetScaleY(scaleY);
}

INLINE void Button::SetLabelScale(f32 scale)
{
	this->cLabel.SetScale(scale);
}

INLINE void Button::SetLabelScale(f32 scaleX, f32 scaleY)
{
	this->cLabel.SetScale(scaleX, scaleY);
}

INLINE void Button::AddLabelScaleX(f32 scaleX)
{
	this->cLabel.AddScaleX(scaleX);
}

INLINE void Button::AddLabelScaleY(f32 scaleY)
{
	this->cLabel.AddScaleY(scaleY);
}

INLINE void Button::AddLabelScale(f32 scale)
{
	this->cLabel.AddScale(scale);
}

INLINE void Button::AddLabelScale(f32 scaleX, f32 scaleY)
{
	this->cLabel.AddScale(scaleX, scaleY);
}

INLINE f32 Button::GetLabelWidth() const
{
	return this->cLabel.GetWidth();
}

INLINE f32 Button::GetLabelHeight() const
{
	return this->cLabel.GetHeight();
}

INLINE f32 Button::GetLabelX() const
{
	//return this->cLabel.GetX() - IWidget::GetX();
	return fLabelOffsetX;
}


INLINE f32 Button::GetLabelY() const
{
	//return this->cLabel.GetY() - IWidget::GetY();
	return fLabelOffsetY;
}

INLINE f32 Button::GetLabelRotation() const
{
	return this->cLabel.GetRotation();
}

INLINE f32 Button::GetLabelScaleX() const
{
	return this->cLabel.GetScaleX();
}

INLINE f32 Button::GetLabelScaleY() const
{
	return this->cLabel.GetScaleY();
}

INLINE void Button::SetFont(const Font *font)
{
	this->cLabel.SetFont(font);

	if (cLabel.GetWidth() > this->GetWidth())
		this->SetWidth(cLabel.GetWidth());
	if (cLabel.GetHeight() > this->GetHeight())
		this->SetHeight(cLabel.GetHeight());
}

INLINE void Button::SetLabelHorizontalAlignment(eHorizontalAlignment align)
{
	this->eLabelHAlignment = align;

	this->bButtonChanged = TRUE;
}

INLINE void Button::SetLabelVerticalAlignment(eVerticalAlignment align)
{
	this->eLabelVAlignment = align;

	this->bButtonChanged = TRUE;
}

INLINE void Button::SetLabelPressOffset(f32 x, f32 y)
{
	this->fLabelPressOffsetX = x;
	this->fLabelPressOffsetY = y;
}

INLINE void Button::SetLabelHoverOffset(f32 x, f32 y)
{
	this->fLabelHoverOffsetX = x;
	this->fLabelHoverOffsetY = y;
}

INLINE void Button::SetLabelBlending(eBlendMode blending)
{
	//this->cLabel.SetBlending(blending);
	this->eLabelBlendOperation = blending;
}

INLINE void Button::SetLabelColor(u8 r, u8 g, u8 b, u8 a)
{
	this->cLabel.SetBlending(eLabelBlendOperation);
	this->cLabel.SetColor(r, g, b, a);
}

INLINE void Button::SetLabelColor(PIXEL px)
{
	this->cLabel.SetBlending(eLabelBlendOperation);
	this->cLabel.SetColor(px);
}

INLINE void Button::SetLabelPressColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iLabelPressColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelPressColor(PIXEL px)
{
	this->iLabelPressColor = px;
}

INLINE void Button::SetLabelHoverColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iLabelHoverColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelHoverColor(PIXEL px)
{
	this->iLabelHoverColor = px;
}

INLINE void Button::SetLabelDisabledColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iLabelDisabledColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Button::SetLabelDisabledColor(PIXEL px)
{
	this->iLabelDisabledColor = px;
}

INLINE void Button::SetText(const WideString str)
{
	this->cLabel.SetText(str);

	if (cLabel.GetWidth() > this->GetWidth())
		this->SetWidth(cLabel.GetWidth());
	if (cLabel.GetHeight() > this->GetHeight())
		this->SetHeight(cLabel.GetHeight());

	this->UpdateLabel();

	this->bLabelBased = TRUE;
	this->bButtonChanged = TRUE;
}

INLINE void Button::SetText(const String &str)
{
	this->cLabel.SetText(str);

	if (cLabel.GetWidth() > this->GetWidth())
		this->SetWidth(cLabel.GetWidth());
	if (cLabel.GetHeight() > this->GetHeight())
		this->SetHeight(cLabel.GetHeight());

	this->UpdateLabel();

	this->bLabelBased = TRUE;
	this->bButtonChanged = TRUE;
}

/*
INLINE void Button::Print(WideString str, ...)
{
	va_list ap;
	va_start(ap, str);
	this->cLabel.Print(str, &ap);
	va_end(ap);

	if (cLabel.GetWidth() > this->GetWidth())
		this->SetWidth(cLabel.GetWidth());
	if (cLabel.GetHeight() > this->GetHeight())
		this->SetHeight(cLabel.GetHeight());

	this->UpdateLabel();

	this->bLabelBased = TRUE;
	this->bButtonChanged = TRUE;
}
*/

INLINE void Button::SetSpriteWidth(f32 w)
{
	this->cSprite.SetWidth(w);
}

INLINE void Button::SetSpriteHeight(f32 h)
{
	this->cSprite.SetHeight(h);
}

INLINE void Button::SetSpriteX(f32 x)
{
	this->fSpriteOffsetX = x;
}

INLINE void Button::SetSpriteY(f32 y)
{
	this->fSpriteOffsetY = y;
}

INLINE void Button::AddSpriteX(f32 value)
{
	this->fSpriteOffsetX += value;
}

INLINE void Button::AddSpriteY(f32 value)
{
	this->fSpriteOffsetY += value;
}

INLINE void Button::SetSpritePosition(f32 x, f32 y)
{
	this->fSpriteOffsetX = x;
	this->fSpriteOffsetY = y;
}

INLINE void Button::AddSpritePosition(f32 x, f32 y)
{
	this->fSpriteOffsetX += x;
	this->fSpriteOffsetY += y;
}

INLINE void Button::SetSpriteRotation(f32 rot)
{
	this->cSprite.SetRotation(rot);
}

INLINE void Button::AddSpriteRotation(f32 rot)
{
	this->cSprite.AddRotation(rot);
}

INLINE void Button::SetSpriteScaleX(f32 scaleX)
{
	this->cSprite.SetScaleX(scaleX);
}

INLINE void Button::SetSpriteScaleY(f32 scaleY)
{
	this->cSprite.SetScaleY(scaleY);
}

INLINE void Button::SetSpriteScale(f32 scale)
{
	this->cSprite.SetScale(scale);
}

INLINE void Button::SetSpriteScale(f32 scaleX, f32 scaleY)
{
	this->cSprite.SetScale(scaleX, scaleY);
}

INLINE void Button::AddSpriteScaleX(f32 scaleX)
{
	this->cSprite.AddScaleX(scaleX);
}

INLINE void Button::AddSpriteScaleY(f32 scaleY)
{
	this->cSprite.AddScaleY(scaleY);
}

INLINE void Button::AddSpriteScale(f32 scale)
{
	this->cSprite.AddScale(scale);
}

INLINE void Button::AddSpriteScale(f32 scaleX, f32 scaleY)
{
	this->cSprite.AddScale(scaleX, scaleY);
}

INLINE f32 Button::GetSpriteWidth() const
{
	return this->cSprite.GetWidth();
}

INLINE f32 Button::GetSpriteHeight() const
{
	return this->cSprite.GetHeight();
}

INLINE f32 Button::GetSpriteX() const
{
	return this->cSprite.GetX() - IWidget::GetX();
}

INLINE f32 Button::GetSpriteY() const
{
	return this->cSprite.GetY() - IWidget::GetY();
}

INLINE f32 Button::GetSpriteRotation() const
{
	return this->cSprite.GetRotation();
}

INLINE f32 Button::GetSpriteScaleX() const
{
	return this->cSprite.GetScaleX();
}

INLINE f32 Button::GetSpriteScaleY() const
{
	return this->cSprite.GetScaleY();
}

INLINE const char* Button::GetSpriteFilename() const
{
	return this->pstrSpriteFilename;
}

INLINE IImage *Button::GetSpriteTexture() const
{
	return this->cSprite.GetTexture();
}

INLINE void Button::SetSpriteBlending(eBlendMode op)
{
	this->cSprite.SetBlending(op);
}

INLINE void Button::SetSpriteColor(u8 r, u8 g, u8 b, u8 a)
{
	this->cSprite.SetColor(r, g, b, a);
}

INLINE void Button::SetSpriteColor(PIXEL px)
{
	this->cSprite.SetColor(px);
}

INLINE PIXEL Button::GetSpriteColor()
{
	return this->cSprite.GetColor();
}


INLINE void Button::SetSpritePressOffset(f32 x, f32 y)
{
	this->fSpritePressOffsetX = x;
	this->fSpritePressOffsetY = y;
}

INLINE void Button::SetSpriteHoverOffset(f32 x, f32 y)
{
	this->fSpriteHoverOffsetX = x;
	this->fSpriteHoverOffsetY = y;
}

INLINE void Button::SetSpriteAutoUpdate(BOOL b)
{
	this->bSpriteAutoUpdate = b;
}

INLINE BOOL Button::IsSpriteAutoUpdate() const
{
	return this->bSpriteAutoUpdate;
}

INLINE void Button::SetDragCentered(BOOL b)
{
	this->bCenterDrag = b;
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
