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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "Defines.h"
#include "interface/IObject.h"
#include "Enum.h"

namespace Seed {

class SEED_CORE_API Configuration : public IObject
{
	public:
		Configuration();
		~Configuration();

		void SetVideoMode(u32 videoMode);
		u32 GetVideoMode() const;

		void SetResolution(u32 width, u32 height);
		u32 GetResolutionWidth() const;
		u32 GetResolutionHeight() const;

		void SetWorkingDirectory(const char *path);
		const char *GetWorkingDirectory() const;

		void SetFrameRate(eSystemFrameRate frameRate);
		eSystemFrameRate GetFrameRate() const;

		void SetApplicationTitle(const char *title);
		const char *GetApplicationTitle() const;

		void SetApplicationDescription(const char *desc);
		const char *GetApplicationDescription() const;

		void SetPlatformSimulation(ePlatformSimulation platform);
		ePlatformSimulation GetPlatformSimulation() const;
		
		void SetCanHaveMultipleInstances(BOOL enable);
		BOOL GetCanHaveMultipleInstances() const;

		void SetWarningMultipleInstances(BOOL warnUser);
		BOOL GetWarningMultipleInstances() const;

		void SetMemorySize(u32 memsize);
		u32 GetMemorySize() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(Configuration);

		// Screen
		u32	iVideoMode;
		u32 iResolutionWidth;
		u32 iResolutionHeight;

		// FileSystem
		const char *pcWorkingDirectory;

		// System
		BOOL	bMultipleInstances;
		BOOL	bWarningMultipleInstances;
		eSystemFrameRate iFrameRate;
		const char *pcTitle;
		const char *pcDescription;

		// Memory
		u32 iMemorySize;

		// General
		ePlatformSimulation iPlatformSimulation;
};

} // namespace

#endif // __CONFIGURATION_H__
