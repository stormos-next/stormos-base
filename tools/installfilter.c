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
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <dlfcn.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

/*
 * We hardcode the proto dir for now.  Yes I know, it's horrible.
 */
#define PROTO_DIR_PATH		"/root/stormos-base/proto"
#define PROTO_DIR_LEN		24

#define LIBTOOL_DIR_PATH	"/usr/share/libtool"
#define LIBTOOL_DIR_LEN		18
#define ACLOCAL_DIR_PATH	"/usr/share/aclocal"
#define ACLOCAL_DIR_LEN		18
#define AUTOCONF_DIR_PATH	"/usr/share/autoconf"
#define AUTOCONF_DIR_LEN	19
#define AUTOMAKE_DIR_PATH	"/usr/share/automake"
#define AUTOMAKE_DIR_LEN	19

/*
 * FIXME: This will leak memory
 */
static const char *__get_redirect(const char *oldpath)
{
	char *newpath;

	/*
	 * Redirect libtool and aclocal paths to proto dir
	 */	
	if (strncmp(oldpath, LIBTOOL_DIR_PATH, LIBTOOL_DIR_LEN) == 0 ||
	    strncmp(oldpath, ACLOCAL_DIR_PATH, ACLOCAL_DIR_LEN) == 0 ||
	    strncmp(oldpath, AUTOCONF_DIR_PATH, AUTOCONF_DIR_LEN) == 0 ||
	    strncmp(oldpath, AUTOMAKE_DIR_PATH, AUTOMAKE_DIR_LEN) == 0)
		asprintf(&newpath, PROTO_DIR_PATH "%s", oldpath);

	/*
	 * No redirect
	 */
	else
		newpath = strdup(oldpath);

	return newpath;
}

static int __is_crap_file(const char *path)
{
	int len = strlen(path);

	/*
	 * It's a fucking libtool archive.  Seriously guys.
	 */
	if (len > 2 && path[len-3] == '.' && path[len-2] == 'l' && path[len-1] == 'a')
		return 1;

	/*
	 * No idea what the file is.  Assumes all's good.
	 */
	return 0;
}

static int __inside_protodir(const char *path)
{
	/*
	 * Check if given path resides in proto dir
	 */
	return (strncmp(path, PROTO_DIR_PATH, PROTO_DIR_LEN) == 0);
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
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (__is_crap_file(path) && __inside_protodir(path))
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
	 * If asked to creat a crap file, return handle to /dev/null
	 */
	if (__is_crap_file(path) && __inside_protodir(path))
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
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (flag & O_CREAT)
	{
		if (__is_crap_file(path) && __inside_protodir(path))
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
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (flag & O_CREAT)
	{
		if (__is_crap_file(path) && __inside_protodir(path))
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
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && __is_crap_file(path) && __inside_protodir(path))
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
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && __is_crap_file(path) && __inside_protodir(path))
		fp = _fopen64("/dev/null", mode);
	else
		fp = _fopen64(__get_redirect(path), mode);

	return fp;
}
#endif

DIR *opendir(const char *path)
{
	static DIR *(*_opendir)(const char *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_opendir == NULL)
		_opendir = (DIR *(*)(const char *))dlsym(RTLD_NEXT, "opendir");

	/*
	 * Redirect some requests to the proto dir
	 */
	return _opendir(__get_redirect(path));	
}

#define LIBTOOL_BIN_PATH	"/usr/bin/libtool"
#define LIBTOOL_BIN_LEN		16
#define AUTOM4TE_BIN_PATH	"/usr/bin/autom4te"
#define AUTOM4TE_BIN_LEN	17
#define ACLOCAL_BIN_PATH	"/usr/bin/aclocal"
#define ACLOCAL_BIN_LEN		16
#define AUTOCONF_BIN_PATH	"/usr/bin/autoconf"
#define AUTOCONF_BIN_LEN	17
#define AUTOMAKE_BIN_PATH	"/usr/bin/automake"
#define AUTOMAKE_BIN_LEN	17

