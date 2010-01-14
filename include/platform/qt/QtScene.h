/*! \file QtScene.h
	\author	Danny Angelo Carminati Grein
	\brief QT Rendering Scene
*/


#ifndef __QT_SCENE_H__
#define __QT_SCENE_H__

#ifdef _QT_


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
