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

VER =	emotion-1.7.5

include ../Makefile.defs

# Use fontconfig library from protodir
AUTOCONF_ENV += \
	PKG_CONFIG_PATH=$(DESTDIR)/usr/X11R6/lib/pkgconfig

# Configure does not honor PATH, so override.
AUTOCONF_OPTS += \
	--with-edje-cc=$(DESTDIR)/usr/bin/edje_cc

include ../Makefile.targ
include ../Makefile.targ.autoconf
