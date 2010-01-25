/*! \file IGameApp.cpp
	\author	Danny Angelo Carminati Grein
	\brief The real main for a game application
*/


#include "interface/IGameApp.h"
#include "Enum.h"
#include <stdlib.h>
#include <stdio.h>

namespace Seed {


IGameApp::IGameApp()
{
}

IGameApp::~IGameApp()
{
}

INLINE void IGameApp::Setup(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
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
	return FALSE;
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

