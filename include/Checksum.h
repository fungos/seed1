/*! \file Checksum.h
	\author	Danny Angelo Carminati Grein
	\brief Checksum calculation
*/


#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__


#include "Defines.h"


namespace Seed {

/// Helper class to calculate CRC checksums
class Checksum
{
	public:
		Checksum();
		~Checksum();

		/// Sets the first polynomial to use when generating CRC tables
		void 	SetPolynomial1(unsigned int polynomial);

		/// Sets the seconds polynomial to use when generatinc CRC tables
		void	SetPolynomial2(unsigned int polynomial);

		/// Calculate a filename CRC (will ignore case and change '\' to '/'
		/**
 		\param name A file name
 		*/
		u64		Calculate(const char *name);

		/// Calculate CRC from an arbitrary data until len size
		/**
 		\param data Pointer to the data to which generate a CRC
		\param len Size of the data
 		*/
		u64		Calculate(const char *data, u32 len);


	public:
		static Checksum instance;


	private:
		SEED_DISABLE_COPY(Checksum);


	private:
		unsigned int	iPolynomial1;
		unsigned int	iPolynomial2;

		unsigned int	aiCRCTable1[256];
		unsigned int	aiCRCTable2[256];

};


Checksum *const pChecksum = &Checksum::instance;


} // namespace


#endif // __CHECKSUM_H__
