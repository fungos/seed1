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

/*! \file Renderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer implementation
*/

#include "Renderer.h"
#include "Log.h"
#include "interface/ISceneNode.h"
#include "RendererDevice.h"
#include "Profiler.h"

#include <algorithm>
#include <vector>

namespace Seed {

Renderer::Renderer()
	: vRenderables()
	//, vVisibleRenderables()
{
}

Renderer::~Renderer()
{
	vRenderables.clear();
	RenderableVector().swap(vRenderables);

	//vVisibleRenderables.clear();
	//RenderableVector().swap(vVisibleRenderables);
}

void Renderer::PushChildNodes(ISceneNode *node, NodeVector &v)
{
	for (u32 i = 0; i < node->Size(); i++)
	{
		ISceneObject *obj = node->GetChildAt(i);
		if (obj && obj->IsNode() && obj->IsVisible())
		{
			v.push_back(static_cast<ISceneNode *>(obj));
			this->PushChildNodes(static_cast<ISceneNode *>(obj), v);
		}
	}
}

BOOL Renderer::Update(f32 dt)
{
	UNUSED(dt);
	SEED_FUNCTION_PROFILER;

	if (!IModule::IsEnabled())
		return FALSE;

	vRenderables.clear();

	NodeVector v;
	for (u32 i = 0; i < arScenes.Size(); i++)
	{
		ISceneNode *node = arScenes[i];
		v.push_back(node);
		this->PushChildNodes(node, v);
	}

	NodeVectorIterator it = v.begin();
	NodeVectorIterator end = v.end();
	for (; it != end; ++it)
	{
		ISceneNode *node = (*it);

		for (u32 i = 0; i < node->Size(); i++)
		{
			ISceneObject *obj = node->GetChildAt(i);
			if (obj->IsVisible())
				vRenderables.push_back(obj);
		}
	}

	return TRUE;
}

INLINE void Renderer::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	UNUSED(fill);
	SEED_ABSTRACT_METHOD;
}

void Renderer::Render()
{
	SEED_FUNCTION_PROFILER;
	if (pRendererDevice && pRendererDevice->IsEnabled() && IModule::IsEnabled())
	{
		this->Culler();

		this->Begin();
			this->RenderObjects(vRenderables); //vVisibleRenderables);
		this->End();
	}
}

INLINE void Renderer::RenderObjects(const RenderableVector &vec) const
{
	ConstRenderableVectorIterator it = vec.begin();
	ConstRenderableVectorIterator end = vec.end();

	for (; it != end; ++it)
	{
		ISceneObject *obj = const_cast<ISceneObject *>(*it);
		ASSERT_NULL(obj);

		obj->Render();
	}
}

// FIXME: Culler(SceneNode, CullingOperation)
INLINE void Renderer::Culler()
{
	SEED_FUNCTION_PROFILER;
	this->Sort(vRenderables);
/*
	vVisibleRenderables.clear();

	ConstRenderableVectorIterator it = vRenderables.begin();
	ConstRenderableVectorIterator end = vRenderables.end();

	for (; it != end; ++it)
	{
		ISceneObject *obj = const_cast<ISceneObject *>(*it);
		ASSERT_NULL(obj);

		if (obj->IsVisible())
			vVisibleRenderables.push_back(obj);
	}

	this->Sort(vVisibleRenderables);
*/
}

INLINE void Renderer::Sort(RenderableVector &vec)
{
#if !SEED_ENABLE_DEPTH_TEST
	std::sort(vec.begin(), vec.end(), ISceneObjectAscendingPrioritySort());
#else
	UNUSED(vec)
#endif
}

INLINE void Renderer::Begin() const
{
	pRendererDevice->Begin();
}

INLINE void Renderer::End() const
{
	pRendererDevice->End();
}

INLINE void Renderer::Add(ISceneNode *node)
{
	arScenes.Add(node);
}

INLINE void Renderer::Remove(ISceneNode *node)
{
	arScenes.Remove(node);
}

INLINE const char *Renderer::GetObjectName() const
{
	return "Renderer";
}

} // namespace
