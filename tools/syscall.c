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
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>

#include "config.h"

#define LIBTOOL_DIR_PATH	"/usr/share/libtool"
#define LIBTOOL_DIR_LEN		18
#define ACLOCAL_DIR_PATH	"/usr/share/aclocal"
#define ACLOCAL_DIR_LEN		18
#define AUTOCONF_DIR_PATH	"/usr/share/autoconf"
#define AUTOCONF_DIR_LEN	19
#define AUTOMAKE_DIR_PATH	"/usr/share/automake"
#define AUTOMAKE_DIR_LEN	19

#define LIBTOOL_BIN_PATH        "/usr/bin/libtool"
#define LIBTOOL_BIN_LEN         16
#define AUTOM4TE_BIN_PATH       "/usr/bin/autom4te"
#define AUTOM4TE_BIN_LEN        17
#define ACLOCAL_BIN_PATH        "/usr/bin/aclocal"
#define ACLOCAL_BIN_LEN         16
#define AUTOCONF_BIN_PATH       "/usr/bin/autoconf"
#define AUTOCONF_BIN_LEN        17
#define AUTOMAKE_BIN_PATH       "/usr/bin/automake"
#define AUTOMAKE_BIN_LEN        17
#define AUTORECONF_BIN_PATH	"/usr/bin/autoreconf"
#define AUTORECONF_BIN_LEN	19	

/*
 * Look in string.c for this
 */
extern char *strreplace(const char*, const char*, const char*);

/*
 * FIXME: This will leak memory
 */
char *__get_redirect(const char *old_path)
{
	char *new_path;

	/*
	 * FIXME: Where the eff is this coming from?
	 */
	if (strncmp(old_path, AUTOCONF_DIR_PATH "/m4/", AUTOCONF_DIR_LEN + 4) == 0)
		asprintf(&new_path, PROTO_DIR_PATH ACLOCAL_DIR_PATH "%s", old_path + AUTOCONF_DIR_LEN + 3);	

	/*
	 * Redirect libtool and fiends directories to proto dir
	 */	
	else if (strncmp(old_path, LIBTOOL_DIR_PATH, LIBTOOL_DIR_LEN) == 0 ||
	    strncmp(old_path, ACLOCAL_DIR_PATH, ACLOCAL_DIR_LEN) == 0 ||
	    strncmp(old_path, AUTOCONF_DIR_PATH, AUTOCONF_DIR_LEN) == 0 ||
	    strncmp(old_path, AUTOMAKE_DIR_PATH, AUTOMAKE_DIR_LEN) == 0)
		asprintf(&new_path, PROTO_DIR_PATH "%s", old_path);

	/* 
	 * Redirect libtool and fiends executables to proto dir
	 */
        else if (strncmp(old_path, LIBTOOL_BIN_PATH, LIBTOOL_BIN_LEN) == 0 ||
            strncmp(old_path, LIBTOOL_BIN_PATH + 9, LIBTOOL_BIN_LEN - 9) == 0)
                new_path = strdup(PROTO_DIR_PATH LIBTOOL_BIN_PATH);
        else if (strncmp(old_path, AUTOM4TE_BIN_PATH, AUTOM4TE_BIN_LEN) == 0 ||
            strncmp(old_path, AUTOM4TE_BIN_PATH + 9, AUTOM4TE_BIN_LEN - 9) == 0)
                new_path = strdup(PROTO_DIR_PATH AUTOM4TE_BIN_PATH);
        else if (strncmp(old_path, ACLOCAL_BIN_PATH, ACLOCAL_BIN_LEN) == 0 ||
            strncmp(old_path, ACLOCAL_BIN_PATH + 9, ACLOCAL_BIN_LEN - 9) == 0)
                new_path = strdup(PROTO_DIR_PATH ACLOCAL_BIN_PATH);
        else if (strncmp(old_path, AUTOCONF_BIN_PATH, AUTOCONF_BIN_LEN) == 0 ||
            strncmp(old_path, AUTOCONF_BIN_PATH + 9, AUTOCONF_BIN_LEN - 9) == 0)
                new_path = strdup(PROTO_DIR_PATH AUTOCONF_BIN_PATH);
        else if (strncmp(old_path, AUTOMAKE_BIN_PATH, AUTOMAKE_BIN_LEN) == 0 ||
            strncmp(old_path, AUTOMAKE_BIN_PATH + 9, AUTOMAKE_BIN_LEN - 9) == 0)
                new_path = strdup(PROTO_DIR_PATH AUTOMAKE_BIN_PATH);
	else if (strncmp(old_path, AUTORECONF_BIN_PATH, AUTORECONF_BIN_LEN) == 0 ||
	    strncmp(old_path, AUTORECONF_BIN_PATH + 9, AUTORECONF_BIN_LEN - 9) == 0)
		new_path = strdup(PROTO_DIR_PATH AUTORECONF_BIN_PATH);

	/*
	 * No redirect
	 */
	else
		new_path = strdup(old_path);

#ifdef DEBUG_REDIRECT
	if (strcmp(old_path, new_path) != 0)
		fprintf(stderr, "Redirecting \"%s\" => \"%s\"\n", old_path, new_path);
#endif

	return new_path;
}

