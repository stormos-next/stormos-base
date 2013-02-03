#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright (c) 2013, Joyent, Inc.  All rights reserved.
# Copyright (c) 2013 Andrew Stormont.  All rights reserved.
#

VER =	gcc-4.4.4

include ../Makefile.defs

PATCHES = Patches/*
PATCHSTRIP = 0

CLEANFILES +=	$(VER)-i386-build

GXX =	/bin/false

AUTOCONF_OPTS += \
	--with-ld=/usr/bin/ld \
	--without-gnu-ld \
	--with-gnu-as \
	--with-as=$(DESTDIR)/usr/gnu/bin/gas \
	--enable-languages="c,c++" \
	--enable-shared \
	--with-gmp-include="$(DESTDIR)/usr/include/gmp" \
	--with-mpfr-include="$(DESTDIR)/usr/include/mpfr" \
	--disable-nls \
	--with-included-gettext

AUTOCONF_ENV += \
	M4=$(DESTDIR)/usr/gnu/bin/m4

# Need to set LD_LIBRARY_PATH so our broken bootstrap compiler (gcc 3.4.3)
# can find libgcc_s.so.1 and so the compiler built in stage2 can find the
# new libgmp and libmpfr in the proto dir
all: $(AUTOCONF_OUT.32)
	(cd $(VER.32); env - PATH=$(PATH) \
	    LD_LIBRARY_PATH=$(DESTDIR)/usr/lib:/opt/gcc-3.4.3/lib \
	    CFLAGS="-g -O2" \
	    STAGE1_CFLAGS="-g -O2" \
	    CFLAGS_FOR_TARGET="-g -O2" \
	    $(MAKE) $(PARALLEL) $(OVERRIDES) bootstrap)

install: install_autoconf_32

include ../Makefile.targ
