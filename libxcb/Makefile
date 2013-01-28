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

PATCHES = Patches/*
PATCHSTRIP = 0

# Put this with the rest of the X stuff
AUTOCONF_OPTS = \
	--prefix=/usr/X11R6

# Use pthread-stubs and libXau from proto area
AUTOCONF_ENV += \
	PKG_CONFIG_PATH="$(DESTDIR)/usr/X11R6/lib/pkgconfig"

# Convince libXau we have xproto installed
XAU_CFLAGS = \
        -I/usr/X11R6/include

XAU_LDFLAGS = \
        -L/usr/X11R6/lib \
        -R/usr/X11R6/lib

AUTOCONF_ENV += \
        NEEDED_CFLAGS="$(XAU_CFLAGS)" \
        NEEDED_LIBS="$(XAU_LDFLAGS)"

# Help xcbgen find it's crap
XCBPROTO_XCBPYTHONDIR = \
	$(DESTDIR)/usr/X11R6/lib/python2.6/site-packages/

XCBPROTO_XCBINCLUDEDIR = \
	$(DESTDIR)/usr/X11R6/share/xcb

OVERRIDES += \
	XCBPROTO_XCBPYTHONDIR="$(XCBPROTO_XCBPYTHONDIR)" \
	XCBPROTO_XCBINCLUDEDIR="$(XCBPROTO_XCBINCLUDEDIR)"

include ../Makefile.targ
include ../Makefile.targ.autoconf
