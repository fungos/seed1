find_path(OGG_INCLUDE_DIR ogg.h 
	PATHS
		/usr/include/ogg
		/usr/local/include/ogg)

find_library(OGG_LIBRARY 
	NAMES ogg
	PATH 
		/usr/lib 
		/usr/local/lib)

if (OGG_INCLUDE_DIR AND OGG_LIBRARY)
	set(OGG_FOUND true)
endif (OGG_INCLUDE_DIR AND OGG_LIBRARY)

if (OGG_FOUND)
	if (NOT Ogg_FIND_QUIETLY)
		message(STATUS "Ogg Found: ${OGG_LIBRARY}")
	endif (NOT Ogg_FIND_QUIETLY)
else (OGG_FOUND)
	if (Ogg_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Ogg")
	endif (Ogg_FIND_REQUIRED)
endif (OGG_FOUND)
