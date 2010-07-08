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

/*! \file Vertex.h
	\author	Danny Angelo Carminati Grein
	\brief Vertex format
*/

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Defines.h"
#include "Vector3.h"
#include "Point.h"

namespace Seed {

struct sVertex
{
	Vector3f	cVertex;
	uPixel		iColor;
	Point2f		cCoords;

	sVertex()
		: cVertex(0.0f, 0.0f, 0.0f)
		, iColor(255, 255, 255, 255)
		, cCoords(0.0f, 0.0f)
	{
	}

	sVertex &operator=(const sVertex &v)
	{
		cVertex = v.cVertex;
		iColor = v.iColor;
		cCoords = v.cCoords;
	}
};

struct RendererPacket
{
	eMeshType				nMeshType;
	eBlendMode				nBlendMode;
	uPixel					iColor;
	ITexture				*pTexture;
	void					*pVertexData;
	u32						iSize;

	RendererPacket()
		: nMeshType(Seed::TriangleStrip)
		, nBlendMode(Seed::BlendNone)
		, iColor(0, 0, 0, 255)
		, pTexture(NULL)
		, pVertexData(NULL)
		, iSize(0)
	{
	}
};

} // namespace

#endif // __VERTEX_H__
