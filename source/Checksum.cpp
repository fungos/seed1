/*! \file Checksum.cpp
\author	Danny Angelo Carminati Grein
\brief Checksum calculation
*/


#include "Checksum.h"


namespace Seed {

Checksum Checksum::instance;


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

