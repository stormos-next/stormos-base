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

VER =	enlightenment-0.17.0

include ../Makefile.defs

# enlightenment needs some convincing that dbus is actually installed
# since we don't have a pkg-config file for it
DBUS_CFLAGS = \
	-I/usr/include/dbus-1.0 \
	-I/usr/lib/dbus-1.0/include

DBUS_LIBS = \
	-ldbus-1

EDBUS_CFLAGS = \
	-I$(DESTDIR)/usr/include/e_dbus-1

EDBUS_LIBS = \
	-ledbus \
	-ldbus-1

ECORE_CFLAGS = \
	-I$(DESTDIR)/usr/include/ecore-1 \
	-I$(DESTDIR)/usr/include/eet-1 \
	-I$(DESTDIR)/usr/include/edje-1 \
	-I$(DESTDIR)/usr/include/efreet-1 \
	-I$(DESTDIR)/usr/include/eina-1 \
	-I$(DESTDIR)/usr/include/eina-1/eina \
	-I$(DESTDIR)/usr/include/eio-1

ECORE_LIBS = \
	-leet \
	-lecore \
	-lecore_file \
	-lecore_input \
	-lecore_input_evas \
	-lecore_ipc \
	-lecore_x \
	-ledje \
	-lefreet \
	-lehal \
	-leina \
	-leio \
	-lpthread

XCB_CFLAGS = \
	-I$(DESTDIR)/usr/X11R6/include

XCB_LDFLAGS = \
	-L$(DESTDIR)/usr/X11R6/lib \
	-R/usr/X11R6/lib \
	-lxcb \
	-lxcb-keysyms \
	-lxcb-shape

AUTOCONF_ENV += \
	E_FM_CFLAGS="$(DBUS_CFLAGS) $(EDBUS_CFLAGS) $(ECORE_CFLAGS) $(XCB_CFLAGS)" \
	E_FM_LIBS="$(DBUS_LIBS) $(EDBUS_LIBS) $(ECORE_LIBS) $(XCB_LDFLAGS)" \
	E_FM_CMDLINE_CFLAGS="$(DBUS_CFLAGS) $(EDBUS_CFLAGS) $(ECORE_CFLAGS) $(XCB_CFLAGS)" \
	E_FM_CMDLINE_LIBS="$(DBUS_LIBS) $(EDBUS_LIBS) $(ECORE_LIBS) $(XCB_LDFLAGS)" \
	E_ALERT_CFLAGS="$(DBUS_CFLAGS) $(EDBUS_CFLAGS) $(ECORE_CFLAGS) $(XCB_CFLAGS)" \
	E_ALERT_LIBS="$(DBUS_LIBS) $(EDBUS_LIBS) $(ECORE_LIBS) $(XCB_LDFLAGS)" \
	E_CFLAGS="$(DBUS_CFLAGS) $(EDBUS_CFLAGS) $(ECORE_CFLAGS) $(XCB_CFLAGS)" \
	E_LIBS="$(DBUS_LIBS) $(EDBUS_LIBS) $(ECORE_LIBS) $(XCB_LDFLAGS)"	

# Use fontconfig library from Joergs X distribution
AUTOCONF_ENV += \
	PKG_CONFIG_PATH="/usr/X11R6/lib/pkgconfig"

# And X headers too
CFLAGS += \
	-I/usr/X11R6/include

# Configure does not honor PATH, so override.
AUTOCONF_OPTS += \
	--with-edje-cc=$(DESTDIR)/usr/bin/edje_cc \
	--with-eet-eet=$(DESTDIR)/usr/bin/eet

# No udev for us... hooray!
AUTOCONF_OPTS += \
	--enable-device-hal

# Help eet find it's libraries
OVERRIDES += \
	LD_LIBRARY_PATH="$(DESTDIR)/usr/lib"

include ../Makefile.targ
include ../Makefile.targ.autoconf
