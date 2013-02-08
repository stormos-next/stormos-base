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

# We keep our GNU crap in /usr/gnu
GNU_DIR = /usr/gnu

PATH := $(PATH):$(DESTDIR)$(GNU_DIR)/bin:$(GNU_DIR)/bin

AUTOCONF_ENV += \
	PKG_CONFIG_PATH="$(DESTDIR)$(GNU_DIR)/lib/pkgconfig"

AUTOCONF_OPTS = \
	--prefix=$(GNU_DIR)

CFLAGS += \
	-I$(DESTDIR)$(GNU_DIR)/include \
	-I$(GNU_DIR)/include 

LDFLAGS += \
	-L$(DESTDIR)$(GNU_DIR)/lib \
	-L$(GNU_DIR)/lib \
	-R$(GNU_DIR)/lib

LDFLAGS.64 += \
	-L$(DESDIR)$(GNU_DIR)/lib/amd64 \
	-L$(GNU_DIR)/lib/amd64 \
	-R$(GNU_DIR)/lib/amd64
