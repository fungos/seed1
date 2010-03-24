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
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::SelectTexture(u32 texId)
{
	UNUSED(texId);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::UploadData(void *userData)
{
	UNUSED(userData);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::Begin() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::End() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::BeginRenderMask() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::BeginRenderMasked() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::BeginRenderUnmasked() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer::BeginRenderSpecial() const
{
	SEED_ABSTRACT_METHOD;
}

BOOL IRenderer::Update(f32 delta)
{
	if (!IModule::IsEnabled())
		return FALSE;

	this->Update(vRenderables, delta);
	this->Update(vRenderablesStatic, delta);

	return TRUE;
}

INLINE void IRenderer::Update(const RenderableVector &vec, f32 delta) const
{
	ConstRenderableVectorIterator it = vec.begin();
	ConstRenderableVectorIterator end = vec.end();

	for (; it != end; ++it)
	{
		IRenderable *obj = const_cast<IRenderable *>(*it);
		ASSERT_NULL(obj);

		obj->Update(delta);
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
	RenderableVector().swap(vRenderables);
}

INLINE void IRenderer::ClearStatic()
{
	vRenderablesStatic.clear();
	RenderableVector().swap(vRenderablesStatic);
}

} // namespace
