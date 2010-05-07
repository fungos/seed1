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

/*! \file SaveSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Save System
*/

#ifndef __SAVESYSTEM_H__
#define __SAVESYSTEM_H__

#include "Cartridge.h"
#include "Singleton.h"

#define SAVESYSTEM_SLOT_UNUSED		(255)

namespace Seed {

class SEED_CORE_API SaveSystem
{
	SEED_SINGLETON_DECLARE(SaveSystem);

	public:
		struct SEED_CORE_API sSaveInfo
		{
			u32 iID;
			u32 iLastSlot;
			u32 iCRC;
		};

	public:
		eCartridgeError Initialize(eCartridgeSize type);
		eCartridgeError Prepare(u32 myId, void *slotBlankData, u32 slotDataSize, void *sharedBlankData = NULL, u32 sharedSize = 0);
		eCartridgeError Check();
		eCartridgeError FormatCard(void *slotBlankData, void *sharedBlankData = NULL);
		eCartridgeError FormatSlot(u8 slot, void *slotBankData);

		eCartridgeError Load(u32 slot, void *loadAddress, void *sharedData = NULL);
		eCartridgeError Save(u32 slot, void *data, void *sharedData = NULL);
		eCartridgeError GetLastUsedSlot(u32 *lastSlot);

		eCartridgeError ReadSharedData(void *sharedData);
		eCartridgeError WriteSharedData(void *sharedData);

		void SetTotalSlots(u32 slots);
		u32 GetTotalSlots() const;

		BOOL IsSaving() const;

	private:
		SEED_DISABLE_COPY(SaveSystem);

		eCartridgeError GetNewSlotBuffer(u32 slot, u8 *slotBuffer);
		eCartridgeError GetSlotBuffer(u32 slot, u8 *slotBuffer);
		eCartridgeError SetSlotBuffer(u32 slot, u8 slotBuffer);
		eCartridgeError ReadHeaderData(sSaveInfo *header);
		eCartridgeError WriteHeaderData(sSaveInfo *header);
		eCartridgeError FormatShared(void *sharedBlankData);

		void Wait();

	private:
		u32 iTotalSlots;
		u32 iNumSlots;
		u32 iSlotDataSize;
		u32 iSharedDataSize;
		u32 iID;
		eCartridgeSize cardType;

		static BOOL bIsSaving;
		BOOL bInitialized;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(SaveSystem);
}

} // namespace

#endif // __SAVESYSTEM_H__
