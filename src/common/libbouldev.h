/* Bouldev 2022 */
#ifndef __LIBBOULDEV_H__
#define __LIBBOULDEV_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool os_is_vm(void);
char *os_type(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __has_include
#if __has_include(<libbouldev_private.h>)
#include <libbouldev_private.h>
#endif
#endif

#endif
