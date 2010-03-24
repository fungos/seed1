/*! \file IInput.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Input base interface
*/


#include "interface/IInput.h"

namespace Seed {

IInput::IInput()
{
}

IInput::~IInput()
{
}

INLINE BOOL IInput::IsPointer() const
{
	return FALSE;
}

INLINE BOOL IInput::IsJoystick() const
{
	return FALSE;
}

INLINE BOOL IInput::IsMotion() const
{
	return FALSE;
}

INLINE BOOL IInput::IsKeyboard() const
{
	return FALSE;
}

INLINE BOOL IInput::IsRequired() const
{
	return TRUE;
}

INLINE const char *IInput::GetObjectName() const
{
	return "Input";
}

} // namespace
