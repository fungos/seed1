PROJECT=libSeed.a
include Objects.in
TARGET = ../bin/debug/sdl/$(PROJECT)

AR = ar
AR_PARAM = cru
AS = as
NM = nm
STRIP = strip
LD = ld
GCC = g++
CFLAGS = -Wall -fPIC -g `sdl-config --cflags` $(CDEFS) -I/media/d/SDK/seed/include
CDEFS = -D _SDL_ -D _DEBUG_
LFLAGS = `sdl-config --libs` -L/bin -lSDL_image -lopenal -lGL -lseed -logg -lvorbis -lvorbisfile -loggz -loggplay -ltheora
CPPFLAGS = $(CFLAGS)
LIBS = 



all:	$(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET) *~
