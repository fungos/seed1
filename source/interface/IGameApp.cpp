/*! \file IGameApp.cpp
	\author	Danny Angelo Carminati Grein
	\brief The real main for a game application
*/

#include "interface/IGameApp.h"
#include "Dictionary.h"
#include "Enum.h"
#include <stdlib.h>
#include <stdio.h>
#include "Log.h"

namespace Seed {

IGameApp::IGameApp()
	: cConfig()
	, cResourceManager("IGameApp")
{
}

IGameApp::~IGameApp()
{
}

INLINE BOOL IGameApp::Shutdown()
{
	pDictionary->Reset();
	glResourceManager.Reset();

	return TRUE;
}

INLINE void IGameApp::Setup(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
	SEED_ABSTRACT_METHOD;
}

INLINE void IGameApp::WriteOut(const char *msg)
{
	fprintf(stdout, "%s\n", msg);
}

INLINE void IGameApp::WriteErr(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

INLINE void IGameApp::WriteDbg(const char *msg)
{
	fprintf(stdout, "%s\n", msg);
}

INLINE BOOL IGameApp::HasError() const
{
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE const Configuration *IGameApp::GetConfiguration() const
{
	return &cConfig;
}

INLINE ResourceManager *IGameApp::GetResourceManager()
{
	return &cResourceManager;
}

INLINE const char *IGameApp::GetObjectName() const
{
	return "IGameApp";
}

INLINE int IGameApp::GetObjectType() const
{
	return Seed::ObjectInterfaceGameApp;
}

} // namespace
