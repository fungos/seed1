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

/*! \file Checksum.h
	\author	Danny Angelo Carminati Grein
	\brief Checksum calculation
*/

#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#include "Defines.h"
#include "Singleton.h"

namespace Seed {

/// Helper class to calculate CRC checksums
class SEED_CORE_API Checksum
{
	SEED_SINGLETON_DECLARE(Checksum);
	public:
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

	private:
		SEED_DISABLE_COPY(Checksum);

	private:
		unsigned int	iPolynomial1;
		unsigned int	iPolynomial2;

		unsigned int	aiCRCTable1[256];
		unsigned int	aiCRCTable2[256];

};

//extern "C" {
//SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Checksum);
//}

#define pChecksum Checksum::GetInstance()

} // namespace

#endif // __CHECKSUM_H__
