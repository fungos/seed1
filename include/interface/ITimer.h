/*! \file ITimer.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Timer class interface
*/

#ifndef __ITIMER_H__
#define __ITIMER_H__

#include "IModule.h"


namespace Seed {


/// Timer Interface
/**
Interface for working with Ticks and Milliseconds
*/
class ITimer : public IModule
{
	public:
		ITimer();
		virtual ~ITimer();

		virtual u64 GetMilliseconds() const;

		virtual void Sleep(u32 ms) const;

		// IModule
		virtual BOOL Reset();
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(ITimer);
};


} // namespace


#endif // __ITIMER_H__

