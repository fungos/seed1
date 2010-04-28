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

/*! \file IInputKeyboard.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Keyboard class interface
*/

#include "interface/IInputKeyboard.h"
#include "interface/IEventInputKeyboardListener.h"
#include "Log.h"
#include "Array.h"

#include <vector>
#include <algorithm>

namespace Seed {

#if defined(DEBUG)
const char *keyName[] = {
	"KeyNone", // 0
	"", // 1
	"", // 2
	"", // 3
	"", // 4
	"", // 5
	"", // 6
	"", // 7
	"KeyBackspace",
	"KeyTab",
	"", // 10
	"", // 11
	"KeyClear",
	"KeyReturn/KeyEnter",
	"", // 14
	"", // 15
	"", // 16
	"", // 17
	"", // 18
	"KeyPause",
	"", // 20
	"", // 21
	"", // 22
	"", // 23
	"", // 24
	"", // 25
	"", // 26
	"KeyEscape",
	"", // 28
	"", // 29
	"", // 30
	"", // 31
	// Ascii table 1 to 1
	"KeySpace",
	"KeyExclam",
	"KeyQuoteDouble",
	"KeyNumberSign",
	"KeyDollar",
	"KeyPercent",
	"KeyAmpersand",
	"KeyApostrophe",
	"KeyParenLeft",
	"KeyParenRight",
	"KeyAsterisk",
	"KeyPlus",
	"KeyComma",
	"KeyMinus",
	"KeyPeriod",
	"KeySlash",
	"Key0",
	"Key1",
	"Key2",
	"Key3",
	"Key4",
	"Key5",
	"Key6",
	"Key7",
	"Key8",
	"Key9",
	"KeyColon",
	"KeySemicolon",
	"KeyLess",
	"KeyEqual",
	"KeyGreater",
	"KeyQuestion",
	"", // @ 64
	// Both capital and lowercase chars are valid
	"KeyA",
	"KeyB",
	"KeyC",
	"KeyD",
	"KeyE",
	"KeyF",
	"KeyG",
	"KeyH",
	"KeyI",
	"KeyJ",
	"KeyK",
	"KeyL",
	"KeyM",
	"KeyN",
	"KeyO",
	"KeyP",
	"KeyQ",
	"KeyR",
	"KeyS",
	"KeyT",
	"KeyU",
	"KeyV",
	"KeyW",
	"KeyX",
	"KeyY",
	"KeyZ",

	"KeyBracketLeft",
	"KeyBackslash",
	"KeyBracketRight",
	"KeyCircum",
	"KeyUnderscore",
	"KeyQuoteLeft/KeyConsole",

	"KeyA", // a 97
	"KeyB",
	"KeyC",
	"KeyD",
	"KeyE",
	"KeyF",
	"KeyG",
	"KeyH",
	"KeyI",
	"KeyJ",
	"KeyK",
	"KeyL",
	"KeyM",
	"KeyN",
	"KeyO",
	"KeyP",
	"KeyQ",
	"KeyR",
	"KeyS",
	"KeyT",
	"KeyU",
	"KeyV",
	"KeyW",
	"KeyX",
	"KeyY",
	"KeyZ", // z 122

	"KeyBraceLeft",
	"KeyBar/KeyPipe",
	"KeyBraceRight",
	"KeyTilde",

	"KeyDelete", // 127

	"", // 128
	"",
	"", // 130
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 140
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 150
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 160
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 170
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 180
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 190
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 200
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 210
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 220
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 230
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 240
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 250
	"",
	"",
	"",
	"",
	"",

	"KeyPad0", // 256
	"KeyPad1",
	"KeyPad2",
	"KeyPad3",
	"KeyPad4",
	"KeyPad5",
	"KeyPad6",
	"KeyPad7",
	"KeyPad8",
	"KeyPad9",
	"KeyPadPeriod",
	"KeyPadDivide",
	"KeyPadMultiply",
	"KeyPadMinus",
	"KeyPadPlus",
	"KeyPadEnter",
	"KeyPadEquals",

	"KeyUp",
	"KeyDown",
	"KeyRight",
	"KeyLeft",
	"KeyInsert",
	"KeyHome",
	"KeyEnd",
	"KeyPageUp",
	"KeyPageDown",

	"KeyF1",
	"KeyF2",
	"KeyF3",
	"KeyF4",
	"KeyF5",
	"KeyF6",
	"KeyF7",
	"KeyF8",
	"KeyF9",
	"KeyF10",
	"KeyF11",
	"KeyF12",
	"KeyF13",
	"KeyF14",
	"KeyF15",

	"", // 297
	"", // 298
	"", // 299

	"KeyNumLock", // 300
	"KeyCapsLock",
	"KeyScrollLock",

	"KeyShiftRight",
	"KeyShiftLeft",
	"KeyControlRight",
	"KeyControlLeft",
	"KeyAltRight",
	"KeyAltLeft",
	"KeyMetaRight",
	"KeyMetaLeft",
	"KeySuperLeft",
	"KeySuperRight",
	"KeyAltGr",
	"KeyMenu",
	"", // 315

	"KeyPrintScreen",

};
#endif


IInputKeyboard::IInputKeyboard()
	: arKeyboardListeners()
{
}

IInputKeyboard::~IInputKeyboard()
{
}

/*
INLINE BOOL IInputKeyboard::IsKeyHold(u32 key) const
{
	return FALSE;
}

INLINE BOOL IInputKeyboard::IsKeyPressed(u32 key) const
{
	return FALSE;
}

INLINE BOOL IInputKeyboard::IsKeyReleased(u32 key) const
{
	return FALSE;
}
*/

INLINE void IInputKeyboard::AddKeyboardListener(IEventInputKeyboardListener *listener)
{
	ASSERT_NULL(listener);
	arKeyboardListeners.Add(listener);
}

INLINE void IInputKeyboard::RemoveKeyboardListener(IEventInputKeyboardListener *listener)
{
	ASSERT_NULL(listener);

	for (u32 i = 0; i < arKeyboardListeners.Size(); i++)
	{
		if (arKeyboardListeners[i] == listener)
		{
			arKeyboardListeners.Del(i);
			break;
		}
	}
}

INLINE void IInputKeyboard::SendEventKeyboardPress(const EventInputKeyboard *ev)
{
	ASSERT_NULL(ev);

#if defined(DEBUG)
	Dbg(">>>> Key Press: %s Modifier: 0x%04x", keyName[ev->GetKey().GetValue()], ev->GetModifier());
#endif

	for (u32 i = 0; i < arKeyboardListeners.Size(); i++)
	{
		ASSERT_NULL(arKeyboardListeners[i]);
		arKeyboardListeners[i]->OnInputKeyboardPress(ev);

		if (ev->IsConsumed())
			break;
	}
}

INLINE void IInputKeyboard::SendEventKeyboardRelease(const EventInputKeyboard *ev)
{
	ASSERT_NULL(ev);

#if defined(DEBUG)
	Dbg(">>>> Key Release: %s Modifier: 0x%04x", keyName[ev->GetKey().GetValue()], ev->GetModifier());
#endif

	for (u32 i = 0; i < arKeyboardListeners.Size(); i++)
	{
		ASSERT_NULL(arKeyboardListeners[i]);
		arKeyboardListeners[i]->OnInputKeyboardRelease(ev);

		if (ev->IsConsumed())
			break;
	}
}

} // namespace
