/*! \file EventFileSystem.h
	\author	Rafael Eduardo Gonchor
	\brief A file system event
*/


#ifndef __EVENT_FILESYSTEM_H__
#define __EVENT_FILESYSTEM_H__


#include "interface/IEvent.h"


namespace Seed {


class EventFileSystem : public IEvent
{
	public:
		EventFileSystem();
		virtual ~EventFileSystem();

		virtual const char *GetObjectName() const;

		void SetFile(const char *pFilename);
		const char *GetFile();

	private:
		const char *pFilename;

	private:
		SEED_DISABLE_COPY(EventFileSystem);
};


} // namespace


#endif // __EVENT_FILESYSTEM_H__
