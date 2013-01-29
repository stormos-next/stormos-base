#!/bin/bash
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

# Disable direct binding of symbols
#export LD_NODIRECT=yes

# Load our installfilter libraries
export LD_PRELOAD_32=$DIRNAME/installfilter-32.so
export LD_PRELOAD_64=$DIRNAME/installfilter-64.so

# Run whatever command we're given
exec $@