int execve(const char *old_path, const char **old_argv, const char **envp)
{
	static int (*_execve)(const char *, const char **, const char **) = NULL;
	const char *new_path; char **new_argv;
	int i;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_execve == NULL)
		_execve = (int (*)(const char *, const char **, const char **))dlsym(RTLD_NEXT, "execve");

	/*
	 * Redirect some exec paths to proto dir
	 */
	new_path = __get_redirect(old_path);

	/*
	 * Redirect any matching paths passed in argv 
	 */
	for (i = 0; old_argv[i] != NULL; i++)

	if (new_path == NULL || (new_argv = malloc(sizeof(char *) * (i + 1))) == NULL)	
	{
		errno = ENOMEM;
		return -1;
	}

	for (i = 0; old_argv[i] != NULL; i++)
		/*
		 * FIXME: This is a hack for GNU M4
		 */
		if (strstr(old_argv[i], "=" AUTOCONF_DIR_PATH) != NULL)
			new_argv[i] = strreplace("=" AUTOCONF_DIR_PATH, "=" PROTO_DIR_PATH AUTOCONF_DIR_PATH, old_argv[i]);
		else
			new_argv[i] = __get_redirect(old_argv[i]);
	new_argv[i] = NULL;

#ifdef DEBUG_EXECVE
	fprintf(stderr, "Dumping conents of old_argv:\n");
	for (i = 0; old_argv[i] != NULL; i++)
		if (*old_argv[i] == '\0')
			fprintf(stderr, "\t*** EMPTY ***\n");
		else
			fprintf(stderr, "\t%s\n", old_argv[i]);

	fprintf(stderr, "Dumping contents of new_argv:\n");
	for (i = 0; new_argv[i] != NULL; i++)
		if (*new_argv[i] == '\0')
			fprintf(stderr, "\t*** EMPTY ***\n");
		else
			fprintf(stderr, "\t%s\n", new_argv[i]);
#endif

	return _execve(new_path, (const char **)new_argv, envp);
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

int symlink(const char *target, const char *path)
{
	static int (*_symlink)(const char *, const char *) = NULL;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_symlink == NULL)
		_symlink = (int (*)(const char *, const char *))dlsym(RTLD_NEXT, "symlink");	

	/*
	 * Redirect some link targets to protodir
	 */
	return _symlink(__get_redirect(target), path);
}

char *getenv(const char *name)
{
	static char *(*_getenv)(const char *) = NULL;
	char *value;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_getenv == NULL)
		_getenv = (char *(*)(const char *))dlsym(RTLD_NEXT, "getenv");

	/*
	 * Get the real value of the variable
	 */
	if ((value = _getenv(name)) == NULL)
		return NULL;

	/*
	 * Redirect some getenv results to the protodir
	 */
	return __get_redirect(value);
}

int clearenv(void)
{
	extern char **environ;
	char **new_environ;

	/*
	 * Bail if there's not enough memory to fudge environ
	 */
	if ((new_environ = (char **)malloc(sizeof(char *) * 3)) == NULL)
		return -1;

	/*
	 * Inject LD_PRELOAD variables
	 */
	asprintf(&new_environ[0], "LD_PRELOAD_32=%s", getenv("LD_PRELOAD_32"));
	asprintf(&new_environ[1], "LD_PRELOAD_64=%s", getenv("LD_PRELOAD_64"));

	/*
	 * Apply changes.
	 */
	new_environ[2] = NULL;
	environ = new_environ;

	return 0;
}

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
