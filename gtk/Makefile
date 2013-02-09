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

VER =	gtk+-2.20.1

include ../Makefile.defs

PATCHES = Patches/*

# Disable -zfatal-warnings for now
LDFLAGS = $(T32_LDFLAGS)

# Help gtk find glib and pango in proto dir
LIBRARY_PATH = \
	$(DESTDIR)/usr/lib:$(DESTDIR)/usr/X11R6/lib

AUTOCONF_ENV += \
	LD_LIBRARY_PATH=$(LIBRARY_PATH)

OVERRIDES += \
	LD_LIBRARY_PATH=$(LIBRARY_PATH)

LDFLAGS += \
	-L$(DESTDIR)/usr/X11R6/lib \
	-R$(DESTDIR)/usr/X11R6/lib

include ../class/x11.mk
include ../Makefile.targ
include ../Makefile.targ.autoconf
