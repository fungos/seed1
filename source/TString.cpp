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

#include "TString.h"
#include "SeedInit.h"

namespace Seed {


String::String()
	: pOriginalStr(NULL)
	, pConstructedStr(NULL)
	, iConstructedSize(0)
{
	this->Release();
}

String::String(const WideString newStr)
	: pOriginalStr(NULL)
	, pConstructedStr(NULL)
	, iConstructedSize(0)
{
	this->Set(newStr);
}

String::String(const String& str)
	: pOriginalStr(NULL)
	, pConstructedStr(NULL)
	, iConstructedSize(0)
{
	*this = str;
}

String::~String()
{
	this->Release();
}

INLINE String &String::Set(const WideString newStr)
{
	this->Release();

	pOriginalStr = static_cast<const u16 *>((void *)newStr);

	iConstructedSize = this->Length(pOriginalStr) + 1;
	pConstructedStr = glStringPool.Alloc(iConstructedSize);
	
	this->Copy(pOriginalStr);

	return *this;
}

INLINE String &String::Reset()
{
	if (pConstructedStr)
	{
		glStringPool.Free(pConstructedStr);
		pConstructedStr = NULL;
	}

	iConstructedSize = this->Length(pOriginalStr) + 1;
	pConstructedStr = glStringPool.Alloc(iConstructedSize);
	
	this->Copy(pOriginalStr);

	return *this;
}

INLINE String &String::Release()
{
	pOriginalStr = NULL;

	if (pConstructedStr)
	{
		glStringPool.Free(pConstructedStr);
		pConstructedStr = NULL;
	}

	return *this;
}

INLINE String &String::operator=(const String &string)
{
	this->Release();

	iConstructedSize	= string.iConstructedSize;
	pOriginalStr		= string.pOriginalStr;

	pConstructedStr		= glStringPool.Alloc(iConstructedSize);
	
	this->Copy(string.pConstructedStr);

	return *this;
}


INLINE String &String::Clear()
{
	if(pConstructedStr)
		MEMSET(pConstructedStr, '\0', this->Length() * sizeof(u16));

	return *this;
}

INLINE String &String::Set(const u16 *paramName, const u16 *paramVal)
{
	this->Replace(paramName, paramVal);
	return *this;
}

INLINE String &String::Set(const u16 *paramName, const char *paramVal)
{
	u32 len = strlen(paramVal);

	u16 parsedParamVal[128];
	MEMSET(parsedParamVal, '\0', sizeof(parsedParamVal));
	//u16 *parsedParamVal = glStringPool.Alloc(len * sizeof(u16) + 2);
	char *c0;
	char *c1;
	for (u32 i = 0; i < len; i++)
	{
		c0 = (char*)(parsedParamVal + i + 0);
		c1 = (char*)(parsedParamVal + i + 1);

		*c0 = paramVal[i];
		*c1 = '\0';
	}

	parsedParamVal[len] = '\0';
 
	this->Replace(paramName, parsedParamVal);
	//glStringPool.Free(parsedParamVal);

	return *this;
}

INLINE String &String::Set(const u16 *paramName, const char paramVal)
{
	char tmp[4];

	tmp[0] = paramVal;
	tmp[1] = '\0';
	tmp[2] = '\0';
	tmp[3] = '\0';
 
	this->Replace(paramName, (u16 *)tmp);

	return *this;
}

INLINE String &String::Set(const u16 *paramName, const u16 paramVal)
{
	char *temp;
	char tmp[4];
	char tmp2[2];

	temp = tmp2;

	temp = (char*)&paramVal;

	tmp[0] = tmp2[0];
	tmp[1] = tmp2[1];
	tmp[2] = '\0';
	tmp[3] = '\0';
 
	this->Replace(paramName, (u16 *)paramVal);

	return *this;
}

INLINE String &String::Set(const u16 *paramName, u32 paramVal)
{
	char strTemp[16];
	snprintf(strTemp, 16, "%d", paramVal);

	this->Set(paramName, strTemp);

	return *this;
}

INLINE String &String::Set(const u16 *paramName, f32 paramVal)
{
	char strTemp[32];
	snprintf(strTemp, 32, "%.4f", paramVal); // FIXME

	this->Set(paramName, strTemp);

	return *this;
}

INLINE String &String::Set(const u16 *paramName, bool paramVal)
{
	this->Set(paramName, (paramVal ? "True" : "False"));
	return *this;
}

INLINE String &String::Set(const u16 *paramName, String *paramVal)
{
	if (paramVal)
	{
		this->Set(paramName, paramVal->pConstructedStr);
	}
	return *this;
}

INLINE BOOL String::Copy(const u16 *str)
{
	BOOL ret = FALSE;

	if (pConstructedStr && str)
	{
		u32 len = this->Length(str);
		if (len < iConstructedSize)
		{
			MEMCOPY(pConstructedStr, str, len * sizeof(u16) + 2);
			ret = TRUE;
		}
	}

	return FALSE;
}

INLINE void String::Replace(const u16 *str, const u16 *val)
{
	if (!pConstructedStr || !str)
		return;

	u32 bufferLen = this->Length(pConstructedStr);
	u32 stringLen = this->Length(str);
	u32 valueLen  = this->Length(val);

	s32 nextOcurrence = 0;
	do
	{
		nextOcurrence = this->GetNextOcurrence(str);

		if (nextOcurrence >= 0)
		{
			//test for size increase
			if (valueLen > stringLen)
				this->Realloc(bufferLen + (valueLen - stringLen));

			this->Cut(nextOcurrence, stringLen);
			this->Paste(nextOcurrence, val);
		}
		else
			break;

	} while (TRUE);
}

INLINE s32 String::GetNextOcurrence(const u16 *str) const
{
	u32 bufferLen = this->Length(pConstructedStr);
	u32 stringLen = this->Length(str);

	u16 strTemp[64];
	for (u32 i = 0; i < bufferLen; i++)
	{
		if (i + stringLen > bufferLen)
			return -1; //not found

		MEMCOPY(strTemp, &pConstructedStr[i], stringLen * sizeof(u16));
		strTemp[stringLen] = 0;

		if (this->Equals(strTemp, str))
			return i;
	}

	return -1; //not found
}

INLINE void String::Realloc(u32 newSize)
{
	iConstructedSize = newSize;
	if (pConstructedStr)
	{
		iConstructedSize = newSize + 1;

		u16 *pOldBuffer = pConstructedStr;
		pConstructedStr = glStringPool.Alloc(newSize);
		this->Copy(pOldBuffer);
		glStringPool.Free(pOldBuffer);
	}
	else
	{
		pConstructedStr = glStringPool.Alloc(newSize);
	}
}

INLINE void String::SubString(u32 from, u32 to)
{
	ASSERT(from <= to);

	if (from == to)
	{
		MEMSET(pConstructedStr, '\0', this->Length() * sizeof(u16));
		return;
	}

	u32 len = to - from + 1;
	MEMCOPY(pConstructedStr, &pConstructedStr[from], len * sizeof(u16));
	pConstructedStr[len - 1] = NULL;
}

INLINE void String::Cut(u32 index, u32 size)
{
	if (!size)
		return;
	
	u32 len = this->Length();
	ASSERT(index + size <= len);

	MEMCOPY(&pConstructedStr[index], &pConstructedStr[index + size], (len - index - size) * sizeof(u16));
	pConstructedStr[len - size] = NULL;
}

INLINE void String::Paste(u32 index, const u16 *val)
{
	u32 bufferLen = this->Length(pConstructedStr) + 1;
	u32 valueLen  = this->Length(val);

	//move buffer forward
	// FIXME: WARNING: copy forward eh *possivel* problema em algumas implementacoes de memcpy que nao possuem buffer temporario internamente.
	u16 *tmp = glStringPool.Alloc(bufferLen);
	MEMCOPY(tmp, pConstructedStr, bufferLen * sizeof(u16));
	MEMCOPY(&pConstructedStr[index + valueLen], &tmp[index], (bufferLen - index) * sizeof(u16));
	glStringPool.Free(tmp);
	
	//paste string
	MEMCOPY(&pConstructedStr[index], val, valueLen * sizeof(u16));
	pConstructedStr[bufferLen + valueLen - 1] = NULL;
}

INLINE void String::Append(WideChar chr)
{
	if (!pConstructedStr)
	{
		iConstructedSize = 32;
		pConstructedStr = glStringPool.Alloc(iConstructedSize);
		Clear();
	}

	u32 len = this->Length() + 2; // len + 1char + null
	if (len >= iConstructedSize)
		this->Realloc(len);

	pConstructedStr[len - 2] = chr;
	pConstructedStr[len - 1] = NULL;
}

INLINE void String::Append(const String *pString)
{
	if (!pConstructedStr)
	{
		iConstructedSize = 32;
		pConstructedStr = glStringPool.Alloc(iConstructedSize);
		Clear();
	}

	u32 iLen		= Length();
	u32 iAppendLen	= pString->Length();
	u32 iSize		= iLen + iAppendLen + 1; // len + stringlen + null
	if (iSize >= iConstructedSize)
		this->Realloc(iSize);

	for(u32 i = 0; i < iAppendLen; i++)
		pConstructedStr[iLen + i] = pString->GetData()[i];

	pConstructedStr[iSize-1] = NULL;
}

INLINE u32 String::Length() const
{
	return String::Length(pConstructedStr);
}

INLINE u32 String::Length(const WideString str)
{
	if (!str)
		return 0;

	u16 iIndex = 0;
	while (str[iIndex])
		iIndex += 1;

	return iIndex;
}

INLINE BOOL String::Equals(const String *pString) const
{
	return this->Equals(pConstructedStr, pString->GetData());
}

INLINE BOOL String::Equals(const u16 *str) const
{
	return this->Equals(pConstructedStr, str);
}

INLINE BOOL String::Equals(const WideString str1, const WideString str2)
{
	if (str1 == str2)
		return TRUE;

	u32 len1 = String::Length(str1);
	u32 len2 = String::Length(str2);

	if (len1 != len2)
		return FALSE;

	for (u32 i = 0; i < len1; i++)
	{
		if (str1[i] != str2[i])
			return FALSE;
	}

	return TRUE;
}

INLINE const WideString String::GetData() const
{
	return (const WideString)pConstructedStr;
}

} // namespace
