#ifndef __TSTRING_H__
#define __TSTRING_H__

#include "Defines.h"

namespace Seed {

class String
{
	public:
		String();
		String(const WideString newStr);
		virtual ~String();
		
		String &Initialize(const WideString newStr);
		String &Reset();
		
		// Size modifiers
		// All methods return this
		String &Set();
		String &Set(const u16 *paramName, const u16 *paramVal);
		String &Set(const u16 *paramName, const char *paramVal);
		String &Set(const u16 *paramName, u32 paramVal);
		String &Set(const u16 *paramName, f32 paramVal);
		String &Set(const u16 *paramName, bool paramVal);
		String &Set(const u16 *paramName, const char paramVal);
		
		void SubString(u32 index, u32 size);
		void Append(WideChar chr);
		//void Append(const u16 *str);
		//void Append(const String &str);

		// Common string functions
		u32 Length() const;
		BOOL Equals(const u16 *str) const;

		static u32 Length(const WideString str);
		static BOOL Equals(const WideString str1, const WideString str2);

		static u32 Length(const u16 *str);
		static BOOL Equals(const u16 *str1, const u16 *str2);

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
		BOOL bInitialized;
};


} // namespace

#endif //__TSTRING_H__