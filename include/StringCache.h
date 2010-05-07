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

/*! \file StringCache.h
	\author	Danny Angelo Carminati Grein
	\brief Static String Cache
*/

#ifndef __STRING_CACHE_H__
#define __STRING_CACHE_H__

#include "interface/IModule.h"
#include "Base.h"
#include "Enum.h"
#include "File.h"
#include "Singleton.h"

#include <vector>
#include <algorithm>

#define _S(x)	pStringCache->GetStringById(x)

namespace Seed {

/// StringCache
/**
Static string cache
*/
class SEED_CORE_API StringCache : public IModule
{
	SEED_SINGLETON_DECLARE(StringCache);
	public:
		const char *GetStringById(u32 fileId) const;
		u32 GetIdByString(const char *str) const;

		// IModule
		virtual BOOL Reset();
		virtual BOOL Initialize();
		virtual BOOL Shutdown();

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		void BuildStringTable();
		void DestroyStringTable();

	private:
		SEED_DISABLE_COPY(StringCache);

	private:
		File	stFile;
		u32		iStringCount;

		typedef char* 	CachedString;
		CachedString	*pStringTable;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(StringCache);
}

} // namespace

#endif // __STRING_CACHE_H__
