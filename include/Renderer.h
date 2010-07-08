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

/*! \file Renderer.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderer class interface
*/

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Array.h"
#include "interface/IUpdatable.h"
#include "interface/IModule.h"

#include <vector>

#define RENDERER_MAX_SCENES 32

namespace Seed {

class ISceneObject;
class ISceneNode;
class ITexture;

/// Renderer Interface
/**
Rendering engine interface
*/
class SEED_CORE_API Renderer : public IUpdatable, public IModule
{
	typedef std::vector<ISceneNode *> NodeVector;
	typedef NodeVector::iterator NodeVectorIterator;
	typedef NodeVector::const_iterator ConstNodeVectorIterator;

	typedef std::vector<ISceneObject *> RenderableVector;
	typedef RenderableVector::iterator RenderableVectorIterator;
	typedef RenderableVector::const_iterator ConstRenderableVectorIterator;

	public:
		Renderer();
		virtual ~Renderer();

		virtual void Render();
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill = FALSE) const;

		virtual void Begin() const;
		virtual void End() const;

		virtual void Sort(RenderableVector &vec);
		virtual void Culler();

		void Add(ISceneNode *node);
		void Remove(ISceneNode *node);

		// IUpdatable
		//virtual BOOL Reset();
		virtual BOOL Update(f32 delta);

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		Array<ISceneNode *, RENDERER_MAX_SCENES> arScenes;
		RenderableVector vRenderables;
		RenderableVector vVisibleRenderables;

	private:
		void RenderObjects(const RenderableVector &vec) const;
		void PushChildNodes(ISceneNode *, NodeVector &vec);

		SEED_DISABLE_COPY(Renderer);
};

} // namespace

#endif // __RENDERER_H__
