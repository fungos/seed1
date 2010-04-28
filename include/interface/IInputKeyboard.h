/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file IInputKeyboard.h
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Keyboard class interface
*/

#ifndef __IINPUT_KEYBOARD_H__
#define __IINPUT_KEYBOARD_H__

#include "interface/IInput.h"
#include "Array.h"

namespace Seed {

class EventInputKeyboard;
class IEventInputKeyboardListener;

/// Keyboard Input Interface
/**
Platforms that support Keyboard (physical) input must implement this interface.
*/
class SEED_CORE_API IInputKeyboard
{
	public:
		IInputKeyboard();
		virtual ~IInputKeyboard();
/*
		virtual BOOL IsKeyHold(u32 key) const;
		virtual BOOL IsKeyPressed(u32 key) const;
		virtual BOOL IsKeyReleased(u32 key) const;
*/
		virtual void AddKeyboardListener(IEventInputKeyboardListener *listener);
		virtual void RemoveKeyboardListener(IEventInputKeyboardListener *listener);

	protected:
		void SendEventKeyboardPress(const EventInputKeyboard *ev);
		void SendEventKeyboardRelease(const EventInputKeyboard *ev);

	protected:
		Array<IEventInputKeyboardListener *, 1000> arKeyboardListeners;

	private:
		SEED_DISABLE_COPY(IInputKeyboard);
};

} // namespace

#endif // __IINPUT_POINTER_H__
