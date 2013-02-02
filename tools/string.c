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

/*
 * Return the position of substring 'needle' in 'haystack'.
 */
int strindex(const char *haystack, const char *needle)
{
        int i = 0, offset = 0;
	
	while (1)
	{
		/*
		 * Reached end of needle.  Success.
		 */
		if (needle[i] == '\0')
			break;
		/*
		 * Reached end of haystack before match.
		 */
		else if (haystack[i + offset] == '\0')
			return -1;
		/*
		 * No match so adjust offset
		 */
		else if (haystack[i + offset] != needle[i])
			offset++;
		/*
		 * It's a match.  Keep going.
		 */
		else
			i++;
	}
        
	return offset;
}

/*
 * Return a copy of 'string' with all instances of 'search' replaced with 'replace'
 */
char *strreplace(const char *search, const char *replace, const char *string)
{
	int old_pos, new_pos, offset = 0, search_len, replace_len;
	char *in_string, *out_string;

	/*
	 * Load a copy of the input string
	 */
	if ((in_string = strdup(string)) == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	/*
	 * Zero length search string given.  No thankyou.
	 */
	if ((search_len = strlen(search)) == 0)
		return in_string;

	/*
	 * Zero length replace string given.  No thankyou either.
	 */
	if ((replace_len = strlen(replace)) == 0)
		return in_string;

	while (1)
		/*
		 * Return input string if there is nothing to replace.
		 */
		if ((old_pos = strindex(in_string + offset, search)) == -1)
			break;

		/*
		 * Adjust offset if we find replaced text in input string already.
		 */
		else if ((new_pos = strindex(in_string + offset, replace)) != -1 &&
			new_pos < old_pos + replace_len)
			offset += replace_len;

		/*
		 * Create new string with replacement made.
		 */
		else
		{
			/*
			 * Allocate enough memory to store the new string.
			 */
			if ((out_string = malloc(strlen(in_string) - search_len + replace_len)) == NULL)
			{
				free(in_string);
				errno = ENOMEM;
				return NULL;
			}

			/*
			 * Copy any text leading up the to the replacement.
			 */
			if (old_pos + offset > 0)
				strncpy(out_string, in_string, old_pos + offset);

			/*
			 * Copy in replacement string.
			 */
			strcpy(out_string + old_pos + offset, replace);

			/*
			 * Copy anything left after the replacement.
			 */
			strcpy(out_string + old_pos + offset + replace_len, in_string + old_pos + offset + search_len);

			/*
			 * Slap in a null terminator
			 */
			out_string[strlen(in_string) - search_len + replace_len]  = '\0';

			/*
			 * Replacement done! (this time round).
			 */
			free(in_string);
			in_string = out_string;
		}
			
	return in_string;
}
