#ifndef __KEY_H__
#define __KEY_H__


#include "Defines.h"


namespace Seed {


/*!
Represents a key (or a character).
*/
class Key
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
