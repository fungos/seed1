/*! \file EventInputPointer.h
	\author	Danny Angelo Carminati Grein
	\brief A event from an input pointer
*/

#ifndef __EVENT_INPUT_POINTER_H__
#define __EVENT_INPUT_POINTER_H__


#include "interface/IEventInput.h"


namespace Seed {


class EventInputPointer : public IEventInput
{
	public:
		EventInputPointer(u32 j, u32 pressed, u32 hold, u32 released, f32 x, f32 y);
		virtual ~EventInputPointer();

		u32 GetJoystick() const;
		u32 GetPressed()  const;
		u32 GetReleased() const;
		u32 GetHold() 	 const;
		f32 GetX() 		 const;
		f32 GetY() 		 const;

		// IObject
		const char *GetObjectName() const;

	protected:
		u32 iJoystick;
		u32 iPressed;
		u32 iHold;
		u32 iReleased;
		f32 fX;
		f32 fY;

	private:
		SEED_DISABLE_COPY(EventInputPointer);
};


} // namespace


#endif // __EVENT_INPUT_POINTER_H__

