/*! \file Base.h
	\author	Danny Angelo Carminati Grein
	\brief Basic things
*/

#ifndef __BASE_H__
#define __BASE_H__


#include "Defines.h"

#include <string.h>

namespace Seed {

/// String comparator operator to be used with STL/STL-like containers.
struct LowerThanStringComparator
{
	BOOL operator()(const char *s1, const char *s2) const
	{
		return STRCMP(s1, s2) < 0;
	}
};


} // namespace


#endif // __BASE_H__
