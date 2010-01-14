/*! \file IphRenderer.h
	\author	Danny Angelo Carminati Grein
	\brief Renderer Iphone Implementation
*/

#ifndef __IPH_RENDERER_H__
#define __IPH_RENDERER_H__

#ifdef _IPHONE_

#include "interface/IRenderer.h"
#include <vector>

namespace Seed { namespace iPhone {


class Renderer : public IRenderer
{
	public:
		Renderer();
		virtual ~Renderer();

		virtual void SetBufferMode(eBufferMode mode);
		virtual BOOL Reset();

		virtual void Begin() const;
		virtual void End() const;


	protected:
		friend class Screen;

		Screen *myScreen;
		eBufferMode iBufferMode;


	private:
		Renderer(const Renderer &);
		Renderer &operator=(const Renderer &);
};


}} // namespace


#else // _IPHONE_

	#error "Include 'Renderer.h' instead 'platform/iphone/IphRenderer.h' directly."

#endif // _IPHONE_
#endif // __IPH_RENDERER_H__
