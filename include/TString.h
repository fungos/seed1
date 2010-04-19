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

#ifndef __TSTRING_H__
#define __TSTRING_H__

#include "Defines.h"

namespace Seed {

class String
{
	public:
		String();
		explicit String(const WideString newStr);
		explicit String(const String &str);
		virtual ~String();
		
		String &Set(const WideString newStr);
		String &Reset();
		String &Clear();
		String &Release();
		
		// Size modifiers
		// All methods return this
		String &Set();
		String &Set(const u16 *paramName, const u16 *paramVal);
		String &Set(const u16 *paramName, const char *paramVal);
		String &Set(const u16 *paramName, u32 paramVal);
		String &Set(const u16 *paramName, f32 paramVal);
		String &Set(const u16 *paramName, bool paramVal);
		String &Set(const u16 *paramName, const char paramVal);
		String &Set(const u16 *paramName, const u16 paramVal);
		String &Set(const u16 *paramName, String *paramVal);
		
		void SubString(u32 index, u32 size);
		void Append(WideChar chr);
		//void Append(const u16 *str);
		void Append(const String *pString);

		// Common string functions
		u32 Length() const;
		BOOL Equals(const u16 *str) const;
		BOOL Equals(const String *pString) const;

		static u32 Length(const WideString str);
		static BOOL Equals(const WideString str1, const WideString str2);

		const WideString GetData() const; // DO NOT USE!

		String &operator=(const String &string);

	private:
		void Cut(u32 index, u32 size);
		BOOL Copy(const u16 *src);
		void Replace(const u16 *str, const u16 *val);
		void Realloc(u32 size);
		void Paste(u32 index, const u16 *val);

		s32 GetNextOcurrence(const u16 *str) const;

	private:
		const u16 *pOriginalStr;
		u16 *pConstructedStr;
		u32 iConstructedSize;
};


} // namespace

#endif //__TSTRING_H__
