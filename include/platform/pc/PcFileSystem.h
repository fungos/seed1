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

/*! \file PcFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief FileSystem PC Implementation
*/

#ifndef __PC_FILESYSTEM_H__
#define __PC_FILESYSTEM_H__

#include "Defines.h"

#if defined(_PC_)

#include "MemoryManager.h"
#include "interface/IFileSystem.h"
#include "Singleton.h"

#define FILESYSTEM_DEFAULT_PATH	"data/"

namespace Seed { namespace PC {

class SEED_CORE_API FileSystem : public IFileSystem
{
	SEED_SINGLETON_DECLARE(FileSystem);
	public:
		virtual BOOL Open(const char *pFname, File *file, IMemoryPool *pool = pDefaultPool);
		virtual u32 GetLength() const;
		virtual void MakeDirectory(const char *dir) const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual void SetWorkDirectory(const char *dir);

	private:
		SEED_DISABLE_COPY(FileSystem);

		void *Read(const char *fname, IMemoryPool *pool, u32 *pLength);

	private:
		u32 iLastLength;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(FileSystem);
}

}} // namespace

#else // _PC_
	#error "Include 'FileSystem.h' instead 'platform/pc/PcFileSystem.h' directly."
#endif // _PC_
#endif // __PC_FILESYSTEM_H__
