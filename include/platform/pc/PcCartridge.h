/*! \file PcCartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge PC Implementation
*/

#ifndef __PC_CARTRIDGE_H__
#define __PC_CARTRIDGE_H__

#include "Defines.h"

#if defined(_PC_)

#include "interface/ICartridge.h"
#include <stdio.h>
#include <stdlib.h>

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))
#define CARTRIDGE_FILENAME	"savedata"

#define PC_MAX_PATH		2048


namespace Seed { namespace PC {


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

Cartridge *const pCartridge = &Cartridge::instance;


}} // namespace


#else // _PC_

	#error "Include 'Cartridge.h' instead 'platform/pc/PcCartridge.h' directly."

#endif // _PC_
#endif // __PC_CARTRIDGE_H__
