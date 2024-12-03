/*
 * Copyright (c) 2024 Henry Ford <fordhenry2299@gmail.com>

 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static void
usage(void)
{
	fprintf(stderr, "usage: noecho\n");
	exit(2);
}

int
main(int argc, char *argv[])
{
	struct termios io;
	int ch, echo, rv;

	rv = 1;

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch (ch) {
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 0)
		usage();

	if (pledge("stdio tty", NULL) == -1)
		err(1, "pledge");

	if (tcgetattr(STDIN_FILENO, &io) == -1)
		err(1, "tcgetattr");
	echo = io.c_lflag & ECHO;

	if (echo) {
		io.c_lflag &= ~ECHO;
		if (tcsetattr(STDIN_FILENO, TCSADRAIN, &io) == -1)
			err(1, "tcsetattr");
	}

	while ((ch = fgetc(stdin)) != EOF) {
		if (fputc(ch, stdout) == EOF) {
			warn("fputc");
			goto tio;
		}
	}

	rv = 0;
	tio:
	if (echo) {
		io.c_lflag |= echo;
		if (tcsetattr(STDIN_FILENO, TCSADRAIN, &io) == -1)
			err(1, "tcsetattr");
	}
	return rv;
}
