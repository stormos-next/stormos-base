/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 *
 * (c) Copyright 2013 Andrew Stormont.
 */

#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/fcntl.h>

#include "config.h"

/*
 * Look inside syscall.c for this
 */
extern char *__get_redirect(const char *);

static int block_file(const char *path)
{
	int len;

	/*
	 * Only block files being written to the protodir
	 */
	if (strncmp(path, PROTO_DIR_PATH, PROTO_DIR_LEN) != 0)
		return 0;

	/*
	 * It's an effing libtool archive.  Seriously guys.
	 */
	if ((len = strlen(path)) > 2 && path[len-3] == '.' 
	    && path[len-2] == 'l' && path[len-1] == 'a')
		return 1;

	/*
	 * No idea what the file is.  Assumes all's good.
	 */
	return 0;
}

int creat(const char *path, mode_t mode)
{
	static int (*_creat)(const char *, mode_t) = NULL;
	static int (*_open)(const char *, int) = NULL;
	int fd;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_creat == NULL)
		_creat = (int (*)(const char *, mode_t))dlsym(RTLD_NEXT, "creat");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (block_file(path))
	{
		/*
		 * Prefer to keep our own handle to the real open function
		 */
		if (_open == NULL)
			_open = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open");

		fd = _open("/dev/null", O_WRONLY);
	}
	else
		fd = _creat(__get_redirect(path), mode);

	return fd; 
}

#ifndef _LP64
int creat64(const char *path, mode_t mode)
{
	static int (*_creat64)(const char *, mode_t) = NULL;
	static int (*_open64)(const char *, int) = NULL;
	int fd;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_creat64 == NULL)
		_creat64 = (int (*)(const char *, mode_t))dlsym(RTLD_NEXT, "creat64");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (block_file(path))
	{
		/*
		 * Prefer to keep our own handle to the real open64 function
		 */
		if (_open64 == NULL)
			_open64 = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open64");

		fd = _open64("/dev/null", O_WRONLY);
	}
	else
		fd = _creat64(__get_redirect(path), mode);

	return fd;
}
#endif

int open(const char *path, int flag, ...)
{
	static int (*_open)(const char *, int, ...) = NULL;
	int fd;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_open == NULL)
		_open = (int (*)(const char *, int, ...))dlsym(RTLD_NEXT, "open");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (flag & O_CREAT)
	{
		if (block_file(path))
			fd = _open("/dev/null", flag ^ O_CREAT);
		else
		{
			mode_t mode;
			va_list args;

			/*
			 * The posix specification says that the mode
			 * argument _must_ be given with O_CREAT
			 */
			va_start(args, flag);
			mode = va_arg(args, mode_t);
			va_end(args);

			fd = _open(__get_redirect(path), flag, mode);
		}
	}
	else
		fd = _open(__get_redirect(path), flag);

	return fd;
}

#ifndef _LP64
int open64(const char *path, int flag, ...)
{
	static int (*_open64)(const char *, int, ...) = NULL;
	int fd;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_open64 == NULL)
		_open64 = (int (*)(const char *, int, ...))dlsym(RTLD_NEXT, "open64");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (flag & O_CREAT)
	{
		if (block_file(path))
			fd = _open64("/dev/null", flag ^ O_CREAT);
		else
		{
			mode_t mode;	
			va_list args;

			/*
			 * The posix specification says that the mode
			 * argument _must_ be given with O_CREAT
			 */
			va_start(args, flag);
			mode = va_arg(args, mode_t);
			va_end(args);

			fd = _open64(__get_redirect(path), flag, mode);
		}
	}
	else
		fd = _open64(__get_redirect(path), flag);

	return fd;
}
#endif

FILE *fopen(const char *path, const char *mode)
{
	static FILE *(*_fopen)(const char *, const char *) = NULL;
	FILE *fp;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_fopen == NULL)
		_fopen = (FILE *(*)(const char *, const char *))dlsym(RTLD_NEXT, "fopen");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && block_file(path))
		fp = _fopen("/dev/null", mode);
	else
		fp = _fopen(__get_redirect(path), mode);

	return fp;
}

#ifndef _LP64
FILE *fopen64(const char *path, const char *mode)
{
        static FILE *(*_fopen64)(const char *, const char *) = NULL;
	FILE *fp;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
        if (_fopen64 == NULL)
                _fopen64 = (FILE *(*)(const char *, const char *))dlsym(RTLD_NEXT, "fopen64");

	/*
	 * If asked to block file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && block_file(path))
		fp = _fopen64("/dev/null", mode);
	else
		fp = _fopen64(__get_redirect(path), mode);

	return fp;
}
#endif

int rename(const char *oldpath, const char *newpath)
{
	static int (*_rename)(const char *, const char *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_rename == NULL)
		_rename = (int (*)(const char *, const char *))dlsym(RTLD_NEXT, "rename");

	/*
	 * If asked to block file, unlink the original file instead
	 */
	if (block_file(newpath))
		(void) unlink(oldpath);
	else
		return _rename(oldpath, newpath);

	return 0;
}
