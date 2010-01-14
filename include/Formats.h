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

struct ObjectHeader
{
	u32 magic;
	u32 version;
	u32 platform;
};

struct ImageObjectHeader
{
	u32 width;
	u32 height;
	u32 colorkey;
	u32 padding01;
	u32 padding02;
};

struct SpriteObjectHeader
{
	//ObjectHeader block;
	u32 animation_amount;
	u32 flags;
	
	SpriteObjectHeader() : animation_amount(0), flags(0) {}
};
/*
struct AnimationObjectHeader
{
	//ObjectHeader block;
	u32 frame_amount;
	eAnimationFlag flags;
	u32 animId;
};

struct FrameObjectHeader
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
struct PackageObjectHeader
{
	//ObjectHeader block;
	u32 filesAmount;
	// packages FS table
};

struct MusicObjectHeader
{
	//ObjectHeader block;
	u32 volume;
};

struct SoundObjectHeader
{
	//ObjectHeader block;
	u32 volume;
	u32 flags;
};

struct ButtonObjectHeader
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

struct DictionaryObjectHeader
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

struct FontObjectHeader
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


