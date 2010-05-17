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

/*! \file PcCartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge PC Implementation
*/

#ifndef __PC_CARTRIDGE_H__
#define __PC_CARTRIDGE_H__

#include "Defines.h"

#if defined(_PC_)

#include "interface/ICartridge.h"
#include "Singleton.h"

#include <stdio.h>
#include <stdlib.h>

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))
#define CARTRIDGE_FILENAME	"savedata"

#define PC_MAX_PATH		2048

namespace Seed { namespace PC {

class SEED_CORE_API Cartridge : public ICartridge
{
	SEED_SINGLETON_DECLARE(Cartridge);
	public:
		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual BOOL Prepare(eCartridgeSize size);

		virtual BOOL Read(u32 src, void *dest, u32 len);
		virtual BOOL Write(u32 dest, const void *src, u32 len);

	private:
		SEED_DISABLE_COPY(Cartridge);

		u32 GetCardType(eCartridgeSize size);
		BOOL CreateSaveFile();
		BOOL Verify(const char *filename, u32 filesize);
		BOOL GetFileSize(const char *filename, u32 *length);

	private:
		char strPath[PC_MAX_PATH];
		u32 iType;
		s32 iCurrentSlot;
		u8 *pData;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Cartridge);
}

}} // namespace

#else // _PC_
	#error "Include 'Cartridge.h' instead 'platform/pc/PcCartridge.h' directly."
#endif // _PC_
#endif // __PC_CARTRIDGE_H__
