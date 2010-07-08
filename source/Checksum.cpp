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

/*! \file Checksum.cpp
\author	Danny Angelo Carminati Grein
\brief Checksum calculation
*/

#include "Checksum.h"

namespace Seed {

Checksum *pChecksum = NULL;
SEED_SINGLETON_DEFINE(Checksum);

Checksum::Checksum()
	: iPolynomial1(0)
	, iPolynomial2(0)
{
	this->SetPolynomial1(0xedb88320);
	this->SetPolynomial2(0x12477cdf);
}

Checksum::~Checksum()
{
}

void Checksum::SetPolynomial1(unsigned int polynomial)
{
	iPolynomial1 = polynomial;

	for (int n = 0; n < 256; n++)
	{
		unsigned long c = (unsigned long)n;

		for (int b = 0; b < 8; b++)
			c = c & 1 ? iPolynomial1 ^ (c >> 1) : c >> 1;

		aiCRCTable1[n] = c;
	}
}

void Checksum::SetPolynomial2(unsigned int polynomial)
{
	iPolynomial2 = polynomial;

	for ( int n = 0; n < 256; n++)
	{
		unsigned long c = (unsigned long)n;

		for (int b = 0; b < 8; b++)
			c = c & 1 ? iPolynomial2 ^ (c >> 1) : c >> 1;

		aiCRCTable2[n] = c;
	}
}

u64 Checksum::Calculate(const char *name)
{
	u32 crc1 = 0;
	u32 crc2 = 0;

	while (*name != '\0')
	{
		u8 chr = (u8)*name++;

		if (chr >= 'a' && chr <= 'z')
			chr = (u8)((chr - (u8)'a') + (u8)'A');
		else if (chr == '\\')
			chr = (u8)'/';

		crc1 = aiCRCTable1[(crc1 ^ chr) & 0xff] ^ (crc1 >> 8);
		crc2 = aiCRCTable2[(crc2 ^ chr) & 0xff] ^ (crc2 >> 8);
	}

	return ((static_cast<u64>(crc1) << 32) | static_cast<u64>(crc2));
}

u64 Checksum::Calculate(const char *data, u32 len)
{
	u32 crc1 = 0;
	u32 crc2 = 0;

	for (u32 i = 0; i < len; i++)
	{
		u8 chr = (u8)*data++;

		crc1 = aiCRCTable1[(crc1 ^ chr) & 0xff] ^ (crc1 >> 8);
		crc2 = aiCRCTable2[(crc2 ^ chr) & 0xff] ^ (crc2 >> 8);
	}

	return ((static_cast<u64>(crc1) << 32) | static_cast<u64>(crc2));
}

} // namespace
