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


class File
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

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [](void *);

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
