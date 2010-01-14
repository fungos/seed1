/*! \file IEventFileSystemListener.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the event file system listener class interface
*/


#ifndef __IEVENTFILESYSTEMLISTENER_H__
#define __IEVENTFILESYSTEMLISTENER_H__

#include "IEventListener.h"
#include "Defines.h"


namespace Seed {


class EventFileSystem;


class IEventFileSystemListener : public IEventListener
{
	public:
		IEventFileSystemListener();
		virtual ~IEventFileSystemListener();

		virtual void OnFileSystemFatalError(const EventFileSystem *ev);
		virtual void OnFileSystemWrongDiskError(const EventFileSystem *ev);
		virtual void OnFileSystemNoDiskError(const EventFileSystem *ev);
		virtual void OnFileSystemRetryError(const EventFileSystem *ev);
		virtual void OnFileSystemOperationResumed(const EventFileSystem *ev);
		virtual void OnFileSystemLoadCompleted(const EventFileSystem *ev);

	private:
		SEED_DISABLE_COPY(IEventFileSystemListener);
};


} // namespace



#endif // __IEVENTFILESYSTEMLISTENER_H__
