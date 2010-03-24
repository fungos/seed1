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
