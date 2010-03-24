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

class IBasicMesh : public ITransformable2D
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
		void *operator new [](size_t);
		void operator delete [](void *);

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
