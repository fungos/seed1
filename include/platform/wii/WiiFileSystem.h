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

/*! \file WiiFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief File system initialization
*/

#ifndef __WII_FILESYSTEM_H__
#define __WII_FILESYSTEM_H__

#if defined(_WII_)

#include "interface/IFileSystem.h"
#include "MemoryManager.h"
#include "Singleton.h"

#define FILESYSTEM_DEFAULT_PATH	"/data/"
#define FILESYSTEM_DEFAULT_PACK "data.arc"

namespace Seed { namespace WII {

class FileSystem : public IFileSystem
{
	SEED_SINGLETON_DECLARE(FileSystem);
	public:
		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual BOOL Open(const char *pFname, File *file, IMemoryPool *pool = pDefaultPool);
		//virtual void *Alloc(const char *pFname, u32 *fileLen, IMemoryPool *pool = pDefaultPool);

	private:
		SEED_DISABLE_COPY(FileSystem);

		BOOL ChangeDir(const char *dirName);
		void *Read(const char *fname, IMemoryPool *pool, u32 *pLength);
		void *Read(s32 entry, IMemoryPool *pool);

	private:
		s32 entryNumber;
		void *packageStart;
		BOOL bUsePackage;
		WiiHandle package;
		u32 lastLength;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(FileSystem);
}

}} // namespace

#else // _WII_
	#error "Include 'FileSystem.h' instead 'platform/wii/WiiFileSystem.h' directly."
#endif // _WII_
#endif // __WII_FILESYSTEM_H__
