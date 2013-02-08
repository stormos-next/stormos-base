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

VER =	libxcb-1.9

include ../Makefile.defs
include ../class/x11.mk

PATCHES = Patches/*
PATCHSTRIP = 0

# Help xcbgen find it's crap
XCBPROTO_XCBPYTHONDIR = \
	$(DESTDIR)$(X11_DIR)/lib/python2.6/site-packages/

XCBPROTO_XCBINCLUDEDIR = \
	$(DESTDIR)$(X11_DIR)/share/xcb

OVERRIDES += \
	XCBPROTO_XCBPYTHONDIR="$(XCBPROTO_XCBPYTHONDIR)" \
	XCBPROTO_XCBINCLUDEDIR="$(XCBPROTO_XCBINCLUDEDIR)"

include ../Makefile.targ
include ../Makefile.targ.autoconf
