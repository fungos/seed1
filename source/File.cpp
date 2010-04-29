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

/*! \file EventSystem.cpp
	\author	Everton Fernando Patitucci da Silva
	\brief A system event
*/

#include "Defines.h"
#include "MemoryManager.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"

#define TAG		"[File] "

namespace Seed {

File::File()
	: pName(NULL)
	, pData(NULL)
	, pPool(NULL)
	, iSize(0)
	, bPackaged(FALSE)
{
}

File::File(const char *filename, IMemoryPool *pool)
	: pName(filename)
	, pData(NULL)
	, pPool(pool)
	, iSize(0)
	, bPackaged(FALSE)
{
	pFileSystem->Open(filename, this, pool);
	// FIXME:
	// cyclic dependency not good, not good
	// file don't need to know about filesystem.. but, this will make the usage simplier.
}

File::~File()
{
	this->Close();
}

void File::Close()
{
	if (!bPackaged && pData)
	{
		pMemoryManager->Free(const_cast<void *>(pData), pPool);
	}

	pName = NULL;
	pData = NULL;
	pPool = NULL;
	iSize = 0;
	bPackaged = FALSE;
}

INLINE void File::SetSize(u32 size)
{
	iSize = size;
}

INLINE u32 File::GetSize() const
{
	return iSize;
}

INLINE void File::SetData(const void *data)
{
	pData = data;
}

INLINE const void *File::GetData() const
{
	return pData;
}

INLINE IMemoryPool *File::GetPool() const
{
	return pPool;
}

INLINE const char *File::GetName() const
{
	return pName;
}

INLINE void File::SetName(const char *name)
{
	pName = name;
}

SEED_DISABLE_INSTANCING_IMPL(File);

} // namespace
