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
#include "Singleton.h"

#define _(id)			pDictionary->GetString(id)
#define _2(id, len)		pDictionary->GetString(id, len)

namespace Seed {

class SEED_CORE_API Dictionary : public IModule, public IEventSystemListener
{
	SEED_SINGLETON_DECLARE(Dictionary);

	public:
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

//extern "C" {
//SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Dictionary);
//}

#define pDictionary Dictionary::GetInstance()

} // namespace

#endif // __DICTIONARY_H__
