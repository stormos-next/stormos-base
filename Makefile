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
# Copyright (c) 2012, Joyent, Inc.
# Copyright (c) 2013 Andrew Stormont.
#
# To build everything just run 'gmake' in this directory.
#

BASE =		$(PWD)
DESTDIR =	$(BASE)/proto
PATH =		$(DESTDIR)/usr/bin:/usr/bin:/usr/sbin:/sbin:/opt/csw/bin

BASE_SUBDIRS = \
	bash \
	bind \
	bzip2 \
	coreutils \
	cpp \
	curl \
	dialog \
	g11n \
	gnupg \
	gtar \
	gzip \
	ipmitool \
	less \
	libexpat \
	libidn \
	libm \
	libxml \
	libz \
	ncurses \
	node.js \
	nss-nspr \
	ntp \
	openldap \
	openssl \
	pbzip2 \
	perl \
	rsync \
	rsyslog \
	screen \
	socat \
	tun \
	uuid \
	vim \
	wget

DESKTOP_SUBDIRS = \
	compositeproto \
	dri2proto \
	e_dbus \
	ecore \
	edje \
	eet \
	efreet \
	eina \
	eio \
	elementary \
	embryo \
	emotion \
	enlightenment \
	ethumb \
	evas \
	evas_generic_loaders \
	fixesproto \
	glproto \
	libast \
	libdrm \
	libgif \
	libiconv \
	libjpeg \
	liblua \
	libpixman \
	libpng \
	libtiff \
	libxau \
	libxcb \
	libxcomposite \
	libxext \
	libxrandr \
	libxrender \
	libxslt \
	makedepend \
	mesa \
	pciaccess \
	pthread-stubs \
	randrproto \
	renderproto \
	xcb-proto \
	xcb-util \
	xcb-util-image \
	xcb-util-keysyms \
	xextproto \
	xproto

TOOLCHAIN_SUBDIRS = \
	autoconf \
	automake \
	binutils \
	bison \
	flex \
	gcc4 \
	libgmp \
	libmpfr \
	m4 \
	make \
	pkg-config

NAME =	illumos-extra

AWK =		$(shell (which gawk 2>/dev/null | grep -v "^no ") || which awk)
BRANCH =	$(shell git symbolic-ref HEAD | $(AWK) -F/ '{print $$3}')

ifeq ($(TIMESTAMP),)
  TIMESTAMP =	$(shell date -u "+%Y%m%dT%H%M%SZ")
endif

GITDESCRIBE = \
	g$(shell git describe --all --long | $(AWK) -F'-g' '{print $$NF}')

TARBALL =	$(NAME)-$(BRANCH)-$(TIMESTAMP)-$(GITDESCRIBE).tgz

# Get this in before make starts processing targets
.DEFAULT_GOAL = all 

curl: libz openssl1x libidn
gzip: libz
node.js: openssl1x libm
ncurses: libm
dialog: ncurses
socat: openssl1x
wget: openssl1x libidn
openldap: openssl1x

#
# toolchain dependencies
#
autoconf: m4
automake: autoconf m4
bison: flex
gcc4: libgmp libmpfr m4 flex bison binutils
flex: m4
libmpfr: libgmp
make: gcc4

#
# enlightenment dependencies 
#
e_dbus: ecore eina
ecore: curl evas_generic_loaders libxrandr libxrender
edje: ecore eet eina embryo liblua
eet: libz libjpeg eina
efreet: edje
eina: libiconv
eio: ecore
elementary: eet edje embryo
embryo: eio
emotion: edje
enlightenment: libxcb xcb-util xcb-util-keysyms emotion
ethumb: edje emotion
evas: eet libgif libjpeg libpng eina libpixman xcb-util-image
evas_generic_loaders: evas
fixesproto: xextproto
libdrm: pciaccess pthread-stubs
libpng: libz
libxcomposite: compositeproto
libxcb: libxslt xcb-proto pthread-stubs libxau
libxext: xproto xextproto
libxfixes: xextproto
libxrandr: randrproto libxrender
libxrender: renderproto
makedepend: xproto
mesa: makedepend dri2proto libxfixes glproto libdrm libexpat libxml libxext
xcb-util: xproto libxcb xcb-proto
xcb-util-image: xcb-util xproto libxcb xcb-proto
xcb-util-keysyms: xcb-util xproto libxcb xcb-proto

#
# Toolchain rules: Used to build the and stage the gnu toolchain
# in the proto area.  It will be used instead of the host tools.
#

# (subdirname): build and install into protodir 
$(TOOLCHAIN_SUBDIRS): FRC
	(cd $@ && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) install)

# install-(subdirname): install onto host
$(TOOLCHAIN_SUBDIRS:%=install-%): $@ 
	(cd $(patsubst install-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=/ install)

# clean-(subdirname): sanitize build directory
$(TOOLCHAIN_SUBDIRS:%=clean-%): FRC
	(cd $(patsubst clean-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) clean)

toolchain: $(TOOLCHAIN_SUBDIRS)
install-toolchain: $(TOOLCHAIN_SUBDIRS:%=install-%)
clean-toolchain: $(TOOLCHAIN_SUBDIRS:%=clean-%)

#
# Enlightenment rules: Used to build and stage enlightenment
# in the proto area.  It's headers and libraries will be used.
#

# (subdirname): build and install into proto dir
$(DESKTOP_SUBDIRS): FRC
	(cd $@ && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) install)

# install-(subdirname): install onto host
$(DESKTOP_SUBDIRS:%=install-%): $@
	(cd $(patsubst install-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=/ install)

# clean-(subdirname): sanitize build directory
$(DESKTOP_SUBDIRS:%=clean-%): FRC
	(cd $(patsubst clean-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) clean)

desktop: $(DESKTOP_SUBDIRS)
install-desktop: $(DESKTOP_SUBDIRS:%=install-%)
clean-toolchain: $(DESKTOP_SUBDIRS:%=clean-%)

#
# Base rules: Used to build the basic utilities and libraries
# TODO: Finalize this!
#

# (subdirname): build and install into proto dir 
$(BASE_SUBDIRS): FRC
	(cd $@ && \
	    SRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) install)

# install-(subdirname): install onto host
$(BASE_SUBDIRS:%=install-%): $@
	(cd $(patsubst install-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=/ install)

# clean-(subdirname): sanitize build directory
$(BASE_SUBDIRS:%=clean-%): FRC
	(cd $(patsubst clean-%,%,$@) && \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) clean)

base: $(BASE_SUBDIRS)
install-base: $(BASE_SUBDIRS:%=install-%)
clean-base: $(BASE_SUBDIRS:%=clean-%)

#
# Default rules.  Build/Clean/Install everything.
#

all: $(TOOLCHAIN_SUBDIRS) $(BASE_SUBDIRS) $(DESKTOP_SUBDIRS)
install: $(TOOLCHAIN_SUBDIRS:%=install-%) $(BASE_SUBDIRS:%=build-%) $(DESKTOP_SUBDIRS:%=build-%)
clean: $(TOOLCHAIN_SUBDIRS:%=clean-%) $(BASE_SUBDIRS:%=clean-%) $(DESKTOP_SUBDIRS:%=clean-%)
	# Call me paranoid
	[ "$(DESTDIR)" = "/" ] || rm -rf $(DESTDIR)	

# Extra joyent stuff
manifest:
	cp manifest $(DESTDIR)/$(DESTNAME)

tarball:
	tar -zcf $(TARBALL) manifest proto

FRC:

.PHONY: manifest
