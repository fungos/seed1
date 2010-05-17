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

/*! \file SaveSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Save System
*/

#include "Defines.h"
#include "SaveSystem.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Checksum.h"
#include "System.h"

#define TAG "[SaveSystem] "

namespace Seed {

SEED_SINGLETON_DEFINE(SaveSystem);
BOOL SaveSystem::bIsSaving = FALSE;

SaveSystem::SaveSystem()
	: iTotalSlots(PLATFORM_SAVESYSTEM_SLOTS_MAX)
	, iNumSlots(PLATFORM_SAVESYSTEM_SLOTS_MAX)
	, iSlotDataSize(0)
	, iSharedDataSize(0)
	, iID(0)
	, cardType(Cartridge512b)
	, bInitialized(FALSE)
{
}

SaveSystem::~SaveSystem()
{
}

eCartridgeError SaveSystem::Initialize(eCartridgeSize type)
{
	if (!bInitialized)
	{
		Log(TAG "Initializing...");
		cardType = type;

		if (!pCartridge->Prepare(type))
		{
			Log(TAG "Failed to initialize cartridge.");
			bIsSaving = FALSE;
			return pCartridge->GetLastError();
		}

		bInitialized = TRUE;
		bIsSaving = FALSE;
		Log(TAG "Initialization completed.");
	}
	else
	{
		Log(TAG "WARNING: SaveSystem Initilaize being called more than once. FIX IT.");
	}

	return Seed::ErrorNone;
}

eCartridgeError SaveSystem::Check()
{
	sSaveInfo header;
	return this->ReadHeaderData(&header);
}

void SaveSystem::SetTotalSlots(u32 totalSlots)
{
	iTotalSlots = totalSlots;
}

u32 SaveSystem::GetTotalSlots() const
{
	return iTotalSlots;
}

eCartridgeError SaveSystem::Prepare(u32 myId, void *slotBlankData, u32 slotDataSize, void *sharedBlankData, u32 sharedDataSize)
{
	eCartridgeError error;

	if (!bInitialized)
	{
		Log(TAG "Save System not initialized!");
		return Seed::ErrorNotInitialized;
	}

	bIsSaving = TRUE;
	Log(TAG "Preparing Save System");

	iSlotDataSize	= slotDataSize;
	iSharedDataSize	= sharedDataSize;
	iID				= myId;

	u32 requiredSize = pCartridge->GetRequiredSize(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, (iTotalSlots + 1));
	if (pCartridge->GetSize() < requiredSize)
	{
		Log(TAG "Cartridge %d bytes of space, slot size and header require %d bytes.", pCartridge->GetSize(), requiredSize);
		bIsSaving = FALSE;
		return Seed::ErrorInvalidArgument;
	}

	// Test access to the card
	error = this->Check();

	if (error != Seed::ErrorNone)
	{
		bIsSaving = FALSE;
		return error;
	}

	BOOL bError = FALSE;
	eCartridgeError ret = Seed::ErrorNone;

	// Check all shared data state
	if (sharedBlankData != NULL && iSharedDataSize > 0)
	{
		void *sharedTestMemory = pMemoryManager->Alloc(iSharedDataSize, pDefaultPool);

		error = this->ReadSharedData(sharedTestMemory);
		if (error != Seed::ErrorNone)
		{
			bError = TRUE;
			ret = error;

			if (error == Seed::ErrorDataCorrupt)
			{
				Log(TAG "Shared data CRC check failed, formatting...");
				this->FormatShared(sharedBlankData);
			}
		}

		pMemoryManager->Free(sharedTestMemory);
	}

	if (error == Seed::ErrorNone)
	{
		// Check all slots state
		void *slotTestMemory = pMemoryManager->Alloc(iSlotDataSize, pDefaultPool);
		for (u8 i = 0; i < iTotalSlots; i ++)
		{
			error = this->Load(i, slotTestMemory);

			if (error != Seed::ErrorNone)
			{
				bError = TRUE;
				ret = error;

				if (error == Seed::ErrorDataCorrupt)
				{
					Log(TAG "Slot %d CRC check failed, formatting...", i);
					this->FormatSlot(i, slotBlankData);
				}
			}
		}

		pMemoryManager->Free(slotTestMemory);
	}

	bIsSaving = FALSE;
	Log(TAG "Save System Preparation Complete");

	return ret;
}

eCartridgeError SaveSystem::FormatCard(void *slotBlankData, void *sharedBlankData)
{
	eCartridgeError error;
	sSaveInfo header;
	bIsSaving = TRUE;

	if (!bInitialized)
	{
		bIsSaving = FALSE;
		return Seed::ErrorNotInitialized;
	}

	header.iID = iID;
	header.iLastSlot = 0;
	error = this->WriteHeaderData(&header);

	if (error != Seed::ErrorNone)
	{
		bIsSaving = FALSE;
		return error;
	}

	if (sharedBlankData)
	{
		error = this->FormatShared(sharedBlankData);
		if (error != Seed::ErrorNone)
		{
			bIsSaving = FALSE;
			return error;
		}
	}

	for (u8 i = 0; i < iTotalSlots; i++)
	{
		error = this->FormatSlot(i, slotBlankData);
		if (error != Seed::ErrorNone)
		{
			bIsSaving = FALSE;
			return error;
		}
	}

	bIsSaving = FALSE;
	return Seed::ErrorNone;
}

eCartridgeError SaveSystem::FormatShared(void *sharedBlankData)
{
	bIsSaving = TRUE;

	if (!bInitialized)
	{
		bIsSaving = FALSE;
		return Seed::ErrorNotInitialized;
	}

	if (iSharedDataSize == 0 || sharedBlankData == NULL)
	{
		bIsSaving = FALSE;
		return Seed::ErrorNone;
	}

	u32 sharedBlankDataCRC	= (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize);
	u32 offset 				= sizeof(sSaveInfo);
	u32 offsetCrc 			= sizeof(sSaveInfo) + iSharedDataSize;
	if (!pCartridge->Write(offset, sharedBlankData, iSharedDataSize))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}

