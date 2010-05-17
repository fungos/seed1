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

/*! \file Singleton.h
	\author	Danny Angelo Carminati Grein
	\brief Singleton template
*/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#define SEED_SINGLETON_HEAP_DECLARE(type)		\
											protected: \
												type(); \
											public: \
												virtual ~type(); \
												\
												static type *Instance; \
												\
												static inline type *GetInstance() \
												{ \
													if (type::Instance == NULL) \
														type::Instance = new type(); \
													return type::Instance; \
												}

#define SEED_SINGLETON_HEAP_DEFINE(type)		type *type::Instance = NULL; \
												type *const p##type = type::GetInstance();

#define SEED_SINGLETON_STACK_DECLARE(type)	\
											public: \
												type(); \
												virtual ~type(); \
												\
												static type instance; \
												static type *Instance; \
												\
												static inline type *GetInstance() \
												{ \
													type::Instance = &type::instance; \
													return &type::instance; \
												}

#define SEED_SINGLETON_STACK_DEFINE(type)		type type::instance; \
												type *type::Instance = &type::instance; \
												type *const p##type = type::GetInstance();


#if SEED_SINGLETON_HEAP == 1

#define SEED_SINGLETON_DECLARE					SEED_SINGLETON_HEAP_DECLARE
#define SEED_SINGLETON_DEFINE					SEED_SINGLETON_HEAP_DEFINE

#else

#define SEED_SINGLETON_DECLARE					SEED_SINGLETON_STACK_DECLARE
#define SEED_SINGLETON_DEFINE					SEED_SINGLETON_STACK_DEFINE

#endif

#define SEED_SINGLETON_EXTERNALIZE(type)	extern type *const p##type;

#endif // __SIGNLETON_H__
