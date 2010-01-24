PROJECT=libseed.a
include Objects.in

TARGET = ../$(PROJECT)

GCC = g++
CFLAGS = -Weffc++ -Wall -fPIC `sdl-config --cflags` $(CDEFS) -I/media/d/SDK/seed/include -I/media/d/SDK/seeddemos/common/sdl -g
CDEFS = -D _SDL_ -D RELEASE #-D DEBUG
LFLAGS = `sdl-config --libs` -L/bin
CPPFLAGS = $(CFLAGS)
LIBS = -L/media/d/SDK/seed
AR = ar
AR_PARAM = cru

all:	$(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)
#	$(GCC) -shared -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) *~
