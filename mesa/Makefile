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

VER =	Mesa-8.0.5

include ../Makefile.defs
include ../class/x11.mk

TARBALL = MesaLib-8.0.5.tar.gz

PATCHES = Patches/*

# Convince mesa that libX11 is installed
X11_CFLAGS = \
	-I$(X11_DIR)/include

X11_LIBS = \
	-L$(X11_DIR)/lib \
	-R$(X11_DIR)/lib \
	-lX11

AUTOCONF_ENV += \
	X11_CFLAGS="$(X11_CFLAGS)" \
	X11_LIBS="$(X11_LIBS)" \
	DRIGL_CFLAGS="$(X11_CFLAGS)" \
	DRIGL_LIBS="$(X11_LIBS)" \
	XCB_DRI2_CFLAGS="$(X11_CFLAGS)" \
	XCB_DRI2_LIBS="$(X11_LIBS)"

# Enable the bare minium we can get away with
AUTOCONF_OPTS += \
	--enable-32-bit \
	--disable-64-bit \
	--with-gallium-drivers=swrast

# Use python-libxml2 from proto dir
OVERRIDES += \
	PYTHONPATH=$(DESTDIR)/usr/lib/python2.6/site-packages

include ../Makefile.targ
include ../Makefile.targ.autoconf
