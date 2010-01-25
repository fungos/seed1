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

#include <vector>
#include <algorithm>

#define _S(x)	pStringCache->GetStringById(x)


namespace Seed {


/// StringCache
/**
Static string cache
*/
class StringCache : public IModule
{
	public:
		StringCache();
		virtual ~StringCache();

		const char *GetStringById(u32 fileId) const;
		u32 GetIdByString(const char *str) const;

		// IModule
		virtual BOOL Reset();
		virtual BOOL Initialize();
		virtual BOOL Shutdown();

		// IObject
		virtual const char *GetObjectName() const;
		
	public:
		static StringCache instance;
		
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


StringCache *const pStringCache = &StringCache::instance;


} // namespace


#endif // __STRING_CACHE_H__
