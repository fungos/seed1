/*! \file SaveSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Save System
*/

#ifndef __SAVESYSTEM_H__
#define __SAVESYSTEM_H__


#include "Cartridge.h"


#define SAVESYSTEM_SLOT_UNUSED		(255)


namespace Seed {


class SaveSystem
{
	public:
		struct sSaveInfo
		{
			u32 iID;
			u32 iLastSlot;
			u32 iCRC;
		};

	public:
		// Instance
		static SaveSystem instance;

	public:
		SaveSystem();
		virtual ~SaveSystem();

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

SaveSystem *const pSaveSystem = &SaveSystem::instance;


} // namespace

#endif // __SAVESYSTEM_H__

