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
class SEED_CORE_API IInput : public IModule, public IUpdatable
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
