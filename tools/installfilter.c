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
#include <errno.h>
#include <dlfcn.h>
#include <string.h>

/*
 * We can't include fcntl.h because the function definitions will clash,
 * so instead define any important mode flags here:
 */ 
#define O_CREAT 0x100
#define O_TRUNC 0x200

#define O_WRONLY 1

static int __is_crap_file(const char *path)
{
	int len = strlen(path);

	/*
	 * It's a fucking libtool archive.  Seriously guys.
	 */
	if (len > 2 && path[len-3] == '.' && path[len-2] == 'l' && path[len-1] == 'a')
		return 1;

#if 0 // Disable this for now as it can break installs
	/*
	 * It's a static library.  Almost as useless to us.
	 */
	if (len > 1 && path[len-2] == '.' && path[len-1] == 'a')
		return 1;
#endif

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

	//fprintf(stderr, "Intercepting creat(%s, %d) call\n", path, mode);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_creat == NULL)
		_creat = (int (*)(const char *, mode_t))dlsym(RTLD_NEXT, "creat");

	/*
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (__is_crap_file(path))
	{
		/*
		 * Prefer to keep our own handle to the real open function
		 */
		if (_open == NULL)
			_open = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open");

		fd = _open("/dev/null", O_WRONLY);
	}
	else
		fd = _creat(path, mode);

	return fd; 
}

#ifndef __amd64__
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
	if (__is_crap_file(path))
	{
		/*
		 * Prefer to keep our own handle to the real open64 function
		 */
		if (_open64 == NULL)
			_open64 = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open64");

		fd = _open64("/dev/null", O_WRONLY);
	}
	else
		fd = _creat64(path, mode);

	return fd;
}
#endif

int open(const char *path, int mode)
{
	static int (*_open)(const char *, int) = NULL;
	int fd;

	//fprintf(stderr, "Intercepting open(%s, %d) call\n", path, mode);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_open == NULL)
		_open = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open");

	/*
	 * If asked to create a crap file, return handle to /dev/null
	 */	
	if ((mode & O_CREAT || mode & O_TRUNC) && __is_crap_file(path))
		fd = _open("/dev/null", mode);
	else
		fd = _open(path, mode);

	return fd;
}

#ifndef __amd64__
int open64(const char *path, int mode)
{
	static int (*_open64)(const char *, int) = NULL;
	int fd;

	//fprintf(stderr, "Intercepting open64(%s, %d) call\n", path, mode);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_open64 == NULL)
		_open64 = (int (*)(const char *, int))dlsym(RTLD_NEXT, "open64");

	/*
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if ((mode & O_CREAT || mode & O_TRUNC) && __is_crap_file(path))
		fd = _open64("/dev/null", mode);
	else
		fd = _open64(path, mode);

	return fd;
}
#endif

FILE *fopen(const char *path, const char *mode)
{
	static FILE *(*_fopen)(const char *, const char *) = NULL;
	FILE *fp;

	//fprintf(stderr, "Intercepting fopen(%s, %s) call\n", path, mode);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_fopen == NULL)
		_fopen = (FILE *(*)(const char *, const char *))dlsym(RTLD_NEXT, "fopen");

	/*
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && __is_crap_file(path))
		fp = _fopen("/dev/null", mode);
	else
		fp = _fopen(path, mode);

	return fp;
}

#ifndef __amd64__
FILE *fopen64(const char *path, const char *mode)
{
        static FILE *(*_fopen64)(const char *, const char *) = NULL;
	FILE *fp;

	//fprintf(stderr, "Intercepting fopen64(%s, %s) call\n", path, mode);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
        if (_fopen64 == NULL)
                _fopen64 = (FILE *(*)(const char *, const char *))dlsym(RTLD_NEXT, "fopen64");

	/*
	 * If asked to create a crap file, return handle to /dev/null
	 */
	if (mode[0] == 'w' && __is_crap_file(path))
		fp = _fopen64("/dev/null", mode);
	else
		fp = _fopen64(path, mode);

	return fp;
}
#endif

int execve(const char *path, const char **argv, const char **envp)
{
	int (*_execve)(const char *, const char **, const char **) = NULL;
	int i, envc;
	char **new_envp = NULL, **tmp_envp, *preload_32, *preload_64;

	//fprintf(stderr, "Intercepting execve() call\n");

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_execve == NULL)
		_execve = (int (*)(const char *, const char **, const char **))dlsym(RTLD_NEXT, "execve");

	/*
	 * Copy LD_PRELOAD_32 and LD_PRELOAD_64 into envp.
	 */
	if ((preload_32 = getenv("LD_PRELOAD_32")) != NULL || (preload_64 = getenv("LD_PRELOAD_64")) != NULL)
	{
		/*
		 * Get number of existing environment variables
		 */
		for (envc = 0; envp[envc] != NULL; envc++);
		
		if (preload_32 != NULL)
		{
			/*
			 * Bail if there's not enough memory to fudge envp
			 */
			if ((new_envp = (char **)malloc(sizeof(char *) * (envc + 2))) == NULL)
			{
				errno = ENOMEM;
				return -1;
			}

			/*
			 * Copy existing environment variables over
			 */
			for (i = 0; i < envc; i++)
				new_envp[i] = strdup(envp[i]);

			/*
			 * Append LD_PRELOAD_32
			 */ 
			new_envp[envc++] = preload_32;
			new_envp[envc++] = NULL;
		}
		
		if (preload_64 != NULL)
		{
			if (new_envp == NULL)
			{
				/*
				 * Bail if there's not enough memory to fudge envp
				 */
				if ((new_envp = (char **)malloc(sizeof(char *) * (envc + 2))) == NULL)
				{
					errno = ENOMEM;
					return -1;
				}

				/*
				 * Copy existing environment variables over
				 */
				for (i = 0; i < envc; i++)
					new_envp[i] = strdup(envp[i]);
			}
			else
			{
				/* 
				 * Bail if there's not enough memory to fudge envp
				 */
				if ((tmp_envp = (char **)realloc((void *)new_envp, sizeof(char *) * (envc + 2))) == NULL)
				{
					/*
					 * Avoid leaking any mem
					 */
					for (i = 0; i < envc; i++)
						free(new_envp[i]);

					errno = ENOMEM;
					return -1;
				}
				else
					new_envp = tmp_envp;
			}	 
			
			/*
			 * Append LD_PRELOAD_64
			 */
			new_envp[envc++] = preload_64; 
			new_envp[envc++] = NULL;
		}
		
		/*
		 * Call execvp with our injected environment variables
		 */	
		return _execve(path, argv, (const char **)new_envp);
	}

	/*
	 * Couldn't find LD_PRELOAD_* variables to copy over
	 */
	return _execve(path, argv, envp);
}

int rename(const char *oldpath, const char *newpath)
{
	int (*_rename)(const char *, const char *) = NULL;

	//fprintf(stderr, "Intercepting rename(%s, %s) call\n", oldpath, newpath);

	/*
	 * Load the next symbol which is hopefully in libc.
	 */
	if (_rename == NULL)
		_rename = (int (*)(const char *, const char *))dlsym(RTLD_NEXT, "rename");

	/*
	 * If asked to rename a file to a crapfile, unlink the original file instead
	 */
	if (__is_crap_file(newpath))
		(void)unlink(oldpath);
	else
		return _rename(oldpath, newpath);

	return 0;
}

int main(int argc, const char argv)
{
	fprintf(stderr, "What do you think you're doing?\n");

	return 1;
}
