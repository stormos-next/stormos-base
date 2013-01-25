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

VER =	gmp-5.1.0

include ../Makefile.defs

TARBALL =		$(VER).tar.bz2
TARBALL_COMPRESS =	-j

PATCHES =	Patches/*
PATCHSTRIP = 0

# Regen makefiles after patching
FROB_SENTINEL.32 = \
	(mkdir -p $(DESTDIR)/usr/share/aclocal ; cd $(VER.32) ; \
	ACLOCAL='aclocal -I$(DESTDIR)/usr/share/aclocal' autoreconf -ivf)

GCC =	/opt/gcc-3.4.3/bin/gcc
GXX =	/opt/gcc-3.4.3/bin/g++

# If we don't force the 32bit ABI libgmp will try to build 64bit
AUTOCONF_ENV += \
	ABI=32

# help borken bootstrap gcc (3.4.3) find libgcc_s.co.1
AUTOCONF_ENV += \
	LD_LIBRARY_PATH=/opt/gcc-3.4.3/lib

AUTOCONF_OPTS += \
	--includedir=/usr/include/gmp \
	--localstatedir=/var \
	--enable-shared \
	--disable-static \
	--disable-libtool-lock \
	--disable-alloca \
	--enable-cxx \
	--enable-fft \
	--enable-mpbsd \
	--disable-fat \
	--with-pic

# If we don't specify a build type gmp will detect the CPU
# and build an optimized and non-portable library.
AUTOCONF_OPTS.32 += \
	--build=i386-pc-solaris2.11 \
	$(AUTOCONF_OPTS)

include ../Makefile.targ
include ../Makefile.targ.autoconf
