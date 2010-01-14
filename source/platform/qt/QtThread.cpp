/*! \file QtThread.cpp
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/


#ifdef _QT_

#include "platform/qt/QtThread.h"

#define TAG 	"[Thread] "


namespace Seed { namespace QT {


Thread::Thread()
	: iPriority(7)
{
}



Thread::~Thread()
{
	this->Destroy();
}


INLINE void Thread::Create(s32 priority)
{
	this->iPriority = priority;

	IThread::Create(priority);
	QThread::start(static_cast<QThread::Priority>(iPriority));
}


INLINE void Thread::Destroy()
{
	IThread::Destroy();
	QThread::terminate();
}


INLINE BOOL Thread::Run()
{
	IThread::Run();
	return TRUE;
}


void Thread::run()
{
	this->Run();
}


}} // namespace


#endif // _QT_
