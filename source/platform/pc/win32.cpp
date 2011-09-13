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

#include "platform/pc/platform.h"

#if defined(WIN32)

#include "Defines.h"
#include "Log.h"
#include "SeedInit.h"


#pragma push_macro("Delete")
#pragma push_macro("BOOL")
#pragma push_macro("SIZE_T")
#undef Delete
#if defined(_MSC_VER)
#undef BOOL
#endif
#undef SIZE_T
#include <io.h>
#include <windows.h>
#include <tchar.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("BOOL")
#pragma pop_macro("Delete")

#define TAG	"[Platform] "

#define SYSTEM_USERNAME_MAXLEN		1024
#define SYSTEM_USERHOME_MAXLEN		1024
#define SYSTEM_USERAPPDATA_MAXLEN	1024
#define SYSTEM_USERSAVEGAME_MAXLEN	1024

#define SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN	L"Saved Games"
#define SYSTEM_SAVEGAME_FOLDER_XP			L""

#define SYSTEM_APPDATA_FOLDER_VISTA_SEVEN	L"%LOCALAPPDATA%"
#define SYSTEM_APPDATA_FOLDER_XP			L"%APPDATA%"

#define CREATE_MUTEX_INITIAL_OWNER 0x00000001

static wchar_t strUserName[SYSTEM_USERNAME_MAXLEN];
static wchar_t strUserHomePath[SYSTEM_USERHOME_MAXLEN];
static wchar_t strUserAppDataPath[SYSTEM_USERAPPDATA_MAXLEN];
static wchar_t strUserSaveGamePath[SYSTEM_USERSAVEGAME_MAXLEN];
static bool bIsVistaOrSeven = false;

void system_version()
{
	OSVERSIONINFO os;

	os.dwOSVersionInfoSize = sizeof(os);
	GetVersionEx(&os);

	bIsVistaOrSeven = false;
	if (os.dwMajorVersion >= 6)
		bIsVistaOrSeven = true;
}

BOOL create_directory(const wchar_t *path)
{
	CreateDirectoryW(path, NULL);
	u32 err = GetLastError();

	return (err == 0);
}

const wchar_t *get_user_name()
{
	u32 len = SYSTEM_USERNAME_MAXLEN;
	memset((void *)strUserName, '\0', sizeof(strUserName));

	if (!GetUserNameA((LPSTR)&strUserName[0], (LPDWORD)&len))
	{
		return L"Noname";
	}
	return strUserName;
}

const wchar_t *get_user_home_folder()
{
	memset((void *)strUserHomePath, '\0', sizeof(strUserHomePath));
	u32 count = ExpandEnvironmentStringsW(L"%USERPROFILE%\\", (LPWSTR)&strUserHomePath[0], SYSTEM_USERHOME_MAXLEN);
	if (count > SYSTEM_USERHOME_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user home folder (too big!)");
		return L"./";
	}

	return strUserHomePath;
}

const wchar_t *get_user_savegame_folder()
{
	const wchar_t *s = get_user_home_folder();

	memset((void *)strUserSaveGamePath, '\0', sizeof(strUserSaveGamePath));

	system_version();
	if (bIsVistaOrSeven)
		_snwprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, L"%s/%s/", s, SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN);
	else
		_snwprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, L"%s/", s);

	return strUserSaveGamePath;
}

const wchar_t *get_user_appdata_folder()
{
	memset((void *)strUserAppDataPath, '\0', sizeof(strUserAppDataPath));
	u32 count = 0;

	system_version();
	if (bIsVistaOrSeven)
		ExpandEnvironmentStringsW(SYSTEM_APPDATA_FOLDER_VISTA_SEVEN L"\\", (LPWSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);
	else
		ExpandEnvironmentStringsW(SYSTEM_APPDATA_FOLDER_XP L"\\", (LPWSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);

	if (count > SYSTEM_USERAPPDATA_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user appdata folder (too big!)");
		return L"./";
	}

	return strUserAppDataPath;
}

void get_current_directory(wchar_t *buff, int size)
{
	(void)_wgetcwd(buff, size);
}

BOOL change_directory(const wchar_t *to)
{
	return (_wchdir(to) == 0);
}

void print_system_info()
{
	OSVERSIONINFO os;
	MEMORYSTATUS mem;

	os.dwOSVersionInfoSize = sizeof(os);
	GetVersionEx(&os);
	Info(TAG "Windows %ld.%ld.%ld (%s)", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber, os.szCSDVersion);

	GlobalMemoryStatus(&mem);
	Info(TAG "Memory: %ldKb total, %ldKb free", mem.dwTotalPhys/1024L, mem.dwAvailPhys/1024L);
}

BOOL system_check_multiple_instance()
{
#if !defined(_QT_)
	DWORD error = 0;

	HANDLE handleProcess;
	LPCTSTR lpName = (LPCTSTR)Seed::pConfiguration->GetApplicationTitle();

	handleProcess = CreateMutex(NULL, CREATE_MUTEX_INITIAL_OWNER, lpName);
	error = GetLastError();
	if (!handleProcess)
	{
		Log(TAG "ERROR! could not create a object mutex. Error:%d ", error);
		return TRUE;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		HWND hWnd = FindWindow(NULL, Seed::pConfiguration->GetApplicationTitle());
		if (hWnd)
		{
			if (Seed::pConfiguration->GetWarningMultipleInstances())
			{
				MessageBox(NULL, "There is already an instance of this application running!", Seed::pConfiguration->GetApplicationTitle(), MB_ICONWARNING);
			}
			SwitchToThisWindow(hWnd, FALSE);
			ShowWindow(hWnd, SW_RESTORE);
			return FALSE;
		}
	}
#else
	#warning "Implementar 'system_check_multiple_instance' no Qt"
#endif
	return TRUE;
}

#endif // WIN32
