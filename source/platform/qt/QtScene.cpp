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

/*! \file QtScene.cpp
	\author	Danny Angelo Carminati Grein
	\brief Qt Scene
*/

#if defined(_QT_)

#include "platform/qt/QtScene.h"
#include "platform/qt/QtInput.h"
#include "platform/qt/QtScreen.h"
#include "Timer.h"

#include "SeedInit.h"

#define TAG "[Scene] "

namespace Seed { namespace QT {

Scene::Scene(u32 width, u32 height)
	: iWidth(width)
	, iHeight(height)
	, fLastTime(0.0f)
{
	this->setSceneRect(0, 0, width, height);
}

Scene::~Scene()
{
}

void Scene::drawBackground(QPainter *painter, const QRectF &)
{
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());

	painter->beginNativePainting();

	this->setStates(width/height);
	this->drawScene();
	this->defaultStates();

	painter->endNativePainting();
}

bool Scene::drawScene()
{
	f32 newTime		= pTimer->GetMilliseconds() / 1000.0f;
	f32 dt			= newTime - fLastTime;
	fLastTime		= newTime;

	//Private::pRenderer->Render(dt); // FIXME: Viewport rendering!

	return true;
}

// OpenGL specific code

void Scene::setStates(f32 aspect)
{
	UNUSED(aspect)

	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	f32 w = static_cast<f32>(pScreen->GetWidth());
	f32 h = static_cast<f32>(pScreen->GetHeight());

	f32 w2 = static_cast<f32>(width());
	f32 h2 = static_cast<f32>(height());

	//glOrtho(0.0f, 1.0f, h/w, 0.0f, -10000, 0);
	glOrtho(0.0f, w2 / w, h2 / (h + h * (1 - (h / w))), 0.0f, -100000, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);

	// Save previous Renderer state
	glPushAttrib(GL_DEPTH_BUFFER_BIT);

#if SEED_ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#else
	glDisable(GL_DEPTH_TEST);
#endif

	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
}

void Scene::defaultStates()
{
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

QPointF Scene::pixelPosToViewPos(const QPointF& p)
{
	//return QPointF(2.0 * float(p.x()) / width() - 1.0, 1.0 - 2.0 * float(p.y()) / height());
	//return QPointF(float(p.x()) / width(), float(p.y()) / height());
	return QPointF(float(p.x()) / float(pScreen->GetWidth()), float(p.y()) / float(pScreen->GetHeight()));
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());
	pInput->PointerMove(viewpos.x(), viewpos.y());
	//Log(">>>> MOVE: %f,%f", viewpos.x(), viewpos.y());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->buttons() & Qt::LeftButton)
	{
		btn |= static_cast<u32>(Seed::ButtonLeft);
	}
	if (event->buttons() & Qt::RightButton)
	{
		btn |= static_cast<u32>(Seed::ButtonRight);
	}
	if (event->buttons() & Qt::MidButton)
	{
		btn |= static_cast<u32>(Seed::ButtonMiddle);
	}

	pInput->PointerPress(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> PRESS: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->button() == Qt::LeftButton)
	{
		btn |= static_cast<u32>(Seed::ButtonLeft);
	}
	if (event->button() == Qt::RightButton)
	{
		btn |= static_cast<u32>(Seed::ButtonRight);
	}
	if (event->button() == Qt::MidButton)
	{
		btn |= static_cast<u32>(Seed::ButtonMiddle);
	}

	pInput->PointerRelease(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> RELEASE: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);

	event->accept();
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	QGraphicsScene::wheelEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());

	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->delta() > 0)
	{
		btn |= static_cast<u32>(Seed::ButtonUp);
	}
	else if (event->delta() < 0)
	{
		btn |= static_cast<u32>(Seed::ButtonDown);
	}

	pInput->PointerPress(viewpos.x(), viewpos.y(), btn);
	pInput->PointerRelease(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> WHEEL: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);

	event->accept();
}

void Scene::keyPressEvent(QKeyEvent *event)
{
	QGraphicsScene::keyPressEvent(event);
	if (event->isAutoRepeat() || event->isAccepted())
		return;

	u32 key = static_cast<u32>(event->key());
	u32 mod = static_cast<u32>(event->modifiers());

	pInput->KeyboardPress(key, mod);
	//Dbg(">>>> Key Press: %c [%d, %x] Modifier: %x", key, key, key, mod);

	event->accept();
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
	QGraphicsScene::keyReleaseEvent(event);
	if (event->isAutoRepeat() || event->isAccepted())
		return;

	u32 key = static_cast<u32>(event->key());
	u32 mod = static_cast<u32>(event->modifiers());

	pInput->KeyboardRelease(key, mod);
	//Dbg(">>>> Key Release: %c [%d, %x] Modifier: %x", key, key, key, mod);

	event->accept();
}

}} // namespace

#endif // _QT_
