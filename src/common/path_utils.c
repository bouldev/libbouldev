#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libbouldev.h>

bool path_exists(const char *path)
{
        struct stat st;
        return stat(path, &st) == 0;
}

bool path_is_dir(const char *path)
{
        struct stat st;
        bool r = stat(path, &st);
        if(!r) return r;
        return S_ISDIR(st.st_mode);
}
