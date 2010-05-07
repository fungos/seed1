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

/*! \file PcCartridge.cpp
	\author	Danny Angelo Carminati Grein
	\brief Cartridge PC Implementation
*/

#include "Cartridge.h"

#if defined(_PC_)

#include "Log.h"
#include "MemoryManager.h"
#include "FileSystem.h"

#include <stdio.h>

#define TAG "[Cartridge] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(Cartridge);

Cartridge::Cartridge()
	: iType(0)
	, iCurrentSlot(-1)
	, pData(NULL)
{
}

Cartridge::~Cartridge()
{
	this->Reset();
}

INLINE BOOL Cartridge::Initialize()
{
	Log(TAG "Initializing...");
	this->Reset();
	Log(TAG "Initialization completed.");

	return TRUE;
}

INLINE BOOL Cartridge::Reset()
{
	pMemoryManager->Free(this->pData);

	iType = 0;
	iSize = 0;
	MEMSET(strPath, '\0', PC_MAX_PATH);

	return TRUE;
}

INLINE BOOL Cartridge::Shutdown()
{
	this->Reset();
	Log(TAG "Terminating...");
	Log(TAG "Terminated.");
	return TRUE;
}

INLINE BOOL Cartridge::Prepare(eCartridgeSize size)
{
	iType = size;
	iSize = this->GetCardType(size);

	const char *p = pFileSystem->GetWriteableDirectory();
	ASSERT_MSG(p!=NULL, "You must set a WriteableDirectory!");

	MEMSET(strPath, '\0', PC_MAX_PATH);
	STRLCPY(strPath, p, PC_MAX_PATH);
	STRLCAT(strPath, "/", PC_MAX_PATH - 1);
	STRLCAT(strPath, CARTRIDGE_FILENAME, PC_MAX_PATH - strlen(strPath) - 1);

	this->pData = static_cast<u8 *>(pMemoryManager->Alloc(iSize));
	memset(this->pData, 0, iSize);

	if (!this->Verify(strPath, iSize))
	{
		if (!this->CreateSaveFile())
		{
			Log(TAG "ERROR: trying to create save file!");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Cartridge::Read(u32 src, void *dest, u32 len)
{
	if (src >= iSize)
	{
		Log(TAG "%s:%d: source address error.", __FILE__, __LINE__);
		return FALSE;
	}

	if (len > iSize)
	{
		Log(TAG "%s:%d: length error.", __FILE__, __LINE__);
		return FALSE;
	}

	if (src + len > iSize)
	{
		Log(TAG "%s:%d: source address error.", __FILE__, __LINE__);
		return FALSE;
	}

	s32 iResult = 0;
	FILE *pFp = fopen(strPath, "rb");
	if (!pFp)
	{
		Info(TAG "fopen %s failed", strPath);
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	iResult = fseek(pFp, 0, SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		fclose(pFp);
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	u32 iAmountRead = (u32)fread(this->pData, 1, iSize, pFp);
	if (iAmountRead != iSize)
	{
		Log(TAG "fread: could not load save data!");
		fclose(pFp);
		this->eLastError = Seed::ErrorDeviceFull;
		return FALSE;
	}

	iResult = fseek(pFp, static_cast<s32>(src), SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		fclose(pFp);
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	iAmountRead = (u32)fread(dest, 1, len, pFp);
	fclose(pFp);

	if (iAmountRead != len)
	{
		Log(TAG "fread: Could not read all the data requested: AmountRead = %d Requested = %d\n", iAmountRead, len);
		this->eLastError = Seed::ErrorDeviceFull;
		return FALSE;
	}

	return TRUE;
}

BOOL Cartridge::Write(u32 dest, const void *src, u32 len)
{
	if (dest >= iSize)
	{
		Log(TAG "%s:%d: dest address error.", __FILE__, __LINE__);
		return FALSE;
	}

	if (len > iSize)
	{
		Log(TAG "%s:%d: length error.", __FILE__, __LINE__);
		return FALSE;
	}

	if (dest + len > iSize)
	{
		Log(TAG "%s:%d: dest address error.", __FILE__, __LINE__);
		return FALSE;
	}

	s32 iResult = 0;

	u8 *ptr = &this->pData[dest];
	memcpy(ptr, src, len);

	FILE *pFp = fopen(strPath, "wb+");
	if (!pFp)
	{
		Info(TAG "fopen: Could not open '%s' for writing", strPath);
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	iResult = fseek(pFp, 0, SEEK_SET); //fseek(pFp, static_cast<s32>(dest), SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		this->eLastError = Seed::ErrorAccessDenied;
		fclose(pFp);
		return FALSE;
	}

	u32 iAmountWritten = (u32)fwrite(this->pData, 1, iSize, pFp); //fwrite(src, 1, len, pFp);
	fclose(pFp);

	if (iAmountWritten != iSize) //len)
	{
		Log(TAG "fwrite: Could not write all the data requested: AmountWritten = %d, Requested = %d\n", iAmountWritten, iSize); //len);
		this->eLastError = Seed::ErrorDeviceFull;
		return FALSE;
	}

	return TRUE;
}

u32 Cartridge::GetCardType(eCartridgeSize size)
{
	u32 i = 0;
	switch (size)
	{
		case Seed::Cartridge512b:
			i = 512;
		break;

		case Seed::Cartridge8192b:
			i = 8192;
		break;

		case Seed::Cartridge32768b:
			i = 32768;
		break;

		case Seed::Cartridge65536b:
			i = 65536;
		break;

		case Seed::Cartridge262144b:
			i = 262144;
		break;

		case Seed::CartridgeUnlimited:
			i = 1048576;
		break;

		default:
			Log(TAG "Cartridge size invalid.");
		break;
	}

	return i;
}

BOOL Cartridge::Verify(const char *filename, u32 filesize)
{
	u32 len = 0;
	BOOL ret = FALSE;

	if (this->GetFileSize(filename, &len))
	{
		if (len == filesize)
		{
			ret = TRUE;
		}
	}

	return ret;
}

BOOL Cartridge::GetFileSize(const char *filename, u32 *length)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	s32 ret = fseek(fp, 0, SEEK_END);
	*length = static_cast<u32>(ftell(fp));
	if (ret != 0)
	{
		fclose(fp);
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	ret = fclose(fp);
	if (ret != 0)
   {
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
   }

	this->eLastError = Seed::ErrorNone;
	return TRUE;
}

BOOL Cartridge::CreateSaveFile()
{
	Log(TAG "Creating save data...");

	FILE *fp = fopen(strPath, "wb+");
	if (fp)
	{
		void *pBlankData = pMemoryManager->Alloc(iSize);
		memset(pBlankData, 0, iSize);
		BOOL result = this->Write(0, pBlankData, iSize);
		pMemoryManager->Free(pBlankData);

		if (!result)
		{
			Log(TAG "Could not write into the newly created file.");
			this->eLastError = Seed::ErrorDeviceFull;
			return FALSE;
		}
	}
	else
	{
		Log(TAG "could not create file.");
		this->eLastError = Seed::ErrorAccessDenied;
		return FALSE;
	}

	Log(TAG "Save data created.");

	this->eLastError = Seed::ErrorNone;
	return TRUE;
}

}} // namespace

#endif // _PC_
