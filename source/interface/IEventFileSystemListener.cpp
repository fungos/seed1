/*! \file IEventFileSystemListener.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the event file system listener class interface
*/

#include "interface/IEventFileSystemListener.h"
#include "Defines.h"
#include "Log.h"

namespace Seed {

IEventFileSystemListener::IEventFileSystemListener()
{
}

IEventFileSystemListener::~IEventFileSystemListener()
{
}

void IEventFileSystemListener::OnFileSystemFatalError(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

void IEventFileSystemListener::OnFileSystemWrongDiskError(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

void IEventFileSystemListener::OnFileSystemNoDiskError(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

void IEventFileSystemListener::OnFileSystemRetryError(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

void IEventFileSystemListener::OnFileSystemOperationResumed(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

void IEventFileSystemListener::OnFileSystemLoadCompleted(const EventFileSystem *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

} // namespace
