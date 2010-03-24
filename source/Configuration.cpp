#include "Configuration.h"


namespace Seed {


Configuration::Configuration()
	: iVideoMode(0)
	, iResolutionWidth(800)
	, iResolutionHeight(600)
	, pcWorkingDirectory(NULL)
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