	if (!pCartridge->Write(offsetCrc, &sharedBlankDataCRC, sizeof(u32)))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}
	this->Wait();

	bIsSaving = FALSE;
	return Seed::ErrorNone;
}

eCartridgeError SaveSystem::FormatSlot(u8 slot, void *slotBlankData)
{
	bIsSaving = TRUE;

	if (!bInitialized)
	{
		bIsSaving = FALSE;
		return Seed::ErrorNotInitialized;
	}

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		bIsSaving = FALSE;
		return Seed::ErrorInvalidArgument;
	}

	u32 blankDataCRC = (u32)pChecksum->Calculate((const char *)slotBlankData, iSlotDataSize);
	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Write(offset, slotBlankData, iSlotDataSize))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}

	if (!pCartridge->Write(offsetCrc, &blankDataCRC, sizeof(u32)))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}
	this->Wait();

	bIsSaving = FALSE;
	return Seed::ErrorNone;
}

eCartridgeError SaveSystem::Load(u32 slot, void *loadAddress, void *sharedData)
{
	u32 slotCRC;

	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		return Seed::ErrorInvalidArgument;
	}

	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Read(offset, loadAddress, iSlotDataSize))
		return pCartridge->GetLastError();

	if (!pCartridge->Read(offsetCrc, &slotCRC, sizeof(u32)))
		return pCartridge->GetLastError();

	eCartridgeError error = Seed::ErrorNone;
	if (slotCRC != (u32)pChecksum->Calculate((const char *)loadAddress, iSlotDataSize))
	{
		Log(TAG "Slot CRC Failed.");
		error = Seed::ErrorDataCorrupt;
	}

	if (error == Seed::ErrorNone)
		 error = this->ReadSharedData(sharedData);

	this->Wait();

	return error;
}

