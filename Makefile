
DESTDIR ?= 
PREFIX  ?= /usr
COMPRESS_MAN ?= yes
STRIP_BINARY ?= yes

CSECFLAGS ?= -fstack-protector-all -Wall --param ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -fstack-check
CFLAGS ?= -pipe -O2
CFLAGS += $(CSECFLAGS)
DEBUGCFLAGS ?= -pipe -Wall -Werror -ggdb3 -Wno-error=unused-variable $(CSECFLAGS)

CARCHFLAGS ?= -march=native

NORMSYSTEMCFLAGS ?= -std=gnu11 $(CARCHFLAGS)
OLDSYSTEMCFLAGS  ?= -std=gnu99 -DOLDSYSTEM

LIBS := 
LDSECFLAGS ?= -Xlinker -zrelro
LDFLAGS += $(LDSECFLAGS)
INC := $(INC)

INSTDIR = $(DESTDIR)$(PREFIX)

objs=\
main.o\

binary=bypassmount

all: $(objs)
	$(CC) $(NORMSYSTEMCFLAGS) $(CFLAGS) $(LDFLAGS) $(objs) $(LIBS) -o $(binary)

%.o: %.c
	$(CC) $(NORMSYSTEMCFLAGS) $(CFLAGS) $(INC) $< -c -o $@

clean:
	rm -f $(binary) $(objs)

distclean: clean

doc:
	doxygen .doxygen

install:
	install -d "$(INSTDIR)/bin" "$(INSTDIR)/share/man/man1"
ifeq ($(STRIP_BINARY),yes)
	strip --strip-unneeded -R .comment -R .GCC.command.line -R .note.gnu.gold-version $(binary)
endif
	install -m 755 $(binary) "$(INSTDIR)"/bin/
	install -m 644 man/man1/bypassmount.1 "$(INSTDIR)"/share/man/man1/
ifeq ($(COMPRESS_MAN),yes)
	rm -f "$(INSTDIR)"/share/man/man1/bypassmount.1.gz
	gzip "$(INSTDIR)"/share/man/man1/bypassmount.1
endif

deinstall:
	rm -f "$(INSTDIR)"/bin/$(binary) "$(INSTDIR)"/share/man/man1/bypassmount.1.gz

