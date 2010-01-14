find_path(VORBIS_INCLUDE_DIR vorbisfile.h 
	PATHS
		/usr/include/vorbis
		/usr/local/inclue/vorbis)

find_library(VORBIS_LIBRARY 
	NAMES vorbisfile
	PATH 
		/usr/lib 
		/usr/local/lib) 

if (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)
	set(VORBIS_FOUND true)
endif (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)

if (VORBIS_FOUND)
	if (NOT Vorbis_FIND_QUIETLY)
		message(STATUS "Vorbis Found: ${VORBIS_LIBRARY}")
	endif (NOT Vorbis_FIND_QUIETLY)
else (VORBIS_FOUND)
	if (Vorbis_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Vorbis")
	endif (Vorbis_FIND_REQUIRED)
endif (VORBIS_FOUND)
