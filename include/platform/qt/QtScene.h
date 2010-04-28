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

/*! \file QtScene.h
	\author	Danny Angelo Carminati Grein
	\brief QT Rendering Scene
*/

#ifndef __QT_SCENE_H__
#define __QT_SCENE_H__

#if defined(_QT_)

#include "Defines.h"
#include <QPainter>
#include <QGraphicsScene>

namespace Seed { namespace QT {

class Scene : public QGraphicsScene
{
	Q_OBJECT

	public:
		Scene(u32 width, u32 height);
		virtual ~Scene();

	protected:
		bool drawScene();
		void drawBackground(QPainter *painter, const QRectF &);
		void setStates(f32 aspect);
		void defaultStates();

		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void wheelEvent(QGraphicsSceneWheelEvent * event);
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);

	private:
		SEED_DISABLE_COPY(Scene);

		QPointF pixelPosToViewPos(const QPointF& p);

	private:
		u32	iWidth;
		u32	iHeight;

		f32 fLastTime;
};

}} // namespace

#endif // _QT_
#endif // __QT_SCENE_H__