int execve(const char *old_path, const char **old_argv, const char **envp)
{
	static int (*_execve)(const char *, const char **, const char **) = NULL;
	char *new_path; char **new_argv;
	int i;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_execve == NULL)
		_execve = (int (*)(const char *, const char **, const char **))dlsym(RTLD_NEXT, "execve");

	/*
	 * Redirect some requests to the proto dir
	 *
	 * Note: this also truncates the version numbers and forces
	 *       everything to run in the default version.
	 */
	if (strncmp(old_path, LIBTOOL_BIN_PATH, LIBTOOL_BIN_LEN) == 0 ||
	    strncmp(old_path, LIBTOOL_BIN_PATH + 8, LIBTOOL_BIN_LEN - 8) == 0)
		new_path = strdup(PROTO_DIR_PATH LIBTOOL_BIN_PATH);
	else if (strncmp(old_path, AUTOM4TE_BIN_PATH, AUTOM4TE_BIN_LEN) == 0 ||
	    strncmp(old_path, AUTOM4TE_BIN_PATH + 8, AUTOM4TE_BIN_LEN - 8) == 0)
		new_path = strdup(PROTO_DIR_PATH AUTOM4TE_BIN_PATH);
	else if (strncmp(old_path, ACLOCAL_BIN_PATH, ACLOCAL_BIN_LEN) == 0 ||
	    strncmp(old_path, ACLOCAL_BIN_PATH + 8, ACLOCAL_BIN_LEN - 8) == 0)
		new_path = strdup(PROTO_DIR_PATH ACLOCAL_BIN_PATH);
	else if (strncmp(old_path, AUTOCONF_BIN_PATH, AUTOCONF_BIN_LEN) == 0 ||
	    strncmp(old_path, AUTOCONF_BIN_PATH + 8, AUTOCONF_BIN_LEN - 8) == 0)
		new_path = strdup(PROTO_DIR_PATH AUTOCONF_BIN_PATH);
	else if (strncmp(old_path, AUTOMAKE_BIN_PATH, AUTOMAKE_BIN_LEN) == 0 ||
	    strncmp(old_path, AUTOMAKE_BIN_PATH + 8, AUTOMAKE_BIN_LEN - 8) == 0)
		new_path = strdup(PROTO_DIR_PATH AUTOMAKE_BIN_PATH);
	else
		new_path = strdup(old_path);

	/*
	 * Also need to fix up ARGV0 for interpreted scripts
	 */
	for (i = 0; old_argv[i] != NULL; i++)

	if (new_path == NULL || (new_argv = malloc(sizeof(char *) * (i + 1))) == NULL)	
	{
		errno = ENOMEM;
		return -1;
	}

	for (i = 0; old_argv[i] != NULL; i++)
		if (i == 0)
			new_argv[i] = strdup(new_path);
		else
			new_argv[i] = strdup(old_argv[i]);
	new_argv[i++] = NULL;

	return _execve((const char *)new_path, (const char **)new_argv, envp);
}

int rename(const char *oldpath, const char *newpath)
{
	static int (*_rename)(const char *, const char *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_rename == NULL)
		_rename = (int (*)(const char *, const char *))dlsym(RTLD_NEXT, "rename");

	/*
	 * If asked to rename a file to a crapfile, unlink the original file instead
	 */
	if (__is_crap_file(newpath) && __inside_protodir(newpath))
		(void)unlink(oldpath);
	else
		return _rename(oldpath, newpath);

	return 0;
}

int stat(const char *path, struct stat *statptr)
{
	static int (*_stat)(const char *, struct stat *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_stat == NULL)
		_stat = (int (*)(const char *, struct stat *))dlsym(RTLD_NEXT, "stat");

	/*
	 * Redirect some stat requests to the proto directory
	 */
	return _stat(__get_redirect(path), statptr);
}

#ifndef _LP64
int stat64(const char *path, struct stat64 *statptr)
{
	static int (*_stat64)(const char *, struct stat64 *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_stat64 == NULL)
		_stat64 = (int (*)(const char *, struct stat64 *))dlsym(RTLD_NEXT, "stat64");

	/*
	 * Redirect some stat64 requests to the proto directory
	 */
	return _stat64(__get_redirect(path), statptr);
}
#endif

int lstat(const char *path, struct stat *statptr)
{
	static int (*_lstat)(const char *, struct stat *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_lstat == NULL)
		_lstat = (int (*)(const char *, struct stat *))dlsym(RTLD_NEXT, "lstat");

	/*
	 * Redirect some lstat requests to the proto directory
	 */
	return _lstat(__get_redirect(path), statptr);
}

#ifndef _LP64
int lstat64(const char *path, struct stat64 *statptr)
{
	static int (*_lstat64)(const char *, struct stat64 *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_lstat64 == NULL)
		_lstat64 = (int (*)(const char *, struct stat64 *))dlsym(RTLD_NEXT, "lstat64");

	/*
	 * Redirect some lstat64 requests to the proto directory
	 */
	return _lstat64(__get_redirect(path), statptr);
}
#endif

int chdir(const char *path)
{
	static int (*_chdir)(const char *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_chdir == NULL)
		_chdir = (int (*)(const char *))dlsym(RTLD_NEXT, "chdir");

	/*
	 * Redirect some chdir requests to the proto directory
	 */
	return _chdir(__get_redirect(path));
}

int main(int argc, const char argv)
{
	fprintf(stderr, "What do you think you're doing?\n");

	return 1;
}
