#include "Key.h"


namespace Seed {


Key::Key(int value)
	: iValue(value)
{
}

Key::~Key()
{
}

INLINE BOOL Key::IsCharacter() const
{
	return (iValue >= 32 && iValue <= 126) || (iValue >= 162 && iValue <= 255) || (iValue == 9);
}

INLINE BOOL Key::IsNumber() const
{
	return iValue >= 48 && iValue <= 57;
}

INLINE BOOL Key::IsLetter() const
{
	return (((iValue >= 65 && iValue <= 90) || (iValue >= 97 && iValue <= 122) || (iValue >= 192 && iValue <= 255)) && (iValue != 215) && (iValue != 247));
}

INLINE u32 Key::GetValue() const
{
	return iValue;
}

INLINE BOOL Key::operator==(const Key& key) const
{
	return iValue == key.iValue;
}

INLINE BOOL Key::operator!=(const Key& key) const
{
	return (iValue != key.iValue);
}


} // namespace
