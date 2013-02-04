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
#include <limits.h>
#include <sys/stat.h>

/*
 * There is probably a better way of determining this
 */
int valid_envvar(const char * envvar)
{
	const char *c;
	int valid_chars = 0;

	for (c = envvar; *c != '\0'; c++)
		switch(*c)
		{
			/*
			 * Stop all processing at equals
			 */
			case '=':
				return (valid_chars > 0);
			/*
			 * Numbers are allowed but not at the start
			 */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (valid_chars == 0)
					return 0;
			/*
			 * Lower case is also acceptable :(
			 */
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			/*
			 * And upper case :)
			 */
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			/*
			 * And unless I've been sorely missinformed, the underscore too.
			 */
			case '_':
				valid_chars++;
				break;
			/*
			 * Bail on anything else
			 */
			default:
				return 0;
		}

	/*
	 * It was a zero length string
	 */
	return 0;
}

char *find_path(const char *name)
{
	char path[PATH_MAX], dirs[PATH_MAX], *c;
	struct stat stat_buf;

	switch(*name)
	{
		/*
		 * It's a relative path.
		 */
		case '.':
			(void) realpath(name, path);
			break;

		/*
		 * It's an absolute path.
		 */
		case '/':
			(void) strcpy(path, name);
			break;

		/*
		 * It's neither so search PATH.
		 */
		default:
			/*
			 * No PATH variable set.
			 */
			if ((c = getenv("PATH")) == NULL)
			{
				(void) strcpy(path, name);
				break;
			}

			(void) strcpy(dirs, c);

			/*
			 * PATH is malformed?
			 */
			if ((c = strtok(dirs, ":")) == '\0')
			{
				(void) strcpy(path, name);
				break;
			}
			
			(void) sprintf(path, "%s/%s", c, name);
		
			/*
			 * It's the first one.  Done.
			 */
			if (stat(path, &stat_buf) != -1)
				break;

			/*
			 * Trawl through the rest.
			 */
			while ((c = strtok(NULL, ":")) != '\0')
			{
				(void) sprintf(path, "%s/%s", c, name);

				/*
				 * Found it.
				 */
				if (stat(path, &stat_buf) != -1)
					break;
			}

			/*
			 * Could not find path.
			 */
			if (c == NULL)
				(void) strcpy(path, name);
			break;
	}

	return strdup(path);
}

int main(int argc, const char **argv)
{
	int i, envc = 0, argl = 0;
	char **envp = NULL, **tmp_envp, **argp = NULL, **tmp_argp;

	/*
	 * Unlike env, no arguments for us means something is wrong.
	 */
	if (argc == 1)
	{
		fprintf(stderr, "Error: No arguments given\n");
		return EXIT_FAILURE;
	}

	/*
	 * If LD_PRELOAD variables are not set, load them and rexec
	 */
	if (getenv("LD_PRELOAD_32") == NULL || getenv("LD_PRELOAD_64") == NULL)
	{
#ifdef DEBUG
		fprintf(stderr, "DEBUG: No LD_PRELOAD set.  About to re-exec\n");
#endif

		/*
		 * FIXME: Hardcoding.  Should check if files exist, yada.
		 */
		putenv("LD_PRELOAD_32=/root/stormos-base/tools/libbuildenv-32.so");
		putenv("LD_PRELOAD_64=/root/stormos-base/tools/libbuildenv-64.so");

		/*
		 * Exec wants a NULL terminated list of arguments
		 */
		if ((argp = malloc(sizeof(char *) * (argc + 1))) == NULL)
		{
			perror("Error: Unable to allocate arguments");
			return EXIT_FAILURE;
		} 

		for (i = 0; i < argc; i++)
			argp[i] = strdup(argv[i]);
		argp[++i] = NULL;

		/*
		 * Rexec ourself so that libbuildenv redirect works on us too
		 */
		if (execv(argv[0], argp) == -1)
			perror("Error: unable to re-exec self");

		return EXIT_FAILURE;
	}

#ifdef DEBUG
	fprintf(stderr, "DEBUG: Running under libbuildenv's influence\n");
#endif

	/*
	 * Parse any environment variables given on the command line
	 */
	for (i = 1; i < argc; i++)
	{
		/*
		 * Stop and the first non environment variable
		 */
		if (!valid_envvar(argv[i]))
			break;

		/*
		 * Expand envp to hold the new environment variable
		 */
		else if ((tmp_envp = realloc(envp, sizeof(char *) * (envc + 2))) == NULL)
		{
			perror("Error: Unable to allocate environment variables");
			return EXIT_FAILURE;
		}

		/*
		 * Copy argument to new envp
		 */
		else if ((tmp_envp[envc] = strdup(argv[i])) == NULL)
		{
			perror("Error: Unable to copy input argument");
			return EXIT_FAILURE;
		}
		/*
		 * Finalize
		 */
		else
		{
			tmp_envp[++envc] = NULL;
			envp = tmp_envp;
		}
	}

#ifdef DEBUG
	fprintf(stderr, "DEBUG: Finished parsing environment variables\n");
#endif

	/*
	 * Whatevers left should be the executable and it's arguments
	 */
	for (; i < argc; i++)
	{
		/*
		 * Expand argp to hold the new argument list
		 */
		if ((tmp_argp = realloc(argp, sizeof(char *) * (argl + 2))) == NULL)
		{
			perror("Error: Unable to allocate arguments");
			return EXIT_FAILURE;
		}

		/*
		 * Copy argument to argp
		 */
		else if ((tmp_argp[argl] = strdup(argv[i])) == NULL)
		{
			perror("Error: Unable to copy input argument");
			return EXIT_FAILURE;
		}
		/*
		 * Finalize
		 */
		else
		{
			tmp_argp[++argl] = NULL;
			argp = tmp_argp;
		}
	}

#ifdef DEBUG
	fprintf(stderr, "DEBUG: Finsished parsing command line options\n");
#endif

	/*
	 * Add LD_PRELOAD variables needed by libbuildenv
	 */
	if ((tmp_envp = realloc(envp, sizeof(char *) * (envc + 3))) == NULL)
	{
		perror("Error: Unable to allocate environment variables");
		return EXIT_FAILURE;
	}

	(void) asprintf(&tmp_envp[envc], "LD_PRELOAD_32=%s", getenv("LD_PRELOAD_32"));
	(void) asprintf(&tmp_envp[++envc], "LD_PRELOAD_64=%s", getenv("LD_PRELOAD_64"));
	tmp_envp[++envc] = NULL;

	envp = tmp_envp;

#ifdef DEBUG
	fprintf(stderr, "Dumping ENV\n");
	for (i = 0; envp[i]; i++)
		fprintf(stderr, "\t%s\n", envp[i]);

	fprintf(stderr, "Dumping ARGV\n");
	for (i = 0; argp[i]; i++)
		fprintf(stderr, "\t%s\n", argp[i]);
#endif

	/*
	 * Done.  Run command in build environment.  
	 */
	if (execve((const char *)find_path(argp[0]), (const char **)argp, (const char **)envp) == -1)
		perror("Error: Unable to exec");

	return EXIT_FAILURE;
}
