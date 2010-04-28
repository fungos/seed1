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

/*! \file IModule.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Framework Base Module class interface
*/

#ifndef __IMODULE_H__
#define __IMODULE_H__

#include "Defines.h"
#include "interface/IObject.h"

namespace Seed {

/// Object Interface
/**
Interface for basic module
*/
class SEED_CORE_API IModule : public IObject
{
	public:
		IModule();
		virtual ~IModule();

		/// Initialize this module, it must initialize all module attributes.
		virtual BOOL Initialize();

		/// Reset all module attributes as a newly created object.
		virtual BOOL Reset();

		/// Terminate everything and deinitialize all dependency. Reset to before initialization state.
		virtual BOOL Shutdown();

		/// Disables this module
		virtual void Disable();

		/// Enabled this module
		virtual void Enable();

		/// Returns true if this module is enabled
		virtual BOOL IsEnabled() const;

		/// Check if the module is initialized
		BOOL IsInitialized() const;

		/// If this module is mandatory (it is a base subsystem or critical one)
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		BOOL bInitialized;
		BOOL bEnabled;

	private:
		SEED_DISABLE_COPY(IModule);
};

} // namespace

#endif // __IMODULE_H__
