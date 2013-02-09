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

VER =	glib-2.30.3

include ../Makefile.defs

# Disable fatal warnings for now
LDFLAGS = $(T32_LDFLAGS)

# dtrace causes the build to fail
AUTOCONF_OPTS += \
	--disable-dtrace

# glib is braindead without gnu icon
AUTOCONF_OPTS += \
	--with-libiconv=gnu

CFLAGS += \
	-I$(DESTDIR)/usr/gnu/include

LDFLAGS += \
	-L$(DESTDIR)/usr/gnu/lib \
	-R$(DESTDIR)/usr/gnu/lib

# use GNU gettext
AUTOCONF_ENV += \
	MSGFMT=$(DESTDIR)/usr/gnu/bin/msgfmt

include ../Makefile.targ
include ../Makefile.targ.autoconf