eCartridgeError SaveSystem::GetLastUsedSlot(u32 *lastSlot)
{
	eCartridgeError error;
	sSaveInfo header;

	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	error = this->ReadHeaderData(&header);

	if (error == Seed::ErrorNone)
	   *lastSlot = header.iLastSlot;

	return error;
}

eCartridgeError SaveSystem::Save(u32 slot, void *slotData, void *sharedData)
{
	bIsSaving = TRUE;

	if (!bInitialized)
	{
		bIsSaving = FALSE;
		return Seed::ErrorNotInitialized;
	}

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		bIsSaving = FALSE;
		return Seed::ErrorInvalidArgument;
	}

	sSaveInfo header;
	header.iID 			= iID;
	header.iLastSlot 	= slot;
	eCartridgeError error = this->WriteHeaderData(&header);
	if (error != Seed::ErrorNone)
	{
		Log(TAG "Error updating header information.");
		bIsSaving = FALSE;
		return error;
	}

	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Write(offset, slotData, iSlotDataSize))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}

	u32 slotCRC = (u32)pChecksum->Calculate((const char *)slotData, iSlotDataSize);
	if (!pCartridge->Write(offsetCrc, &slotCRC, sizeof(u32)))
	{
		bIsSaving = FALSE;
		return pCartridge->GetLastError();
	}

	error = this->WriteSharedData(sharedData);

	this->Wait();
	bIsSaving = FALSE;

	return error;
}

eCartridgeError SaveSystem::ReadHeaderData(sSaveInfo *header)
{
	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	if (!pCartridge->Read(0, header, sizeof(sSaveInfo)))
		return pCartridge->GetLastError();

	if (header->iID == iID)
	{
		if (header->iCRC == (u32)pChecksum->Calculate((const char *)header, sizeof(sSaveInfo) - sizeof(u32)))
			return Seed::ErrorNone;

		return Seed::ErrorDataCorrupt;
	}

	return Seed::ErrorNotFormatted;
}

eCartridgeError SaveSystem::WriteHeaderData(sSaveInfo *header)
{
	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	header->iCRC = (u32)pChecksum->Calculate((const char *)header, sizeof(sSaveInfo) - sizeof(u32));

	if (!pCartridge->Write(0, header, sizeof(sSaveInfo)))
		return pCartridge->GetLastError();

	return Seed::ErrorNone;
}

eCartridgeError SaveSystem::ReadSharedData(void *sharedBlankData)
{
	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	if (iSharedDataSize == 0 || sharedBlankData == NULL)
		return Seed::ErrorNone;

	if (!pCartridge->Read(sizeof(sSaveInfo), sharedBlankData, iSharedDataSize))
		return pCartridge->GetLastError();

	u32 crc = 0;
	if (!pCartridge->Read(sizeof(sSaveInfo) + iSharedDataSize, &crc, sizeof(u32)))
		return pCartridge->GetLastError();

	if (crc == (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize))
		return Seed::ErrorNone;

	return Seed::ErrorDataCorrupt;
}

eCartridgeError SaveSystem::WriteSharedData(void *sharedBlankData)
{
	if (!bInitialized)
		return Seed::ErrorNotInitialized;

	if (iSharedDataSize == 0 || sharedBlankData == NULL)
		return Seed::ErrorNone;

	u32 crc = (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize);

	if (!pCartridge->Write(sizeof(sSaveInfo), sharedBlankData, iSharedDataSize))
		return pCartridge->GetLastError();

	if (!pCartridge->Write(sizeof(sSaveInfo) + iSharedDataSize, &crc, sizeof(u32)))
		return pCartridge->GetLastError();

	return Seed::ErrorNone;
}

void SaveSystem::Wait()
{
	/*
	#if !defined(DEBUG_GAMESTATE_NODELAY)
	u8 i;
	for (i = 0; i < 24; i ++)
		pSystem->WaitForRetrace(ISystem::RATE_60FPS);
	#endif // DEBUG_GAMESTATE_NODELAY
	*/
}

BOOL SaveSystem::IsSaving() const
{
	return bIsSaving;
}

} // namespace
