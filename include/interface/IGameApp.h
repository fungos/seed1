/*! \file IGameApp.h
	\author	Danny Angelo Carminati Grein
	\brief The real main for a game application
*/

#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "IUpdatable.h"
#include "IModule.h"
#include "Configuration.h"
#include "ResourceManager.h"

namespace Seed {

/// Game Application module
/**
Game application interface. This is responsible for pre-initialization setup (being deprecated soon) and Seed modules configurations.
You can reimplement WriteOut, WriteErr and WriteDbg to a better suited output of your platform.
*/
class IGameApp : public IUpdatable, public IModule
{
	public:
		IGameApp();
		virtual ~IGameApp();

		virtual BOOL Shutdown();

		/// Setup must be called before Seed::Initialize - it will perform per-module configurations.
		/**
		\param argc Command line parameter count. Just pass what you receive in your main.
		\param argv Command line parameter values. Just pass what you receive in your main.
		*/
		virtual void Setup(int argc, char **argv);

		/// Print output level string
		virtual void WriteOut(const char *msg);

		/// Print error level string
		virtual void WriteErr(const char *msg);

		/// Print debug level string
		virtual void WriteDbg(const char *msg);

		/// Get user configuration
		const Configuration *GetConfiguration() const;

		/// Get user resource manager
		ResourceManager *GetResourceManager();

		/// Check whatever an system error ocurred. This is very important for multiplatform support (eg. errors can happend when reading disc within Nintendo Wii)
		virtual BOOL HasError() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(IGameApp);

		Configuration	cConfig;
		ResourceManager	cResourceManager;
};

} // namespace

#endif // __GAME_APP_H__
