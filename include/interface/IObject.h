/*! \file IObject.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Base Object class interface
*/

#ifndef __IOBJECT_H__
#define __IOBJECT_H__


#include "Defines.h"


namespace Seed {


/// Object Interface
/**
Interface for basic object
*/
class IObject
{
	public:
		IObject();
		virtual ~IObject();

		virtual const char *GetObjectName() const = 0;
		virtual int GetObjectType() const = 0;

	private:
		SEED_DISABLE_COPY(IObject);
};


} // namespace


#endif // __IOBJECT_H__
