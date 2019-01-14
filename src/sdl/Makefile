SDL_CFLAGS = `sdl-config --cflags`
CFLAGS = -Wall -ansi -pedantic -O2 -ffast-math -funroll-loops \
	-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG \
	-I. -I.. -I../../data/assets $(SDL_CFLAGS) -DUSE_SDL -DUSE_SDL_MIXER -DUSE_NET

OBJS = gfx.o interrpt.o sfx.o input.o
TARGET = ../sdl.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar cru $(TARGET) $(OBJS)
	ranlib $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

