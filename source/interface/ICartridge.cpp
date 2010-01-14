/*! \file ICartridge.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Cartridge class interface
*/

#include "interface/ICartridge.h"

namespace Seed {


ICartridge::ICartridge()
	: eLastError(ErrorNone)
	, iSize(0)
{
}

ICartridge::~ICartridge()
{
}

INLINE eCartridgeError ICartridge::GetLastError() const
{
	return this->eLastError;
}

INLINE BOOL ICartridge::Write(u32 dest, const void *src, u32 len)
{
	UNUSED(dest);
	UNUSED(src);
	UNUSED(len);
	return FALSE;
}

INLINE u32 ICartridge::GetSize() const
{
	return iSize;
}

INLINE BOOL ICartridge::Read(u32 src, void *dest, u32 len)
{
	UNUSED(dest);
	UNUSED(src);
	UNUSED(len);
	return FALSE;
}

INLINE BOOL ICartridge::Prepare(eCartridgeSize size)
{
	UNUSED(size);
	return FALSE;
}

INLINE u32 ICartridge::GetOffsetForSlot(u32 headerSize, u32 sharedSize, u32 dataSize, u32 slot) const
{
	return headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (slot * (dataSize + sizeof(u32)));
}

INLINE u32 ICartridge::GetOffsetForSlotCRC(u32 headerSize, u32 sharedSize, u32 dataSize, u32 slot) const
{
	return headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (slot * (dataSize + sizeof(u32))) + dataSize;
}

INLINE u32 ICartridge::GetRequiredSize(u32 headerSize, u32 sharedSize, u32 dataSize, u32 num_slots) const
{
	return headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (num_slots * (dataSize + sizeof(u32)));
}


} // namespace
