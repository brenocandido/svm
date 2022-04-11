# Windows find is different from shell find, so specify the location of your shell find installation
# MSYS2 can install it for you

ifeq ($(OS), Windows_NT)
	FIND := C:/msys64/usr/bin/find.exe
else
	FIND := find
endif

FLAGS 			:= -Wall -std=c11 -Wextra -fmessage-length=0 -fsingle-precision-constant -Wmissing-declarations -lm -g
TARGET 			:= libSVM

CC 				:= gcc
LIBFLAGS 		:= rs
ODIR 			:= ./obj
SRCDIR 			:= ./src
INCDIR 			:= ./inc
BINDIR 			:= ./bin

LIB				:= $(BINDIR)/$(TARGET).a

SOURCES 		:= $(shell $(FIND) -name "*.c")
OBJECTS 		:= $(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(SOURCES))

HEADERSDIR 	 	:= $(shell $(FIND) inc -type d)
DEPENDENCIES 	:= $(patsubst %,-I %,$(HEADERSDIR))

all: $(LIB)

$(LIB): $(OBJECTS) | $(BINDIR)
	ar $(LIBFLAGS) $@ $^

$(ODIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h | $(ODIR)
	$(CC) -c $< -o $@ $(FLAGS) $(DEPENDENCIES)

$(ODIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJECTS)
	rm -rf $(BINDIR)/$(TARGET).a
	rm -rf $(ODIR)
	rm -rf $(BINDIR)