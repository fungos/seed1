#include "platform/pc/platform.h"

#if defined(WIN32)

#include "Defines.h"
#include "Log.h"

#include <io.h>
#include <windows.h>
#include <tchar.h>


#define TAG	"[Platform] "


#define SYSTEM_USERNAME_MAXLEN		1024
#define SYSTEM_USERHOME_MAXLEN		1024
#define SYSTEM_USERAPPDATA_MAXLEN	1024
#define SYSTEM_USERSAVEGAME_MAXLEN	1024

#define SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN	"Saved Games"
#define SYSTEM_SAVEGAME_FOLDER_XP			""

#define SYSTEM_APPDATA_FOLDER_VISTA_SEVEN	"%LOCALAPPDATA%"
#define SYSTEM_APPDATA_FOLDER_XP			"%APPDATA%"

static char	strUserName[SYSTEM_USERNAME_MAXLEN];
static char strUserHomePath[SYSTEM_USERHOME_MAXLEN];
static char strUserAppDataPath[SYSTEM_USERAPPDATA_MAXLEN];
static char strUserSaveGamePath[SYSTEM_USERSAVEGAME_MAXLEN];
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

BOOL create_directory(const char *path)
{
	CreateDirectoryA(path, NULL);
	u32 err = GetLastError();

	return (err == 0);
}

const char *get_user_name()
{
	u32 len = SYSTEM_USERNAME_MAXLEN;
	memset((void *)strUserName, '\0', sizeof(strUserName));

	if (!GetUserNameA((LPSTR)&strUserName[0], (LPDWORD)&len))
	{
		return "Noname";
	}
	return strUserName;
}

const char *get_user_savegame_folder()
{
	const char *s = get_user_home_folder();

	memset((void *)strUserSaveGamePath, '\0', sizeof(strUserSaveGamePath));

	system_version();
	if (bIsVistaOrSeven)
		snprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, "%s/%s/", s, SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN);
	else
		snprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, "%s/", s);

	return strUserSaveGamePath;
}

const char *get_user_appdata_folder()
{
	memset((void *)strUserAppDataPath, '\0', sizeof(strUserAppDataPath));
	u32 count = 0;

	system_version();
	if (bIsVistaOrSeven)
		ExpandEnvironmentStringsA(SYSTEM_APPDATA_FOLDER_VISTA_SEVEN "\\", (LPSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);
	else
		ExpandEnvironmentStringsA(SYSTEM_APPDATA_FOLDER_XP "\\", (LPSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);

	if (count > SYSTEM_USERAPPDATA_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user appdata folder (too big!)");
		return "./";
	}

	return strUserAppDataPath;
}

const char *get_user_home_folder()
{
	memset((void *)strUserHomePath, '\0', sizeof(strUserHomePath));
	u32 count = ExpandEnvironmentStringsA("%USERPROFILE%\\", (LPSTR)&strUserHomePath[0], SYSTEM_USERHOME_MAXLEN);
	if (count > SYSTEM_USERHOME_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user home folder (too big!)");
		return "./";
	}

	return strUserHomePath;
}

void get_current_directory(char *buff, int size)
{
	(void)getcwd(buff, size);
}

BOOL change_directory(const char *to)
{
	return (chdir(to) == 0);
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


#endif // WIN32
