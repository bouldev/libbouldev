/* Bouldev 2022 */
#ifndef __LIBBOULDEV_PRIVATE_H__
#define __LIBBOULDEV_PRIVATE_H__

#ifndef BUILDING_LIBBOULDEV
#error "This header should only be used when building libbouldev"
#endif

#include <stdio.h>
#include <stdlib.h>

#define HIDDEN __attribute__((visibility("hidden")))

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
	char *cmdline; // Command to execute
	int ret; // Return code buffer
	char *out; // stdout buffer
} boul_cmd;

HIDDEN bool run_cmd(boul_cmd buf);
HIDDEN bool path_exists(const char *path);
HIDDEN bool path_is_dir(const char *path);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __has_include
#if __has_include(<libbouldev_private.h>)
#include <libbouldev_private.h>
#endif
#endif

#endif
