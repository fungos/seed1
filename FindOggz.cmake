find_path(OGGZ_INCLUDE_DIR oggz.h 
	PATHS
		/usr/include/oggz
		/usr/local/include/oggz)

find_library(OGGZ_LIBRARY 
	NAMES oggz
	PATH 
		/usr/lib 
		/usr/local/lib)

if (OGGZ_INCLUDE_DIR AND OGGZ_LIBRARY)
	set(OGGZ_FOUND true)
endif (OGGZ_INCLUDE_DIR AND OGGZ_LIBRARY)

if (OGGZ_FOUND)
	if (NOT Oggz_FIND_QUIETLY)
		message(STATUS "Oggz Found: ${OGGZ_LIBRARY}")
	endif (NOT Oggz_FIND_QUIETLY)
else (OGGZ_FOUND)
	if (Oggz_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Oggz")
	endif (Oggz_FIND_REQUIRED)
endif (OGGZ_FOUND)
