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
class IRenderer : public IUpdatable, public IModule
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

		virtual void Update(const RenderableVector &vec) const;
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
		virtual BOOL Update(f32 dt);

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
