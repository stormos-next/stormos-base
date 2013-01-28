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

VER =	lua-5.1.4

include ../Makefile.defs

PATCHES = Patches/*

all: unpack
	(cd $(VER.32); \
	    env - PATH=$(PATH) \
	    $(MAKE) CC=$(GCC) $(PARALLEL) $(OVERRIDES) DESTDIR=$(DESTDIR))

install: all
	(cd $(VER.32); \
            env - PATH=$(PATH) \
            $(MAKE) $(install_PARALLEL) $(OVERRIDES) DESTDIR=$(DESTDIR) install)

unpack: $(TARBALL)
	-rm -rf $(VER.32)
	mkdir -p .unpack32
	gtar x -C .unpack32 $(TARBALL_COMPRESS) --no-same-owner -f $(TARBALL)
	$(APPLY_PATCHES.32)
	mv -f .unpack32/$(VER) ./$(VER.32)
	-rmdir .unpack32
	touch $(VER.32)/$(UNPACK_SENTINEL)

include ../Makefile.targ
