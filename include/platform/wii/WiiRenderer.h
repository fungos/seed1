/*! \file WiiRenderer.h
	\author	Danny Angelo Carminati Grein
	\brief Rendering controller
*/

#ifndef __WII_RENDERER_H__
#define __WII_RENDERER_H__

#ifdef _WII_

#include "interface/IRenderer.h"
#include "Screen.h"

namespace Seed { namespace WII {

class Renderer : public IRenderer
{
	friend class Screen;

	public:
		Renderer();
		virtual ~Renderer();

		virtual void SetBufferMode(eBufferMode mode);
		virtual BOOL Reset();

		virtual void Begin() const;
		virtual void End() const;

	protected:
		Screen *screen;
		eBufferMode bufferMode;

	private:
		SEED_DISABLE_COPY(Screen);
		void InitGraphics();

	private:
		WiiFifoObj 	*fifoObj;
		void 		*fifoBuffer;
		void 		*pSShotBuffer;
};

}} // namespace

#else // _WII_
	#error "Include 'Renderer.h' instead 'platform/wii/WiiRenderer.h' directly."
#endif // _WII_
#endif // __WII_RENDERER_H__
