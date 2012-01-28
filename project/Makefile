# reference
# http://git.kernel.org/?p=git/git.git;a=blob_plain;f=Makefile;hb=c965c029330b1f81cc107c5d829e7fd79c61d8ea

NAME=Seed

AR = ar
AR_PARAM = cru
AS = as
NM = nm
STRIP = strip
LD = ld
GCC = g++
CC = gcc

CDEFS = -D_SDL_ -DSEED_BUILD
CFLAGS = -Wall -shared

ifneq ($(mode),release)
	CFLAGS += -g
	CDEFS += -DDEBUG -DYMEM_DEBUG -DYMEM_PARANOID_CHECK -DSEED_ENABLE_PROFILER
	mode = debug
endif

ifeq ($(license),commercial)
	CDEFS += -DSEED_USE_COMMERCIAL
	COMMERCIAL = YesPlease
else
	license = LGPL
endif

CFLAGS += $(CDEFS) -I$(SEEDSDK)/seed/include -I$(SEEDSDK)/seeddemos/common/sdl -I.. -I../contrib
CPPFLAGS = $(CFLAGS)
LIBS = -lSeed -lSDL -lSDL_image
LFLAGS = -L$(SEEDSDK)/seed/bin

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
uname_M := $(shell sh -c 'uname -m 2>/dev/null || echo not')
uname_O := $(shell sh -c 'uname -o 2>/dev/null || echo not')
uname_R := $(shell sh -c 'uname -r 2>/dev/null || echo not')
uname_P := $(shell sh -c 'uname -p 2>/dev/null || echo not')
uname_V := $(shell sh -c 'uname -v 2>/dev/null || echo not')

ifeq ($(uname_S),Linux)
	PLATFORM = Linux
	CDEFS += -DLINUX
	CFLAGS += -fPIC `sdl-config --cflags` -I/usr/local/include
	LFLAGS += -L/usr/local/lib `sdl-config --libs`
	LIBS += -lopenal -lGL -logg -loggz -loggplay -ltheora -lvorbis -lvorbisfile
	#USE_SYSTEM_OGGVORBIS = YesPlease
endif
ifeq ($(uname_S),Darwin)
	PLATFORM = MacOSX
	CFLAGS += -fPIC `sdl-config --cflags` -I/usr/local/include
	LFLAGS += -L/usr/local/lib `sdl-config --libs`
	LIBS += -lopenal -lGL
	USE_CONTRIB_OGGVORBIS = YesPlease
endif
ifneq (,$(findstring CYGWIN,$(uname_S)))
	# ???
endif
ifeq ($(uname_O),Cygwin)
	WINDOWS = UnfortunatelyYes
	PLATFORM = Windows
	USE_CONTRIB_OGGVORBIS = YesPlease
	DEPENDENCY = $(SEEDSDK)/dependency/qtsdk/
	CDEFS += -DWIN32
	LFLAGS += -L$(DEPENDENCY)lib
	LIBS += -lSDLmain -lopenal32 -ld3d8 -lopengl32
	CFLAGS += -I$(DEPENDENCY)/include
endif
ifneq (,$(findstring MINGW,$(uname_S)))
	WINDOWS = UnfortunatelyYes
	PLATFORM = Windows
	USE_CONTRIB_OGGVORBIS = YesPlease
	DEPENDENCY = $(SEEDSDK)/dependency/qtsdk/
	CDEFS += -DWIN32
	LFLAGS += -L$(DEPENDENCY)lib
	LIBS += -lSDLmain -lopenal32 -ld3d8 -lopengl32
	CFLAGS += -I$(DEPENDENCY)/include
endif

ifndef COMMERCIAL
ifdef WINDOWS
	TARGET = ../bin/lib$(NAME).dll
	LIBS += -lgdi32 -lkernel32 -lmingw32
	CFLAGS += -Wl,--out-implib,../bin/lib$(NAME).a
else
	TARGET = ../bin/lib$(NAME).a
endif
else
	TARGET = ../bin/lib$(NAME).a
endif

include Objects.in

#ifeq ($(strip $(SEEDSDK)),) #check empty string

all: $(TARGET)

$(TARGET): info $(OBJECTS)
ifdef WINDOWS
ifdef COMMERCIAL
	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)
else
	$(GCC) -o $(TARGET) $(OBJECTS) $(CPPFLAGS) $(LFLAGS) $(LIBS)
endif
else
	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)
endif

clean:
ifdef WINDOWS
	$(SEEDSDK)/seedtools/rmw -f $(OBJECTS) $(TARGET) *~
else
	rm -rf $(OBJECTS) $(TARGET) *~
endif

info:
	@echo
	@echo ------------------------------------------------------------------------------------------------------------
ifndef SEEDSDK
	@echo Define the environment variable SEEDSDK to point to the base of the full Seed Framework
else
	@echo Using SEEDSDK=$(SEEDSDK)
endif
	@echo Platform: $(PLATFORM)
	@echo Mode: $(mode)
	@echo License: $(license)
	@echo ------------------------------------------------------------------------------------------------------------
	@echo
