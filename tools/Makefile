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
# Copyright (c) 2013 Andrew Stormont.
#

include ../Makefile.defs

CFLAGS = -G -shared -fPIC
LDFLAGS = -Mmapfile -ztext -zdefs -zinterpose
LIBS = -ldl -lc
SRC = syscall.c file.c string.c redirect.c

all: buildenv

libbuildenv-32.so: $(SRC) mapfile
	$(GCC) $(CFLAGS) $(LDFLAGS:%=-Wl,%) $(LIBS) $(SRC) -o $@

libbuildenv-64.so: $(SRC) mapfile
	$(GCC.64) $(CFLAGS) $(LDFLAGS:%=-Wl,%) $(LIBS) $(SRC) -o $@ 

buildenv: buildenv.c libbuildenv-32.so libbuildenv-64.so
	$(GCC) $< -o $@

redirect.c: redirect.lst genredirect.py
	python genredirect.py $< $@

clean:
	rm -f buildenv redirect.c libbuildenv-32.so libbuildenv-64.so
