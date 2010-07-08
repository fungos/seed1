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

#define TAG "[GameApp] "

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
	Log(TAG "Shutting down...");
	pDictionary->Reset();
	cResourceManager.Reset();

	LeakReportPrint;

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
