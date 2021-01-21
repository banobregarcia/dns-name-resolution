SHELL = /bin/bash

# set suffix list, to prevent confusion between different make programs
# line 17 clears an implied suffix list, and line 18 sets a new one
.SUFFIXES:
.SUFFIXES: .c .h .o

DEBUG = yes
SHELL = /bin/bash

# project set up and directories
CC	= gcc
INCLDIR	= include/
BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/

# final executable name
_BIN	= a.out
BIN	= $(addprefix $(BINDIR), $(_BIN))

# files; here all object files will be stored in $(OBJDIR), with the
# same base names as corresponding c files from SRCDIR
SRC	= $(wildcard src/*.c)
_OBJS	= $(patsubst src/%.c, %.o, $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(_OBJS))

# compilation flags

ifeq ($(DEBUG),yes)
	CFLAGS= -Wall -Werror -Wextra -g -I $(INCLDIR)
	OFLAGS =
	LDFLAGS=
else
	CFLAGS= -Wall -Werror -Wextra -I $(INCLDIR)
	OFLAGS =
	LDFLAGS=
endif


# compile binary and object files
.PHONY: all
all: $(BIN)

$(BIN): $(BINDIR) $(OBJS)
	$(CC) $(OFLAGS) $(OBJS) -o $(BIN)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJS): $(OBJDIR) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)


# generate docs with doxygen
DOCDIR = docs/
HTMLDIR = html/
LTXDIR = latex/

# .PHONY: docs
docs: Doxyfile
	doxygen


# clean entire project directory
# .PHONY: clean
clean:
	-rm -rf $(BINDIR) $(OBJDIR) $(DOCDIR) $(HTMLDIR) $(LTXDIR)

test: all
	@for i in test-*.sh ; do echo $$i ; sh $$i || exit 1 ; done

# check code quality
# .PHONY: memcheck

memcheck: all
	valgrind -v --show-leak-kinds=all --leak-check=full --track-origins=yes \
	./$(BIN)

# debugging make
print-% :
	@echo $* = $($*)
