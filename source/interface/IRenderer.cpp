/*! \file IRenderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderer class interface
*/

#include "Screen.h"
#include "interface/IRenderable.h"
#include "interface/IRenderer.h"
#include "Log.h"

#include <algorithm>
#include <vector>

namespace Seed {


IRenderer::IRenderer()
	: vRenderables()
	, vRenderablesStatic()
	, vVisibleRenderables()
	, vVisibleRenderablesStatic()
	, vMaskRenderables()
	, vMaskRenderablesStatic()
	, vMaskedRenderables()
	, vMaskedRenderablesStatic()
	, vSpecialRenderables()
	, vSpecialRenderablesStatic()
{
}

IRenderer::~IRenderer()
{
	vRenderables.clear();
	vRenderablesStatic.clear();

	RenderableVector().swap(vRenderables);
	RenderableVector().swap(vRenderablesStatic);

	vVisibleRenderables.clear();
	vVisibleRenderablesStatic.clear();

	RenderableVector().swap(vVisibleRenderables);
	RenderableVector().swap(vVisibleRenderablesStatic);

	vMaskRenderables.clear();
	vMaskRenderablesStatic.clear();

	RenderableVector().swap(vMaskRenderables);
	RenderableVector().swap(vMaskRenderablesStatic);

	vMaskedRenderables.clear();
	vMaskedRenderablesStatic.clear();

	RenderableVector().swap(vMaskedRenderables);
	RenderableVector().swap(vMaskedRenderablesStatic);

	vSpecialRenderables.clear();
	vSpecialRenderablesStatic.clear();

	RenderableVector().swap(vSpecialRenderables);
	RenderableVector().swap(vSpecialRenderablesStatic);
}

INLINE void IRenderer::SetBufferMode(eBufferMode mode)
{
	UNUSED(mode);
}

INLINE void IRenderer::SelectTexture(u32 texId)
{
	UNUSED(texId);
}

INLINE void IRenderer::UploadData(void *userData)
{
	UNUSED(userData);
}

INLINE void IRenderer::Begin() const
{
}

INLINE void IRenderer::End() const
{
}

INLINE void IRenderer::BeginRenderMask() const
{
}

INLINE void IRenderer::BeginRenderMasked() const
{
}

INLINE void IRenderer::BeginRenderUnmasked() const
{
}

INLINE void IRenderer::BeginRenderSpecial() const
{
}

BOOL IRenderer::Update(f32 dt)
{
	UNUSED(dt);

	if (!IModule::IsEnabled())
		return FALSE;

	this->Update(vRenderables);
	this->Update(vRenderablesStatic);

	return TRUE;
}

INLINE void IRenderer::Update(const RenderableVector &vec) const
{
	ConstRenderableVectorIterator it = vec.begin();
	ConstRenderableVectorIterator end = vec.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		obj->Update();
	}
}

BOOL IRenderer::Render(f32 delta)
{
	if (!IModule::IsEnabled())
		return FALSE;

	this->FilterObjects();
	this->Culler();

	this->Begin();
		this->BeginRenderMask();
		this->RenderScene(vMaskRenderables, delta);
		this->RenderScene(vMaskRenderablesStatic, delta);

		this->BeginRenderMasked();
		this->RenderScene(vMaskedRenderables, delta);
		this->RenderScene(vMaskedRenderablesStatic, delta);

		this->BeginRenderUnmasked();
		this->RenderScene(vVisibleRenderables, delta);
		this->RenderScene(vVisibleRenderablesStatic, delta);

		this->BeginRenderSpecial();
		this->RenderScene(vSpecialRenderables, delta);
		this->RenderScene(vSpecialRenderablesStatic, delta);
	this->End();

	return TRUE;
}

INLINE void IRenderer::RenderScene(const RenderableVector &vec, f32 delta) const
{
	ConstRenderableVectorIterator it = vec.begin();
	ConstRenderableVectorIterator end = vec.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		obj->Render(delta);
	}
}

