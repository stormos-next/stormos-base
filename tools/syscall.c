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
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>

#include "config.h"

/*
 * Look in string.c for this
 */
extern char *strreplace(const char*, const char*, const char*);

/*
 * Look in redirect.c for this
 */
extern char *get_redirect(const char *);

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
	new_path = get_redirect(old_path);

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
#define AUTOCONF_DIR_PATH	"/usr/share/autoconf"
		if (strstr(old_argv[i], "=" AUTOCONF_DIR_PATH) != NULL)
			new_argv[i] = strreplace("=" AUTOCONF_DIR_PATH, "=" PROTO_DIR_PATH AUTOCONF_DIR_PATH, old_argv[i]);
		else
			new_argv[i] = get_redirect(old_argv[i]);
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
	return _stat(get_redirect(path), statptr);
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
	return _stat64(get_redirect(path), statptr);
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
	return _lstat(get_redirect(path), statptr);
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
	return _lstat64(get_redirect(path), statptr);
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
	return _symlink(get_redirect(target), path);
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
	return get_redirect(value);
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

int unsetenv(const char *name)
{
	static int (*_unsetenv)(const char *) = NULL;

	/*
	 * Return success but dont remove LD_PRELOAD variables
	 */
	if (strcmp(name, "LD_PRELOAD_32") == 0 ||
	    strcmp(name, "LD_PRELOAD_64") == 0)
		return 0;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_unsetenv == NULL)
		_unsetenv = (int (*)(const char *))dlsym(RTLD_NEXT, "unsetenv");

	return _unsetenv(name);
}

int setenv(const char *name, const char *value, int flag)
{
	static int (*_setenv)(const char *, const char *, int) = NULL;

	/*
	 * Return success but dont remove LD_PRELOAD variables
	 */
	if ((strcmp(name, "LD_PRELOAD_32") == 0 ||
	    strcmp(name, "LD_PRELOAD_64") == 0) &&
	    (value == NULL || *value == '\0'))
		return 0;

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_setenv == NULL)
		_setenv = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");

	return _setenv(name, value, flag);
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
	return _opendir(get_redirect(path));	
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
	return _chdir(get_redirect(path));
}

int main(int argc, const char argv)
{
	fprintf(stderr, "What do you think you're doing?\n");

	return 1;
}
