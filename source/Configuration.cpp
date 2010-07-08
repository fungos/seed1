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
	: bDebugSprite(FALSE)
	, bDebugButton(FALSE)
	, bDebugText(FALSE)
	, bDebugContainer(FALSE)
	, nVideoMode(Video_AutoDetect)
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
	, iRendererDeviceType(Seed::RendererDeviceOpenGL14)
{
}

Configuration::~Configuration()
{
}

INLINE void Configuration::SetVideoMode(eVideoMode videoMode)
{
	nVideoMode = videoMode;
}

INLINE eVideoMode Configuration::GetVideoMode() const
{
	return nVideoMode;
}
/*
INLINE void Configuration::SetResolution(u32 width, u32 height)
{
	iResolutionWidth = width;
	iResolutionHeight = height;
}
*/
INLINE u32 Configuration::GetResolutionWidth() const
{
	return iResolutionWidth;
}

INLINE u32 Configuration::GetResolutionHeight() const
{
	return iResolutionHeight;
}

INLINE void Configuration::SetWorkingDirectory(const char *path)
{
	pcWorkingDirectory = path;
}

INLINE const char *Configuration::GetWorkingDirectory() const
{
	return pcWorkingDirectory;
}

INLINE void Configuration::SetFrameRate(eSystemFrameRate frameRate)
{
	iFrameRate = frameRate;
}

INLINE eSystemFrameRate Configuration::GetFrameRate() const
{
	return iFrameRate;
}

INLINE void Configuration::SetApplicationTitle(const char *title)
{
	pcTitle = title;
}

INLINE const char *Configuration::GetApplicationTitle() const
{
	return pcTitle;
}

INLINE void Configuration::SetApplicationDescription(const char *desc)
{
	pcDescription = desc;
}

INLINE const char *Configuration::GetApplicationDescription() const
{
	return pcDescription;
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
	iMemorySize = memsize;
}

INLINE u32 Configuration::GetMemorySize() const
{
	return iMemorySize;
}

INLINE void Configuration::SetPlatformSimulation(ePlatformSimulation platform)
{
	switch (platform)
	{
		case SimulateWii:
		{
			iResolutionWidth = 480;
			iResolutionHeight = 640;
			iMemorySize = 20 * 1024 * 1024; // 22MB MEM1
		}
		break;

		case SimulatePsp:
		{
			iResolutionWidth = 480;
			iResolutionHeight = 272;
		}
		break;

		case SimulateN900:
		{
			iResolutionWidth = 800;
			iResolutionHeight = 480;
		}

		case SimulateNothing:
		default:
		break;
	}

	iPlatformSimulation = platform;
}

INLINE ePlatformSimulation Configuration::GetPlatformSimulation() const
{
	return iPlatformSimulation;
}

INLINE void Configuration::SetRendererDeviceType(eRendererDeviceType deviceType)
{
	iRendererDeviceType = deviceType;
}

INLINE eRendererDeviceType Configuration::GetRendererDeviceType() const
{
	return iRendererDeviceType;
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
