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

/*! \file PS3Sound.h
	\author	Danny Angelo Carminati Grein
	\brief ps3dev sound implementation
*/

#ifndef __PS3DEV_SOUND_H__
#define __PS3DEV_SOUND_H__

#include "Defines.h"

#if defined(_PS3DEV_)

#include "File.h"
#include "SeedInit.h"
#include "interface/IResource.h"
#include "interface/ISound.h"
#include "api/oal_ogg/vorbis_util.h"

namespace Seed { namespace PS3 {

IResource *SoundResourceLoader(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

class SEED_CORE_API Sound : public ISound
{
	friend IResource *SoundResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;
	friend class SoundSource;

	public:
		Sound();
		virtual ~Sound();

		void Reset();

		// IResource
		using IResource::Load;
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		virtual const void *GetData() const;

	private:
		SEED_DISABLE_COPY(Sound);

	private:
		u32			iBuffer;
		File			stFile;
};

}} // namespace

#else // _PS3DEV_
	#error "Include 'Sound.h' instead 'platform/ps3dev/PS3Sound.h' directly."
#endif // _PS3DEV_
#endif // __PS3DEV_SOUND_H__
