/*! \file Dictionary.h
	\author Danny Angelo Carminati Grein
	\brief String dictionary
*/

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__


#include "interface/IResource.h"
#include "interface/IObject.h"
#include "interface/IEventSystemListener.h"
#include "EventSystem.h"
#include "MemoryManager.h"
#include "SeedInit.h"
#include "File.h"
#include "MemoryManager.h"
#include "Enum.h"


#define _(id)			pDictionary->GetString(id)
#define _2(id, len)		pDictionary->GetString(id, len)


namespace Seed {


class Dictionary : public IModule, public IEventSystemListener
{
	public:
		Dictionary();
		virtual ~Dictionary();

		virtual const u16 *GetGlyphTable(u32 *outLen) const;
		virtual const u16 *GetString(u32 stringId, u32 *outLen = NULL) const;
		virtual u32 GetSize() const;

		virtual void OnSystemLanguageChanged(const EventSystem *ev);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	public:
		static Dictionary instance;

	protected:
		virtual void SetLanguage(Seed::eLanguage lang, IMemoryPool *pool = pDefaultPool);

	private:
		SEED_DISABLE_COPY(Dictionary);

	private:
		IMemoryPool	*pPool;
		File		stFile;
		eLanguage	iLang;
		u32			iTotalGlyphs;
		u32			iTotalStrings;
		const u16	*pGlyphs;
		const u16	*pStrings;
};


Dictionary *const pDictionary = &Dictionary::instance;


} // namespace


#endif // __DICTIONARY_H__
