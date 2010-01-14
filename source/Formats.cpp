#include "Formats.h"
#include "Log.h"
#include "File.h"


#define TAG		"[Formats] "


namespace Seed {


const char *g_pcPlatform[] =
{
	"Nintendo DS",
	"Nintendo Wii",
	"Apple iPhone",
	"Generic SDL",
	"Win32",
	"MacOSX",
	"Linux 32",
	"Sony PSP"
};


/* Usado para checkar header de bloco dos objetos */
BOOL seed_validate_block(File *file, ObjectHeader *block, u32 magic, u32 version)
{
	if (block->magic != magic)
	{
		Log(TAG "%s: Invalid block magic 0x%x, magic 0x%x was expected.", file->GetName(), block->magic, magic);
		pFileSystem->Close(file);
		ASSERT(0);
	}
	else if (block->version != version)
	{
		Log(TAG "%s: Block is version %d, but version %d is required.", file->GetName(), block->version, version);
		pFileSystem->Close(file);
		ASSERT(0);
	}
	else if (block->platform != PLATFORM_CODE)
	{
		Log(TAG "%s: Invalid platform (%s).", file->GetName(), g_pcPlatform[block->platform]);
		pFileSystem->Close(file);
		ASSERT(0);
	}

	return TRUE;
}


}
