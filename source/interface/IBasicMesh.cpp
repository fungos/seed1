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

#include "interface/IBasicMesh.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "Log.h"

namespace Seed {

IBasicMesh::IBasicMesh()
	: ITransformable2D()
	, pRes(NULL)
	, pPool(NULL)
	, arCustomVertexData(NULL)
	, arCurrentVertexData(NULL)
	, arCustomCoordsData(NULL)
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
	, iNumVertices(0)
	, iNumCustomCoords(0)
	, nMeshType(Seed::TriangleStrip)
{
}

IBasicMesh::~IBasicMesh()
{
}

INLINE void IBasicMesh::SetCustomVertexDataArray(Vector3f *myVertexData, u32 qty, eMeshType type)
{
	iNumVertices = qty;
	arCustomVertexData = myVertexData;
	nMeshType = type;
}

INLINE void IBasicMesh::SetCustomCoordsDataArray(f32 *myCoordsData, u32 qty)
{
	if (qty % 2 == 0)
	{
		iNumCustomCoords = qty;
		arCustomCoordsData = myCoordsData;
	}
}

void IBasicMesh::Update(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;
}

void IBasicMesh::Render(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;
}

void *IBasicMesh::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

void IBasicMesh::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

} // namespace
