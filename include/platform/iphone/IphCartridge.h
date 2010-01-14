/*! \file IphCartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge Iphone Implementation
*/

#ifndef __IPH_CARTRIDGE_H__
#define __IPH_CARTRIDGE_H__

#ifdef _IPHONE_

#include "interface/ICartridge.h"
#include <stdio.h>
#include <stdlib.h>

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))
#define CARTRIDGE_FILENAME	"savedata"

#define IPHONE_MAX_PATH		2048


namespace Seed { namespace iPhone {


class Cartridge : public ICartridge
{
	public:
		Cartridge();
		virtual ~Cartridge();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual BOOL Prepare(eCartridgeSize size);

		virtual BOOL Read(u32 src, void *dest, u32 len);
		virtual BOOL Write(u32 dest, const void *src, u32 len);

	public:
		static Cartridge instance;

	private:
		SEED_DISABLE_COPY(Cartridge);

		inline u32 GetCardType(eCartridgeSize size);
		BOOL CreateSaveFile();
		BOOL Verify(const char *filename, u32 filesize);
		BOOL GetFileSize(const char *filename, u32 *length);

	private:
		char strPath[IPHONE_MAX_PATH];

		u32	iSize;
		u32 iType;
		s32 iCurrentSlot;
		u8 *pData;
};

Cartridge *const pCartridge = &Cartridge::instance;


}} // namespace


#else // _IPHONE_

	#error "Include 'Cartridge.h' instead 'platform/iphone/IphCartridge.h' directly."

#endif // _IPHONE_
#endif // __IPH_CARTRIDGE_H__
