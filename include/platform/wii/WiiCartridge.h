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

/*! \file WiiCatridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge Functionality
*/

#ifndef __WII_CARTRIDGE_H__
#define __WII_CARTRIDGE_H__

#if defined(_WII_)

#include "interface/ICartridge.h"
#include "Singleton.h"

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))
#define CARTRIDGE_FILENAME	"savefile.dat"

namespace Seed { namespace WII {

class Cartridge : public ICartridge
{
	SEED_SINGLETON_DECLARE(Cartridge);
	public:
		// ICartridge
		virtual BOOL Prepare(eCartridgeSize size);

		virtual BOOL Read(u32 src, void *dest, u32 len);
		virtual BOOL Write(u32 dest, const void *src, u32 len);

		virtual u32 GetOffsetForSlot(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetOffsetForSlotCRC(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetRequiredSize(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 num_slots) const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	private:
		SEED_DISABLE_COPY(Cartridge);

		u32 GetCardType(eCartridgeSize size) const;
		BOOL CreateSaveFile();
		BOOL CreateBannerFile();
		BOOL Verify(const char *pName, u32 iSize);
		BOOL GetFileSize(const char *pSrcPath, u32 *pLength);
		BOOL CheckBlocks(u32 fsBlocks, u32 iNodes);

	private:
		char strPath[WII_MAX_PATH] ATTRIBUTE_ALIGN(32);
		u8 cMask;

		u32 iType;
		s32 iCurrentSlot;

		WiiFileInfo pFp;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Cartridge);
}

}} // namespace

#else // _WII_
	#error "Include 'Cartridge.h' instead 'platform/wii/WiiCartridge.h' directly."
#endif // _WII_
#endif // __WII_CARTRIDGE_H__
