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
include ../class/x11.mk

# Convince libxfixes that libx11 is installed
FIXESEXT_CFLAGS = \
	-I$(DESTDIR)$(X11_DIR)/include \
	-I$(X11_DIR)/include

FIXESEXT_LIBS = \
	-L$(X11_DIR)/lib \
	-R$(X11_DIR)/lib \
	-lX11

AUTOCONF_ENV += \
	FIXESEXT_CFLAGS="$(FIXESEXT_CFLAGS)" \
	FIXESEXT_LIBS="$(FIXESEXT_LIBS)"

include ../Makefile.targ
include ../Makefile.targ.autoconf
