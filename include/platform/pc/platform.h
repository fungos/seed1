#ifndef __PC_PLATFORM_H__
#define __PC_PLATFORM_H__

#include "Defines.h"

BOOL create_directory(const char *path);
const char *get_user_name();
const char *get_user_appdata_folder();
const char *get_user_savegame_folder();
const char *get_user_home_folder();
void get_current_directory(char *buff, int size);
BOOL change_directory(const char *to);
void print_system_info();

#endif // __PC_PLATFORM_H__
