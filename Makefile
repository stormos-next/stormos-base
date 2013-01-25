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
SUBDIRS = \
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
	openssl1x \
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

STRAP_SUBDIRS = \
	cpp \
	bzip2 \
	libexpat \
	libidn \
	libm \
	libxml \
	libz \
	nss-nspr \
	openssl1x

BUILD_SUBDIRS = \
	binutils \
	flex \
	gcc4 \
	libgmp \
	libmpfr \
	make

NAME =	illumos-extra

AWK =		$(shell (which gawk 2>/dev/null | grep -v "^no ") || which awk)
BRANCH =	$(shell git symbolic-ref HEAD | $(AWK) -F/ '{print $$3}')

ifeq ($(TIMESTAMP),)
  TIMESTAMP =	$(shell date -u "+%Y%m%dT%H%M%SZ")
endif

GITDESCRIBE = \
	g$(shell git describe --all --long | $(AWK) -F'-g' '{print $$NF}')

TARBALL =	$(NAME)-$(BRANCH)-$(TIMESTAMP)-$(GITDESCRIBE).tgz

all: $(BUILD_SUBDIRS) $(SUBDIRS)

strap: $(STRAP_SUBDIRS)

curl: libz openssl1x libidn
gcc4: libgmp libmpfr flex binutils
gzip: libz
libmpfr: libgmp
make: gcc4
node.js: openssl1x libm
ncurses: libm
dialog: ncurses
socat: openssl1x
wget: openssl1x libidn
openldap: openssl1x

#
# pkg-config may be installed. This will actually only hurt us rather than help
# us. pkg-config is based as a part of the blastwave packages and will pull in
# versions of libraries that we have in /opt/csw rather than using the ones in
# /usr that we want. PKG_CONFIG_LIBDIR controls the actual path. This
# environment variable nulls out the search path. Other vars just control what
# gets appended.
#

$(BUILD_SUBDIRS): FRC
	(cd $@ && \
	    PKG_CONFIG_LIBDIR="" \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) install)

$(SUBDIRS): $(BUILD_SUBDIRS)
	(cd $@ && \
	    PKG_CONFIG_LIBDIR="" \
	    STRAP=$(STRAP) \
	    $(MAKE) DESTDIR=$(DESTDIR) install)

install: $(SUBDIRS) $(BUILD_SUBDIRS)

install_strap: $(STRAP_SUBDIRS) $(BUILD_SUBDIRS)

clean: 
	-for dir in $(SUBDIRS) $(BUILD_SUBDIRS); \
	    do (cd $$dir; $(MAKE) DESTDIR=$(DESTDIR) clean); done
	-rm -rf proto

manifest:
	cp manifest $(DESTDIR)/$(DESTNAME)

tarball:
	tar -zcf $(TARBALL) manifest proto

FRC:

.PHONY: manifest
