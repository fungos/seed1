find_path(THEORA_INCLUDE_DIR theora.h 
	PATHS
		/usr/include/theora
		/usr/local/inclue/theora)

find_library(THEORA_LIBRARY 
	NAMES theora
	PATH 
		/usr/lib 
		/usr/local/lib) 

if (THEORA_INCLUDE_DIR AND THEORA_LIBRARY)
	set(THEORA_FOUND true)
endif (THEORA_INCLUDE_DIR AND THEORA_LIBRARY)

if (THEORA_FOUND)
	if (NOT Theora_FIND_QUIETLY)
		message(STATUS "Theora Found: ${THEORA_LIBRARY}")
	endif (NOT Theora_FIND_QUIETLY)
else (THEORA_FOUND)
	if (Theora_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Theora")
	endif (Theora_FIND_REQUIRED)
endif (THEORA_FOUND)
