find_path(OGGPLAY_INCLUDE_DIR oggplay.h
	PATHS
		/usr/include/oggplay
		/usr/local/include/oggplay)

find_library(OGGPLAY_LIBRARY 
	NAMES oggplay
	PATH 
		/usr/lib 
		/usr/local/lib)

if (OGGPLAY_INCLUDE_DIR AND OGGPLAY_LIBRARY)
	set(OGGPLAY_FOUND true)
endif (OGGPLAY_INCLUDE_DIR AND OGGPLAY_LIBRARY)

if (OGGPLAY_FOUND)
	if (NOT OggPlay_FIND_QUIETLY)
		message(STATUS "OggPlay Found: ${OGGPLAY_LIBRARY}")
	endif (NOT OggPlay_FIND_QUIETLY)
else (OGGPLAY_FOUND)
	if (OggPlay_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find OggPlay")
	endif (OggPlay_FIND_REQUIRED)
endif (OGGPLAY_FOUND)
