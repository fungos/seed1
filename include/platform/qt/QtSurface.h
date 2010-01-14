/*! \file QtSurface.h
	\author	Danny Angelo Carminati Grein
	\brief QT Rendering Surface
*/


#ifndef __QT_SURFACE_H__
#define __QT_SURFACE_H__

#ifdef _QT_

#include <QPainter>
#include <QGraphicsScene>


namespace Seed { namespace QT {


class QtSurface : public QGraphicsScene
{
	Q_OBJECT

	public:
		QtSurface(){}
		virtual ~QtSurface(){}
};

}} // namespace


#endif // _QT_
#endif // __QT_SURFACE_H__
