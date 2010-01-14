cmake_minimum_required (VERSION 2.6)

project (Seed)
set (Seed_VERSION_MAJOR 0)
set (Seed_VERSION_MINOR 1)

set (ARCHIVE_OUTPUT_DIRECTORY "${Seed_SOURCE_DIR}/bin/")
set (LIBRARY_OUTPUT_DIRECTORY "${Seed_SOURCE_DIR}/bin/")
set (OUTPUT_DIRECTORY_LIB "bin/")

# Compiling mode
option(BUILD_ROM			"Set Shipping Mode"						OFF)
option(SEED_BUILTIN			"Statically linked"						ON)

# Complementary code
option(SEED_USE_YMALLOC 	"Use yMemoryManager"	 				ON)
option(SEED_USE_LZF 		"Use LZF compression/decompression" 	OFF)
option(SEED_USE_THEORA		"Use Theora video"						ON)

# Platform specific code - only one! (Cmake supports multiple choices?)
option(PLATFORM_SDL				"Use SDL"								ON)
option(PLATFORM_QT				"Use Qt"								OFF)
option(PLATFORM_WII				"Use Rvl SDK"							OFF)
option(PLATFORM_IPHONE			"Use iPhone SDK"						OFF)


set(ENABLE_BUILTIN 0)
if (SEED_BUILTIN)
	set(ENABLE_BUILTIN 1)
endif (SEED_BUILTIN)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG=1)
	add_definitions(-D_DEBUG=1)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

if (CMAKE_BUILD_TYPE MATCHES Release)
	if (BUILD_ROM)
		add_definitions(-DROM=1)
		add_definitions(-D_ROM=1)
	else (BUILD_ROM)
		add_definitions(-DRELEASE=1)
		add_definitions(-D_RELEASE=1)
	endif (BUILD_ROM)
endif (CMAKE_BUILD_TYPE MATCHES Release)

configure_file(AutoConfig.h.in "${Seed_SOURCE_DIR}/include/AutoConfig.h" DESTINATION include)

include_directories(${Seed_SOURCE_DIR}/include)
include_directories(${Seed_BINARY_DIR})
include_directories(/opt/local/include)
include_directories(/usr/local/include)
include_directories(/usr/include)
include_directories(/Developer/SDKs/MacOSX10.5.sdk/usr/X11/include)

set(SEED_SRC_DIR "source/")
set(SEED_INC_DIR "include/")
include(CMakeSrc.txt)

add_library(seed STATIC ${SEED_SRC_CORE})
