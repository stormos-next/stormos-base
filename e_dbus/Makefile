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

VER =	e_dbus-1.7.5

include ../Makefile.defs

# e_dbus needs some convincing that dbus is actually installed
# since we don't have a pkg-config file for it
DBUS_CFLAGS = \
	-I/usr/include/dbus-1.0 \
	-I/usr/lib/dbus-1.0/include

DBUS_LIBS = \
	-ldbus-1

ECORE_CFLAGS = \
	-I$(DESTDIR)/usr/include/ecore-1 \
	-I$(DESTDIR)/usr/include/eina-1 \
	-I$(DESTDIR)/usr/include/eina-1/eina

ECORE_LIBS = \
	-lecore \
	-leina \
	-lpthread

AUTOCONF_ENV += \
	DBUS_CFLAGS="$(DBUS_CFLAGS)" \
	DBUS_LIBS="$(DBUS_LIBS)" \
	EDBUS_CFLAGS="$(DBUS_CFLAGS) $(ECORE_CFLAGS)" \
	EDBUS_LIBS="$(DBUS_LIBS) $(ECORE_LIBS)"

AUTOCONF_OPTS += \
	--disable-edbus-test \
	--disable-edbus-test-client \
	--disable-edbus-async-test \
	--disable-edbus-bluez-test \
	--disable-edbus-connman0_7x-test \
	--disable-edbus-ofono-test \
	--disable-edbus-ukit-test

include ../Makefile.targ
include ../Makefile.targ.autoconf
