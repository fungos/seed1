/*! \file QtThread.h
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/

#ifndef __QT_THREAD_H__
#define __QT_THREAD_H__


#ifdef _QT_


#include "Defines.h"
#include "interface/IThread.h"

#include <QThread>

namespace Seed { namespace QT {


class Thread : public IThread, public QThread
{
	public:
		Thread();
		virtual ~Thread();

		virtual void Create(s32 priority = 7);
		virtual void Destroy();
		virtual BOOL Run();

	protected:
		virtual void run();

	private:
		SEED_DISABLE_COPY(Thread);

	private:
		s32 iPriority;
};


}} // namespace


#else // _QT_

	#error "Include 'Thread.h' instead 'platform/qt/QtThread.h' directly."

#endif // _QT_
#endif // __QT_THREAD_H__
