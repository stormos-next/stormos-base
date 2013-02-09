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
# Copyright (c) 2013 Andrew Stormont. All rights reserved.
#

VER =	unzip60

include ../Makefile.defs

PATCHES = Patches/*

# Configure scripts lives in subdir
AUTOCONF_DIR = unix

# Configure only take a single argument: the compiler
AUTOCONF_OPTS = $(GCC)

# Must call Makefile from top directory
all_autoconf: OVERRIDES += -f unix/Makefile generic_gcc
install_autoconf: OVERRIDES += -f unix/Makefile

include ../Makefile.targ
include ../Makefile.targ.autoconf
