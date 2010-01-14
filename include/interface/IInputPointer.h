/*! \file IInputPointer.h
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Pointer class interface
*/

#ifndef __IINPUT_POINTER_H__
#define __IINPUT_POINTER_H__

#include "interface/IInput.h"
#include "Array.h"


namespace Seed {


class EventInputPointer;
class IEventInputPointerListener;


/// Pointer Input Interface
/**
Platforms that support Pointer (mouse like) input must implement this interface.
*/
class IInputPointer
{
	public:
		IInputPointer();
		virtual ~IInputPointer();

		virtual BOOL IsHold(u32 button, u16 joystick = 0) const;
		virtual BOOL IsPressed(u32 button, u16 joystick = 0) const;
		virtual BOOL IsReleased(u32 button, u16 joystick = 0) const;
		virtual BOOL IsEnabled(u16 joystick = 0) const;

		virtual f32 GetX(u16 joystick = 0) const = 0;
		virtual f32 GetY(u16 joystick = 0) const = 0;

		virtual u32 GetSensitivity(u16 joystick = 0) const = 0;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0) = 0;
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const = 0;
		virtual u32 GetMaximumPointers() const = 0;

		virtual f32 GetRelativeX(u16 joystick = 0) const = 0;
		virtual f32 GetRelativeY(u16 joystick = 0) const = 0;
		virtual f32 GetDistance(u16 joystick = 0) const = 0;
		virtual f32 GetOrientation(u16 joystick = 0) const = 0;

		void AddPointerListener(IEventInputPointerListener *listener);
		void RemovePointerListener(IEventInputPointerListener *listener);

	protected:
		void SendEventPointerEnable(const EventInputPointer *ev);
		void SendEventPointerDisable(const EventInputPointer *ev);
		void SendEventPointerMove(const EventInputPointer *ev);
		void SendEventPointerPress(const EventInputPointer *ev);
		void SendEventPointerRelease(const EventInputPointer *ev);

	protected:
		Array<IEventInputPointerListener *, 1000> arPointerListeners;

	private:
		SEED_DISABLE_COPY(IInputPointer);
};


} // namespace


#endif // __IINPUT_POINTER_H__

