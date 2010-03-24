#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "Defines.h"
#include "interface/IObject.h"
#include "Enum.h"

namespace Seed {

class Configuration : public IObject
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
