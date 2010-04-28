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

/*! \file IRenderer.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderer class interface
*/

#ifndef __IRENDERER_H__
#define __IRENDERER_H__

#include "IUpdatable.h"
#include "IModule.h"

#include <vector>

namespace Seed {

class IRenderable;

/// Renderer Interface
/**
Rendering engine interface
*/
class SEED_CORE_API IRenderer : public IUpdatable, public IModule
{
	typedef std::vector<IRenderable *> RenderableVector;
	typedef RenderableVector::iterator RenderableVectorIterator;
	typedef RenderableVector::const_iterator ConstRenderableVectorIterator;

	public:
		enum eBufferMode
		{
			ZBufferMode, /*!< ZBuffer mode */
			WBufferMode	 /*!< WBuffer mode */
		};

	public:
		IRenderer();
		virtual ~IRenderer();

		virtual void Update(const RenderableVector &vec, f32 delta) const;
		virtual BOOL Render(f32 delta);

		/// Set the buffer mode for this rendering engine
		virtual void SetBufferMode(eBufferMode mode);

		virtual void SelectTexture(u32 texId);
		virtual void UploadData(void *userData);

		virtual void Begin() const;
		virtual void End() const;
		virtual void RenderScene(const RenderableVector &vec, f32 delta) const;

		virtual void BeginRenderMask() const;
		virtual void BeginRenderMasked() const;
		virtual void BeginRenderUnmasked() const;
		virtual void BeginRenderSpecial() const;

		virtual void Sort(RenderableVector &vec);
		virtual void Culler();

		void Add(IRenderable *obj);
		void Remove(const IRenderable *obj);
		void Clear();

		void AddStatic(IRenderable *obj);
		void RemoveStatic(const IRenderable *obj);
		void ClearStatic();

		// IUpdatable
		virtual BOOL Update(f32 delta);

	protected:
		RenderableVector vRenderables;
		RenderableVector vRenderablesStatic;

		RenderableVector vVisibleRenderables;
		RenderableVector vVisibleRenderablesStatic;

		RenderableVector vMaskRenderables;
		RenderableVector vMaskRenderablesStatic;

		RenderableVector vMaskedRenderables;
		RenderableVector vMaskedRenderablesStatic;

		RenderableVector vSpecialRenderables;
		RenderableVector vSpecialRenderablesStatic;

		void FilterObjects();

	private:
		SEED_DISABLE_COPY(IRenderer);
};

} // namespace

#endif // __IRENDERER_H__
