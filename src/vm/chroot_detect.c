#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>

static int get_inode(void){
	struct stat buf;
	fstat(open("/", O_RDONLY), &buf);
	return buf.st_ino;
}

bool os_chrooted(void){
#ifndef _WIN32
	if (get_inode() != 2) return true;
#endif
	return false;
}
