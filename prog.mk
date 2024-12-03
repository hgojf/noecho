.PHONY: all clean install install-man

CFLAGS += -MD -MP

SRCS ?= $(PROG).c
DEPS ?= $(SRCS:.c=.d)
OBJS ?= $(SRCS:.c=.o)

MAN ?= $(PROG).1

BINDIR ?= bin
MANDIR ?= man
PREFIX ?= /usr/local

BINMODE ?= 0755
MANMODE ?= 0544

all: $(PROG)

clean:
	rm -f $(DEPS) $(OBJS) $(PROG)

install: install-man
	$(INSTALL) -m $(BINMODE) $(PROG) $(PREFIX)/$(BINDIR)

install-man:
	@if [ -z "$(NOMAN)" ]; then \
		sect=$$(echo $(MAN) | cut -d '.' -f 2); \
		echo $(INSTALL) -m $(MANMODE) $(MAN) $(PREFIX)/$(MANDIR)/man$$sect; \
		$(INSTALL) -m $(MANMODE) $(MAN) $(PREFIX)/$(MANDIR)/man$$sect; \
	fi

$(PROG): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)
