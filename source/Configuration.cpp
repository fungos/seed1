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

#include "Configuration.h"


namespace Seed {


Configuration::Configuration()
	: iVideoMode(0)
	, iResolutionWidth(800)
	, iResolutionHeight(600)
	, pcWorkingDirectory(NULL)
	, bMultipleInstances(FALSE)
	, bWarningMultipleInstances(FALSE)
	, iFrameRate(Seed::FrameRateLockAt60)
	, pcTitle(NULL)
	, pcDescription(NULL)
	, iMemorySize(20 * 1024 * 1024)
	, iPlatformSimulation(Seed::SimulateNothing)
{
}

Configuration::~Configuration()
{
}

INLINE void Configuration::SetVideoMode(u32 videoMode)
{
	this->iVideoMode = videoMode;
}

INLINE u32 Configuration::GetVideoMode() const
{
	return this->iVideoMode;
}

INLINE void Configuration::SetResolution(u32 width, u32 height)
{
	this->iResolutionWidth = width;
	this->iResolutionHeight = height;
}

INLINE u32 Configuration::GetResolutionWidth() const
{
	return this->iResolutionWidth;
}

INLINE u32 Configuration::GetResolutionHeight() const
{
	return this->iResolutionHeight;
}

INLINE void Configuration::SetWorkingDirectory(const char *path)
{
	this->pcWorkingDirectory = path;
}

INLINE const char *Configuration::GetWorkingDirectory() const
{
	return this->pcWorkingDirectory;
}

INLINE void Configuration::SetFrameRate(eSystemFrameRate frameRate)
{
	this->iFrameRate = frameRate;
}

INLINE eSystemFrameRate Configuration::GetFrameRate() const
{
	return this->iFrameRate;
}

INLINE void Configuration::SetApplicationTitle(const char *title)
{
	this->pcTitle = title;
}

INLINE const char *Configuration::GetApplicationTitle() const
{
	return this->pcTitle;
}

INLINE void Configuration::SetApplicationDescription(const char *desc)
{
	this->pcDescription = desc;
}

INLINE const char *Configuration::GetApplicationDescription() const
{
	return this->pcDescription;
}

INLINE void Configuration::SetCanHaveMultipleInstances(BOOL enable)
{
	bMultipleInstances = enable;
}

INLINE BOOL Configuration::GetCanHaveMultipleInstances() const
{
	return bMultipleInstances;
}

INLINE void Configuration::SetWarningMultipleInstances(BOOL warnUser)
{
	bWarningMultipleInstances = warnUser;
}

INLINE BOOL Configuration::GetWarningMultipleInstances() const
{
	return bWarningMultipleInstances;
}

INLINE void Configuration::SetMemorySize(u32 memsize)
{
	this->iMemorySize = memsize;
}

INLINE u32 Configuration::GetMemorySize() const
{
	return this->iMemorySize;
}

INLINE void Configuration::SetPlatformSimulation(ePlatformSimulation platform)
{
	switch (platform)
	{
		case SimulateWii:
		{
			this->iResolutionWidth = 480;
			this->iResolutionHeight = 640;
			this->iMemorySize = 20 * 1024 * 1024; // 22MB MEM1
		}
		break;

		case SimulatePsp:
		{
			this->iResolutionWidth = 480;
			this->iResolutionHeight = 272;
		}
		break;

		case SimulateN900:
		{
			this->iResolutionWidth = 800;
			this->iResolutionHeight = 480;
		}

		case SimulateNothing:
		default:
		break;
	}

	this->iPlatformSimulation = platform;
}

INLINE ePlatformSimulation Configuration::GetPlatformSimulation() const
{
	return this->iPlatformSimulation;
}

INLINE const char *Configuration::GetObjectName() const
{
	return "Configuration";
}

INLINE int Configuration::GetObjectType() const
{
	return Seed::ObjectConfiguration;
}


} // namespace
