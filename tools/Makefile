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

LDFLAGS = -shared -fPIC -Wl,-M,mapfile -Wl,-z,text -ldl 

all: installfilter 

installfilter-32.so: installfilter.c mapfile
	gcc -m32 -G $(LDFLAGS) installfilter.c -o installfilter-32.so

installfilter-64.so: installfilter.c mapfile
	gcc -m64 -G $(LDFLAGS) installfilter.c -o installfilter-64.so

installfilter: installfilter.sh installfilter-32.so installfilter-64.so
	cp installfilter.sh installfilter
	chmod +x installfilter

clean:
	rm -f installfilter installfilter-32.so installfilter-64.so
