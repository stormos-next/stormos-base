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

VER =	libXfixes-5.0

include ../Makefile.defs

# Put this with the rest of the X stuff
AUTOCONF_OPTS = \
	--prefix=/usr/X11R6

# Use x stuff from proto dir
AUTOCONF_ENV += \
	PKG_CONFIG_PATH=$(DESTDIR)/usr/X11R6/lib/pkgconfig

# Convince libxfixes that libx11 is installed
FIXESEXT_CFLAGS = \
	-I$(DESTDIR)/usr/X11R6/include \
	-I/usr/X11R6/include

FIXESEXT_LIBS = \
	-L/usr/X11R6/lib \
	-R/usr/X11R6/lib \
	-lX11

AUTOCONF_ENV += \
	FIXESEXT_CFLAGS="$(FIXESEXT_CFLAGS)" \
	FIXESEXT_LIBS="$(FIXESEXT_LIBS)"

include ../Makefile.targ
include ../Makefile.targ.autoconf
