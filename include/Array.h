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

/*! \file Array.h
	\author	Korck
	\brief Array common functions
*/

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "Log.h"

namespace Seed {

template <class TYPE, int SIZE> class Array
{
	private:
		TYPE array[SIZE];
		u32 used;

	public:
		Array()
			: array()
			, used(0)
		{}

		inline int ArraySize()
		{
			return SIZE;
		}

		inline u32 Size() const
		{
			return used;
		}

		inline TYPE& operator[](u32 index)
		{
			ASSERT(index < used);
			return array[index];
		}

		inline void Add(TYPE element)
		{
			ASSERT(used < SIZE);
			array[used++] = element;
		}

		inline void Add(TYPE element, u32 pos)
		{
			ASSERT((used + 1) < SIZE);

			for (s32 i = (s32)used-1; i >= (s32)pos; i--)
			{
				array[i+1] = array[i];
			}

			used++;

			array[pos] = element;
		}

		inline void Add()
		{
			ASSERT(used < SIZE);
			used++;
		}

		inline void Truncate(u32 pos = 0)
		{
			if (pos == 0 && used == 0)
				return;

			ASSERT(pos < used);
			used = pos;
		}

		inline void Del(u32 pos, u32 qtt = 1)
		{
			ASSERT(qtt >= 1 && (pos + qtt) <= used);
			for (u32 i = 0; i < (used - (pos + qtt)); i++)
				array[pos + i] = array[pos + qtt + i];

			used -= qtt;
		}

		inline void Remove(TYPE element)
		{
			ASSERT(element != NULL);

			u32 removed = 0;
			for (u32 i = 0; i < used; i++)
			{
				if (array[i] == element)
				{
					removed++;
					array[i] = NULL;
				}
			}

			u32 firstUsed = 0;
			if (removed)
			{
				u32 moved = 0;
				while (moved != removed)
				{
					u32 emptyPos = 0;
					u32 lastUsed = used - 1;

					for (u32 i = firstUsed; i < used; i++)
					{
						if (array[i] == NULL)
						{
							emptyPos = i;

							if (i)
								firstUsed = i - 1;

							break;
						}
					}

					for (u32 i = lastUsed; i > 0; i--)
					{
						if (array[i])
						{
							lastUsed = i;
							break;
						}
					}

					if (lastUsed > emptyPos)
					{
						array[emptyPos] = array[lastUsed];
						array[lastUsed] = NULL;
					}
					used--;
					moved++;
				}
			}
		}

		inline void Replace(TYPE element, u32 pos, u32 qtt = 1)
		{
			ASSERT(qtt > 0 && (pos + qtt) < used);
			for (u32 i = pos; i < (pos + qtt); i++)
				array[i] = element;
		}
};

} // namespace

#endif // __ARRAY_H__
