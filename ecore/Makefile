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

VER =	ecore-1.7.5

include ../Makefile.defs

# Use libraries from Joergs X distribution
CFLAGS += \
	-I$(DESTDIR)/usr/X11R6/include \
	-I/usr/X11R6/include \
	-DXK_MISCELLANY

LDFLAGS += \
	-L$(DESTDIR)/usr/X11R6/lib \
	-L/usr/X11R6/lib \
	-R/usr/X11R6/lib

AUTOCONF_ENV += \
	PKG_CONFIG_PATH=/usr/X11R6/lib/pkgconfig

# ecore requires libsocket
AUTOCONF_ENV += \
	LIBS="-lsocket"

AUTOCONF_OPTS += \
	--enable-curl \
	--enable-ecore-x \
	--enable-ecore-x-input \
	--enable-ecore-x-randr \
	--enable-ecore-x-composite \
	--enable-ecore-evas-software-buffer \
	--enable-ecore-evas-software-x11

include ../Makefile.targ
include ../Makefile.targ.autoconf
