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
class IModule : public IObject
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
