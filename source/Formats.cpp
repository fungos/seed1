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
BOOL SEED_CORE_API seed_validate_block(File *file, ObjectHeader *block, u32 magic, u32 version)
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
