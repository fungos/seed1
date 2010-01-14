find_path(SEED_INCLUDE_DIR Seed.h 
	HINTS
		$ENV{SEED_PATH}
		$ENV{SEEDPATH}
	PATHS
		/usr/include/seed
		/usr/local/include/seed
		~/dev/seed/seed/include
		/home/fungos/dev/seed/seed/include)

find_library(SEED_LIBRARY 
	NAMES seed
	HINTS
		$ENV{SEED_PATH}
		$ENV{SEEDPATH}
	PATH 
		/usr/lib 
		/usr/local/lib 
		~/dev/seed/seed
		/home/fungos/dev/seed/seed)

if (SEED_INCLUDE_DIR AND SEED_LIBRARY)
	set(SEED_FOUND true)
endif (SEED_INCLUDE_DIR AND SEED_LIBRARY)

if (SEED_FOUND)
	if (NOT Seed_FIND_QUIETLY)
		message(STATUS "Seed SDK Found: ${SEED_LIBRARY}")
	endif (NOT Seed_FIND_QUIETLY)
else (SEED_FOUND)
	if (Seed_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Seed SDK")
	endif (Seed_FIND_REQUIRED)
endif (SEED_FOUND)
