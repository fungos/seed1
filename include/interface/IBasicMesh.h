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

/*! \file IBasicMesh.h
	\author	Danny Angelo Carminati Grein
	\brief A basic mesh interface
*/

#ifndef __IBASICMESH_H__
#define __IBASICMESH_H__

#include "Defines.h"
#include "Enum.h"
#include "MathUtil.h"
#include "interface/ITransformable2D.h"

namespace Seed {

class ResourceManager;
class IMemoryPool;

class SEED_CORE_API IBasicMesh : public ITransformable2D
{
	public:
		IBasicMesh();
		virtual ~IBasicMesh();

		virtual void SetCustomVertexDataArray(Vector3f *myVertexData, u32 qty, eMeshType type = Seed::TriangleStrip);
		virtual void SetCustomCoordsDataArray(f32 *myCoordsData, u32 qty);

		// IRenderable
		virtual void Update(f32 dt);
		virtual void Render(f32 dt);

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new[](size_t) { return NULL; };
		void operator delete[](void *) {};

	private:
		SEED_DISABLE_COPY(IBasicMesh);

	protected:
		ResourceManager *pRes;
		IMemoryPool		*pPool;

		//Custom vertex and uv map
		Vector3f *arCustomVertexData; // user vertex data
		Vector3f *arCurrentVertexData; // transformed vertex data

		f32 *arCustomCoordsData; // tex coords user data

		f32 fTexS0;
		f32 fTexS1;
		f32 fTexT0;
		f32 fTexT1;

		u32 iNumVertices;
		u32 iNumCustomCoords;

		eMeshType	nMeshType;
};

} // namespace

#endif // __IBASICMESH_H__
