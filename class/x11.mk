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

# We store X and related libraries in /usr/X11R6
X11_DIR = /usr/X11R6

PATH := $(PATH):$(DESTDIR)$(X11_DIR)/bin:$(X11_DIR)/bin

AUTOCONF_ENV += \
	PKG_CONFIG_PATH="$(DESTDIR)$(X11_DIR)/lib/pkgconfig"

AUTOCONF_OPTS = \
	--prefix=$(X11_DIR)

CFLAGS += \
	-I$(DESTDIR)$(X11_DIR)/include \
	-I$(X11_DIR)/include 

LDFLAGS += \
	-L$(DESTDIR)$(X11_DIR)/lib \
	-L$(X11_DIR)/lib \
	-R$(X11_DIR)/lib