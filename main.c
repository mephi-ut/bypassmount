/*
    bypassmount - utility to work bypass of some mountpoint

    Copyright (C) 2013  Dmitry Yu Okunev <dyokunev@ut.mephi.ru> 0x8E30679C

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/mount.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define STACK_SIZE (1024 * 1024)

enum flags {
	DEFAULT = 0x00,
	QUIET	= 0x01,
};
typedef enum flags flags_t;

static flags_t flags = DEFAULT;

static inline int my_execv(const char *path, char *const argv[]) {
	int rc;

	rc = execv(path, argv);
	if(!(flags&QUIET))
		fprintf(stderr, "Cannot execv(\"%s\", argv): %s (errno: %i)\n", path, strerror(errno), errno);

	return rc;
}

static int umount_and_exec(char **argv) {
	char *mountpoint = argv[0];
	char *cmd        = argv[1];

	if (unshare(CLONE_NEWNS) == -1) {
		if(!(flags&QUIET))
			fprintf(stderr, "Cannot unshare(CLONE_NEWNS): %s (errno: %i)\n", strerror(errno), errno);
		return errno;
	}

	if(mountpoint == NULL)
		return EINVAL;

	if(umount2(mountpoint, MNT_DETACH)) {
		if(!(flags&QUIET))
			fprintf(stderr, "Cannot umount2(\"%s\", MNT_DETACH) in the new namespace: %s (errno: %i)\n", mountpoint, strerror(errno), errno);
		return errno;
	}
	if(cmd == NULL) {
		char *argv_exec[2] = { NULL };
		cmd = getenv("SHELL");
		argv_exec[1] = cmd;
		return my_execv(cmd, argv_exec);
	}

	char **argv_exec = &argv[1];

	return my_execv(cmd, argv_exec);
}

void syntax() {
	fprintf(stderr, "\tsyntax: bypassmount [-q] <mount point> -- [shell] [shell arg0] [...]\n");
}

int main(int argc, char *argv[]) {
	int opt;
	while((opt = getopt(argc, argv, "q")) != -1) {
		switch(opt) {
			case 'q':
				flags |= QUIET;
				break;
			default:
				fprintf(stderr, "Invalid option.\n");
				syntax();
				return EINVAL;
		}
	}

	if(argc-1 < optind) {
		if (!(flags&QUIET)) {
			fprintf(stderr, "Not enough arguments.\n");
			syntax();
		}
		return EINVAL;
	}

	argv=&argv[optind];

	int rc = umount_and_exec(argv);

	return rc;
}

