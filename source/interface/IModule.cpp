/*! \file IModule.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Framework Base Module class interface
*/


#include "interface/IModule.h"
#include "Enum.h"


namespace Seed {


IModule::IModule()
	: bInitialized(FALSE)
	, bEnabled(TRUE)
{
}

IModule::~IModule()
{
	this->Shutdown();
}

INLINE BOOL IModule::Initialize()
{
	this->bInitialized = TRUE;
	return TRUE;
}

INLINE BOOL IModule::Reset()
{
	return TRUE;
}

INLINE BOOL IModule::Shutdown()
{
	this->bInitialized = FALSE;
	return TRUE;
}

INLINE void IModule::Disable()
{
	this->bEnabled = FALSE;
}

INLINE void IModule::Enable()
{
	this->bEnabled = TRUE;
}

INLINE BOOL IModule::IsEnabled() const
{
	return this->bEnabled;
}

INLINE BOOL IModule::IsInitialized() const
{
	return this->bInitialized;
}

INLINE BOOL IModule::IsRequired() const
{
	return FALSE;
}

INLINE const char *IModule::GetObjectName() const
{
	return "IModule";
}

INLINE int IModule::GetObjectType() const
{
	return Seed::ObjectInterfaceModule;
}

} // namespace
