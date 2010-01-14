#include "platform/pc/platform.h"

#if defined(__linux__) || defined(__APPLE_CC__)

#include "Log.h"

#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define TAG	"[Platform] "

BOOL create_directory(const char *path)
{
	BOOL ret = FALSE;
	int err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	switch (err)
	{
		case EACCES:
		{
			Info(TAG "Permission denied to create '%s'.", path);
		}
		break;

		case EEXIST:
		{
			Info(TAG "Path '%s' already exists.", path);
		}
		break;

		case ENAMETOOLONG:
		{
			Info(TAG "Path name too long: '%s'", path);
		}
		break;

		case ENOENT:
		{
			Info(TAG "Path '%s' does not name an existin entry.", path);
		}
		break;

		case ENOSPC:
		{
			Info(TAG "Not enought space to create '%s'.", path);
		}
		break;

		case ENOTDIR:
		{
			Info(TAG "A component of the path '%s' is not a directory.", path);
		}
		break;

		case EROFS:
		{
			Info(TAG "Read-only filesystem, could not create '%s'.", path);
		}
		break;

		case 0:
		{
			ret = TRUE;
		}
		break;

		default:
		{
			Info(TAG "An error '%d' ocurred trying to create '%s'.", err, path);
		}
		break;
	}

	return ret;
}

const char *get_user_name()
{
	const char *name = getenv("LOGNAME");
	if (!name)
		name = "Noname";
	return name;
}

const char *get_user_savegame_folder()
{
	return get_user_home_folder();
}

const char *get_user_appdata_folder()
{
	return get_user_home_folder();
}

const char *get_user_home_folder()
{
	const char *home = getenv("HOME");
	if (!home)
		home = "./";
	return home;
}

void get_current_directory(char *buff, int size)
{
	getcwd(buff, size);
}

BOOL change_directory(const char *to)
{
	return chdir(to);
}

void print_system_info()
{
}

#endif // _LINUX_
