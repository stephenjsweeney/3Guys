VERSION = 0.2
REVISION = 0
LOCALE_MO = $(patsubst %.po,%.mo,$(wildcard locale/*.po))

OUT = bin

SEARCHPATH += src
SEARCHPATH += src/atlas
SEARCHPATH += src/entities
SEARCHPATH += src/game
SEARCHPATH += src/json
SEARCHPATH += src/level
SEARCHPATH += src/system
SEARCHPATH += src/test
SEARCHPATH += src/util

vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS += defs.h structs.h

_OBJS += atlas.o
_OBJS += cJSON.o crumblingFloor.o
_OBJS += diamond.o door.o draw.o
_OBJS += effects.o entities.o
_OBJS += glRectangle.o glRectangleBatch.o guy.o
_OBJS += i18n.o io.o init.o input.o item.o
_OBJS += key.o
_OBJS += level.o levelLoader.o lookup.o
_OBJS += main.o maths.o movingPlatform.o
_OBJS += player.o pressurePlate.o pushBlock.o
_OBJS += sound.o spikeTrap.o sprites.o star.o strings.o
_OBJS += test.o text.o textures.o title.o tnt.o tools.o
_OBJS += util.o
_OBJS += wall.o wipe.o

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

# top-level rule to create the program.
all: $(PROG) $(LOCALE_MO)

# compiling other source files.
$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<
	
%.mo: %.po
	msgfmt -c -o $@ $<
	
# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) $(OBJS) $(PROG) $(LOCALE_MO)
