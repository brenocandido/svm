# Windows find is different from shell find, so specify the location of your shell find installation
# MSYS2 can install it for you

ifeq ($(OS), Windows_NT)
	FIND := C:/msys64/usr/bin/find.exe
else
	FIND := find
endif

TARGET		:= main
PSIM		:= psim
FLAGS		:= -Wall -std=c11 -Wextra -Wconversion -fmessage-length=0 -fsingle-precision-constant -Wmissing-declarations -Wfloat-equal -lm -g

SRCDIR		:= ./src
ODIR		:= ./obj
APP			:= main
CAPP		:= $(SRCDIR)/$(APP).c
OMAIN		:= $(ODIR)/$(TARGET).o
INCLUDES 	:= $(shell $(FIND) lib/inc inc -type d)
INCDIRS		:= $(patsubst %,-I %,$(INCLUDES))

LIB 		:= ./lib/bin/libSVM.a
LIBMAKE		:= ./lib

CC			:= gcc

.PHONY: all run psim clean cleandata cleanlib cleanall lib

all: cleanall lib build psim

build: $(TARGET)

psim:
	$(CC) -shared -o $(PSIM)_.dll $(SRCDIR)/$(PSIM).c $(FLAGS) $(LIB) $(INCDIRS)

run: all
	./$(TARGET)

$(TARGET): $(OMAIN) | lib
	$(CC) $^ $(LIB) -o $@

$(OMAIN): | $(ODIR)

$(OMAIN): $(CAPP)
	$(CC) -c $< -o $@ $(FLAGS) $(INCDIRS)

$(ODIR): 
	mkdir -p $@

lib: 
	$(MAKE) -C $(LIBMAKE)

clean:
	rm -rf $(TARGET)
	rm -rf $(ODIR)
	rm -rf *.dll
	rm -rf *.exe

cleandata:

cleanlib:
	$(MAKE) clean -C $(LIBMAKE)

cleanall: clean cleandata cleanlib
