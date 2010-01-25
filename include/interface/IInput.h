/*! \file IInput.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Input base interface
*/


#ifndef __IINPUT_H__
#define __IINPUT_H__

#include "IModule.h"
#include "IUpdatable.h"
#include "Enum.h"


namespace Seed {


class IEventInputMotionListener;
class IEventInputKeyboardListener;
class IEventInputPointerListener;


/// Abstract Input Interface
/**
All kind of inputs must implement this interface.
*/
class IInput : public IModule, public IUpdatable
{
	public:
		IInput();
		virtual ~IInput();

		virtual BOOL IsPointer() const;
		virtual BOOL IsMotion() const;
		virtual BOOL IsJoystick() const;
		virtual BOOL IsKeyboard() const;

		// IModule
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		virtual eInputButton GetButtonCode(u32 button) const = 0;
		virtual u32 ConvertButtonFlags(u32 flags) = 0;

	private:
		SEED_DISABLE_COPY(IInput);
};


} // namespace


#endif // __IINPUT_H__
