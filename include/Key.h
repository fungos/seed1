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

#ifndef __KEY_H__
#define __KEY_H__

#include "Defines.h"

namespace Seed {

/*!
Represents a key (or a character).
*/
class SEED_CORE_API Key
{
	public:
		//!	Key constructor.
		/*!
		\param value Ascii code or enum value for the key.
		 */
		Key(int value = 0); // must not be explicit so == comparator works directly with enum;
		virtual ~Key();

		/*!
		Check if this key is a printable character.

		\return TRUE if the key is a letter, a number or a whitespace, everything else returns FALSE.
		 */
		BOOL IsCharacter() const;

		//! Checks if this key is a number.
		/*!
			\return TRUE if the key is a a number from 0 to 9, everything else returns FALSE.
		 */
		BOOL IsNumber() const;

		//! Checks if this key is a letter.
		/*!
			\return TRUE if the key is a letter between a-z or A-Z, everything else returns FALSE.
		 */
		BOOL IsLetter() const;

		/*!
		Gets the value of the key.

		\return the value of the key.
		 */
		u32 GetValue() const;

		/*!
		Compares to keys.

		\param key The key to compare this key with.
		\return True if the keys are equal, false otherwise.
		 */
		BOOL operator==(const Key &key) const;

		/*!
		Compares to keys.

		\param key The key to compare this key with.
		\return True if the keys are not equal, false otherwise.
		*/
		BOOL operator!=(const Key &key) const;

	protected:
		u32 iValue;

	//private:
		//SEED_DISABLE_COPY(Key);
};

} // namespace

#endif // __KEY_H__
