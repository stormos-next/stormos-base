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
# Copyright (c) 2012, Joyent, Inc.  All rights reserved.
# Copyright (c) 2013 Andrew Stormont.  All rights reserved.
#

VER =	tar-1.23

include ../Makefile.defs

AUTOCONF_ENV += \
	LD_OPTIONS="-M $(BASE)/mapfile_noexstk"
AUTOCONF_OPTS += \
	--with-rmt=/usr/sbin/rmt \
	--with-xz=/opt/schily/bin/xz \
	--libexecdir=/usr/sbin \
	--program-prefix=g

include ../Makefile.targ

all: all_autoconf

install: all
	mkdir -p $(DESTDIR)/usr/bin
	/usr/ucb/install -c -m 0555 $(BASE)/$(VER.32)/src/tar $(DESTDIR)/usr/bin/gtar
