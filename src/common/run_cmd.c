/* Bouldev 2022 */
#include <libbouldev.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/param.h>
bool run_cmd(boul_cmd buf)
{
	FILE *fp;
	char path[MAXPATHLEN];

	/* Open the command for reading. */
	fp = popen(buf.cmdline, "r");
	if (fp == NULL) {
		char *errmsg;
		if (!errno) {
			errmsg = "memory allocation failed";
		} else {
			errmsg = strerror(errno);
		}
		fprintf(stderr, "Failed to run command \"%s\" (%s)\n", buf.cmdline, errmsg);
		return false;
	}

	/* Read the output a line at a time - output it. */
	while (fgets(path, sizeof(path), fp) != NULL) {
		strcat(buf.out, path);
	}

	/* close */
	// BSD's WEXITSTATUS needs a writable int (int to int* conversion)
	int closecmd = pclose(fp);
	buf.ret = WEXITSTATUS(closecmd);

	return true;
}
