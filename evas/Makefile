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

VER =	evas-1.7.5

include ../Makefile.defs

PATCHES = Patches/*
PATCHSTRIP = 0

# Convince evas that xlibs are installed
XCB_CFLAGS = \
	-I$(DESTDIR)/usr/X11R6/include \
	-I/usr/X11R6/include 

XCB_LDFLAGS = \
	-L$(DESTDIR)/usr/X11R6/lib \
	-R/usr/X11R6

# libevas requires libsocket
LIBS += -lsocket -lnsl

CFLAGS += -D_XOPEN_SOURCE=500

# use libX11 from Joerg's X distribution
CFLAGS += \
	-I$(DESTDIR)/usr/X11R6/include \
	-I/usr/X11R6/include

LDFLAGS += \
	-L$(DESTDIR)/usr/X11R6/lib \
	-L/usr/X11R6/lib \
	-R/usr/X11R6/lib

AUTOCONF_ENV += \
	XCB_CFLAGS="$(XCB_CFLAGS)" \
	XCB_LIBS="$(XCB_LDFLAGS)" \
	LIBS="$(LIBS)"

# Use pixman, freetype and fontconfig from proto area
AUTOCONF_ENV += \
	PKG_CONFIG_PATH="$(DESTDIR)/usr/X11R6/lib/pkgconfig"

AUTOCONF_OPTS += \
	--with-x \
	--enable-pixman \
	--enable-gl-xlib \
	--enable-software-xlib \
	--disable-software-xcb \
	--enable-image-loader-png \
	--enable-image-loader-jpeg \
	--enable-image-loader-generic

include ../Makefile.targ
include ../Makefile.targ.autoconf
