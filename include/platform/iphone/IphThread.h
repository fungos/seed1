/*! \file IphThread.h
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/

#ifndef __IPH_THREAD_H__
#define __IPH_THREAD_H__


#ifdef _IPHONE_


#include "Defines.h"
#include "interface/IThread.h"


namespace Seed { namespace iPhone {


class Thread : public IThread
{
	public:
		Thread();
		virtual ~Thread();

		virtual void Create();
		virtual BOOL Run();


	private:
		Thread(const Thread &);
		Thread &operator=(const Thread &);


	private:
		void 	*pThread;
};


}} // namespace


#else // _IPHONE_

	#error "Include 'Thread.h' instead 'platform/iphone/IphThread.h' directly."

#endif // _IPHONE_
#endif // __IPH_THREAD_H__
