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

/*! \file ICartridge.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Cartridge class interface
*/

#include "interface/ICartridge.h"
#include "Log.h"

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
	SEED_ABSTRACT_METHOD;
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
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE BOOL ICartridge::Prepare(eCartridgeSize size)
{
	UNUSED(size);
	SEED_ABSTRACT_METHOD;
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
