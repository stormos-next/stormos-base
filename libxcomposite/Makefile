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

VER = 	libXcomposite-0.4.4

include ../Makefile.defs
include ../class/x11.mk

# Convince libXcomposite that xlibs are installed
XCOMPOSITE_CFLAGS = \
	-I$(DESTDIR)$(X11_DIR)/include \
	-I$(X11_DIR)/include 

XCOMPOSITE_LIBS = \
	-L$(X11_DIR)/lib \
	-R$(X11_DIR)/lib

XFIXES_CFLAGS = \
	-I$(DESTDIR)$(X11_DIR)/include \
	-I$(X11_DIR)/include

XFIXES_LIBS = \
	-L$(DESTDIR)$(X11_DIR)/lib \
	-R$(X11_DIR)/lib \
	-lXfixes

AUTOCONF_ENV += \
	XCOMPOSITE_CFLAGS="$(XCOMPOSITE_CFLAGS)" \
	XCOMPOSITE_LIBS="$(XCOMPOSITE_LIBS)" \
	XFIXES_CFLAGS="$(XFIXES_CFLAGS)" \
	XFIXES_LIBS="$(XFIXES_LIBS)"

include ../Makefile.targ
include ../Makefile.targ.autoconf
