COMPILER      = gcc
CFLAGS        = -g
SRCDIR        = ./src
SOURCES       = $(wildcard $(SRCDIR)/*.c)
LIBRARYDIR    = ../lib
LIBRARIES     = $(wildcard $(LIBRARYDIR)/*.c)
SOURCE_NAMES  = $(subst $(SRCDIR)/,,$(SOURCES))
TARGETDIR     = ./bin

all:
	$(foreach NAME,$(SOURCE_NAMES),$(COMPILER) $(CFLAG) $(SRCDIR)/$(NAME) $(LIBRARIES) -o $(TARGETDIR)/$(NAME:.c=);)

clean:
	rm -rf $(TARGETDIR)/*
