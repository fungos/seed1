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

/*! \file EventSystem.h
	\author	Everton Fernando Patitucci da Silva
	\brief A system event
*/

#ifndef __FILE_H__
#define __FILE_H__

#include "Defines.h"
#include "MemoryManager.h"
#include "FileSystem.h"

namespace Seed {

class IFileSystem;
class IMemoryPool;
class Package;

class SEED_CORE_API File
{
	friend class IFileSystem;
	friend class FSNS::FileSystem;
	friend class Package; // FIXME: ROM Unload bug.

	public:
		File();
		File(const char *filename, IMemoryPool *pool = pDefaultPool);
		virtual ~File();

		void Close();

		IMemoryPool *GetPool() const;
		u32 GetSize() const;
		const void *GetData() const;
		const char *GetName() const;

		void SetSize(u32 size);
		void SetData(const void *data);
		void SetName(const char *name);

		SEED_DISABLE_INSTANCING;

	protected:
		const char	*pName;
		const void 	*pData;
		IMemoryPool *pPool;

		u32			iSize;
		BOOL		bPackaged;

	private:
		SEED_DISABLE_COPY(File);
};

} // namespace

#endif // __FILE_H__
