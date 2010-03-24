/*! \file WiiCatridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge Functionality
*/

#ifndef __WII_CARTRIDGE_H__
#define __WII_CARTRIDGE_H__

#if defined(_WII_)

#include "interface/ICartridge.h"

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))
#define CARTRIDGE_FILENAME	"savefile.dat"

namespace Seed { namespace WII {

class Cartridge : public ICartridge
{
	public:
		Cartridge();
		virtual ~Cartridge();

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

	public:
		static Cartridge instance;

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

Cartridge *const pCartridge = &Cartridge::instance;

}} // namespace

#else // _WII_
	#error "Include 'Cartridge.h' instead 'platform/wii/WiiCartridge.h' directly."
#endif // _WII_
#endif // __WII_CARTRIDGE_H__
