/*! \file IUpdatable.h
	\author	Danny Angelo Carminati Grein
	\brief Defines an updatable class
*/


#ifndef __IUPDATABLE_H__
#define __IUPDATABLE_H__


#include "Defines.h"


namespace Seed {


//! Updatable interface
/**
Interface for objects that need be updated each frame.
*/
class IUpdatable
{
	public:
		IUpdatable();
		virtual ~IUpdatable();

		//! Method to be called each frame.
		/*!
			\return If update was sucessful
		 */
		virtual BOOL Update(f32 dt);


	private:
		SEED_DISABLE_COPY(IUpdatable);
};

} // namespace


#endif // __IUPDATABLE_H__
