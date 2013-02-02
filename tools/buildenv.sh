#!/bin/ksh
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
#
# You can obtain a copy of the license at COPYING
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at COPYING.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
# Copyright (c) 2013 Andrew Stormont.
#

# Find our where our libraries are
DIRNAME=`dirname $0`
case "$DIRNAME" in
	.)
		DIRNAME="$PWD"
	;;
	../*)
		DIRNAME="$PWD/$DIRNAME"
	;;
esac

if [ -z "$LD_PRELOAD_32" -o -z "$LD_PRELOAD_64" ]; then
	# Load our libraries and re-exec
	LIBBUILDENV_32=$DIRNAME/libbuildenv-32.so
	if [ ! -f $LIBBUILDENV_32 ]; then
		echo "Error: libbuildenv-32.so not found!" >&1
		exit 1
	fi
	LIBBUILDENV_64=$DIRNAME/libbuildenv-64.so
	if [ ! -f $LIBBUILDENV_64 ]; then
		echo "Error: libbuildenv-64.so not found!" >&1
		exit 1
	fi
	export LD_DIRECT=no
	export LD_PRELOAD_32=$LIBBUILDENV_32
	export LD_PRELOAD_64=$LIBBUILDENV_64
	exec ksh $0 $@
else
	# Run whatever command we're given
	exec env - PATH=$PATH LD_PRELOAD_32=$LD_PRELOAD_32 \
		LD_PRELOAD_64=$LD_PRELOAD_64 $@
fi
