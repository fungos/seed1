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

/*! \file Formats.h
	\author	Danny Angelo Carminati Grein
	\brief File formats structs and magic codes
*/

#ifndef __FORMATS_H__
#define __FORMATS_H__

#include "Defines.h"
#include "Enum.h"

#define IMAGE_OBJECT_MAGIC			0x004F4954
#define IMAGE_OBJECT_VERSION		0x00000001
#define FRAME_MAGIC					0x004F4654
#define FRAME_VERSION				0x00000002
#define SPRITE_OBJECT_MAGIC			0x004F5354
#define SPRITE_OBJECT_VERSION		0x00000003
#define ANIMATION_OBJECT_MAGIC		0x004F4154
#define ANIMATION_OBJECT_VERSION	0x00000001
#define AUDIO_OBJECT_MAGIC			0x004F4254
#define AUDIO_OBJECT_VERSION		0x00000001
#define PACKAGE_OBJECT_MAGIC		0x00504B47
#define PACKAGE_OBJECT_VERSION		0x00000001
#define MUSIC_OBJECT_MAGIC			0x004D4742
#define MUSIC_OBJECT_VERSION		0x00000001
#define SOUND_OBJECT_MAGIC			0x00584653
#define SOUND_OBJECT_VERSION		0x00000001
#define BUTTON_OBJECT_MAGIC			0x004E5442
#define BUTTON_OBJECT_VERSION		0x00000001
#define MASK_OBJECT_MAGIC			0x004B534D
#define MASK_OBJECT_VERSION			0x00000001
#define DICT_OBJECT_MAGIC			0x00434944
#define DICT_OBJECT_VERSION			0x00000001
#define FONT_OBJECT_MAGIC			0x00544E46
#define FONT_OBJECT_VERSION			0x00000001
#define FST_OBJECT_MAGIC			0x00545346
#define FST_OBJECT_VERSION			0x00000001
#define SST_OBJECT_MAGIC			0x00535346
#define SST_OBJECT_VERSION			0x00000001

#define READ_F32(var, ptr)			var = *((f32 *)ptr); ptr = &ptr[sizeof(f32)];
#define READ_U32(var, ptr)			var = *((u32 *)ptr); ptr = &ptr[sizeof(u32)];
#define READ_U16(var, ptr)			var = *((u16 *)ptr); ptr = &ptr[sizeof(u16)];
#define READ_STR(str, ptr)			str = (char *)ptr; ptr = &ptr[STRLEN(str)+1];
#define SKIP_STRUCT(type, ptr)		ptr = &ptr[sizeof(type)];
#define READ_STRUCT(out, type, ptr)	out = static_cast<type *>((void *)ptr); SKIP_STRUCT(type, ptr);

namespace Seed {

class File;
extern const char *g_pcPlatform[];

struct SEED_CORE_API ObjectHeader
{
	u32 magic;
	u32 version;
	u32 platform;
};

struct SEED_CORE_API ImageObjectHeader
{
	u32 width;
	u32 height;
	u32 colorkey;
	u32 padding01;
	u32 padding02;
};

struct SEED_CORE_API SpriteObjectHeader
{
	//ObjectHeader block;
	u32 animation_amount;
	u32 flags;

	SpriteObjectHeader()
		: animation_amount(0)
		, flags(0)
	{}
};
/*
struct SEED_CORE_API AnimationObjectHeader
{
	//ObjectHeader block;
	u32 frame_amount;
	eAnimationFlag flags;
	u32 animId;
};

struct SEED_CORE_API FrameObjectHeader
{
	//ObjectHeader block;
	u32 time;
	u32 x, y;
	u32 w, h;
	u32 resW, resH;
	u32 frameId; // frame name
	u32 fileId; // filename
};
*/
struct SEED_CORE_API PackageObjectHeader
{
	//ObjectHeader block;
	u32 filesAmount;
	// packages FS table
};

struct SEED_CORE_API MusicObjectHeader
{
	//ObjectHeader block;
	u32 volume;
};

struct SEED_CORE_API SoundObjectHeader
{
	//ObjectHeader block;
	u32 volume;
	u32 flags;
};

struct SEED_CORE_API ButtonObjectHeader
{
	//ObjectHeader block;
	u32		id;
	u32		priority;
	f32		x;
	f32		y;
	u32		masktype;
	u32		labelid;
	//const char *sprite;
	//const char *mask;
};

struct SEED_CORE_API DictionaryObjectHeader
{
	//ObjectHeader block;
	u32 language;
	u32 glyphs;
	u32 strings;
	// glyphs table
	// string table
	//   string null-terminated
	//   string null-terminated
	//   ...
};

struct SEED_CORE_API FontObjectHeader
{
	//ObjectHeader block;
	f32 tracking;
	f32 spacing;
	f32 space;
	// u32 characters;
	// const char *pSprite;
};

BOOL seed_validate_block(File *file, ObjectHeader *block, u32 magic, u32 version);

} // namespace

#endif // __FORMATS_H__