INLINE void IRenderer::Culler() // FIXME: Culler(RenderableList, CullingOperation)
{
	vVisibleRenderables.clear();
	vVisibleRenderablesStatic.clear();

	ConstRenderableVectorIterator it = vRenderables.begin();
	ConstRenderableVectorIterator end = vRenderables.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		if (obj->IsVisible() && /*!obj->IsMasked() &&*/ !obj->IsMask())
			vVisibleRenderables.push_back(obj);
	}

	this->Sort(vVisibleRenderables);

	it = vRenderablesStatic.begin();
	end = vRenderablesStatic.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		if (obj->IsVisible() && !obj->IsMasked() && !obj->IsMask())
			vVisibleRenderablesStatic.push_back(obj);
	}

	this->Sort(vVisibleRenderablesStatic);
}

INLINE void IRenderer::FilterObjects()
{
	vMaskRenderables.clear();
	vMaskRenderablesStatic.clear();

	vMaskedRenderables.clear();
	vMaskedRenderablesStatic.clear();

	vSpecialRenderables.clear();
	vSpecialRenderablesStatic.clear();

	ConstRenderableVectorIterator it = vRenderables.begin();
	ConstRenderableVectorIterator end = vRenderables.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		if (obj->IsMask())
			vMaskRenderables.push_back(obj);

		if (obj->IsMasked())
			vMaskedRenderables.push_back(obj);

		if  (obj->IsSpecial())
			vSpecialRenderables.push_back(obj);
	}

	this->Sort(vMaskRenderables);
	this->Sort(vMaskedRenderables);
	this->Sort(vSpecialRenderables);

	it = vRenderablesStatic.begin();
	end = vRenderablesStatic.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		if (obj->IsMask())
			vMaskRenderablesStatic.push_back(obj);

		if (obj->IsMasked())
			vMaskedRenderablesStatic.push_back(obj);

		if  (obj->IsSpecial())
			vSpecialRenderablesStatic.push_back(obj);
	}

	this->Sort(vMaskRenderablesStatic);
	this->Sort(vMaskedRenderablesStatic);
	this->Sort(vSpecialRenderablesStatic);
}

INLINE void IRenderer::Sort(RenderableVector &vec)
{
#if !SEED_ENABLE_DEPTH_TEST
	std::sort(vec.begin(), vec.end(), IRenderableAscendingPrioritySort());
#else
	UNUSED(vec)
#endif
}

INLINE void IRenderer::Add(IRenderable *obj)
{
	ASSERT_NULL(obj);

	ConstRenderableVectorIterator p = std::find(vRenderables.begin(), vRenderables.end(), obj);
	if (p == vRenderables.end())
	{
		vRenderables.push_back(obj);
	}
}

INLINE void IRenderer::Remove(const IRenderable *obj)
{
	ASSERT_NULL(obj);

	RenderableVectorIterator p = std::find(vRenderables.begin(), vRenderables.end(), obj);
	if (p != vRenderables.end())
	{
		vRenderables.erase(p);
	}

	RenderableVector(vRenderables).swap(vRenderables);
}

INLINE void IRenderer::AddStatic(IRenderable *obj)
{
	ASSERT_NULL(obj);

	ConstRenderableVectorIterator p = std::find(vRenderablesStatic.begin(), vRenderablesStatic.end(), obj);
	if (p == vRenderablesStatic.end())
	{
		vRenderablesStatic.push_back(obj);
	}
}

INLINE void IRenderer::RemoveStatic(const IRenderable *obj)
{
	ASSERT_NULL(obj);

	RenderableVectorIterator p = std::find(vRenderablesStatic.begin(), vRenderablesStatic.end(), obj);
	if (p != vRenderablesStatic.end())
	{
		vRenderablesStatic.erase(p);
	}

	RenderableVector(vRenderablesStatic).swap(vRenderablesStatic);
}

INLINE void IRenderer::Clear()
{
	vRenderables.clear();
	RenderableVector().swap( vRenderables );
}

INLINE void IRenderer::ClearStatic()
{
	vRenderablesStatic.clear();
	RenderableVector().swap( vRenderablesStatic );
}


} // namespace

