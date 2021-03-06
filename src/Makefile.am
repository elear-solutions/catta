# This file is part of catta.
#
# catta is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# catta is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with catta; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

AM_CFLAGS=-I$(top_srcdir)/include

# This cool debug trap works on i386/gcc only
AM_CFLAGS+='-DDEBUG_TRAP=__asm__("int $$3")'

lib_LTLIBRARIES = libcatta.la

libcatta_la_SOURCES = \
	malloc.c \
	address.c \
	alternative.c \
	error.c \
	strlst.c \
	domain.c \
	timeval.c \
	simple-watch.c \
	thread-watch.c \
	rlist.c \
	utf8.c utf8.h \
	timeeventq.c timeeventq.h \
	iface.c iface.h \
	server.c internal.h entry.c \
	prioq.c prioq.h \
	cache.c cache.h \
	socket.c socket.h \
	response-sched.c response-sched.h \
	query-sched.c query-sched.h \
	probe-sched.c probe-sched.h \
	announce.c announce.h \
	browse.c browse.h \
	rrlist.c rrlist.h \
	resolve-host-name.c \
	resolve-address.c \
	browse-domain.c \
	browse-service-type.c \
	browse-service.c \
	resolve-service.c \
	dns.c dns.h \
	rr.c rr-util.h \
	log.c \
	browse-dns-server.c \
	fdutil.h fdutil.c \
	util.c util.h \
	hashmap.c hashmap.h \
	wide-area.c wide-area.h \
	multicast-lookup.c multicast-lookup.h \
	querier.c querier.h \
	addr-util.h addr-util.c \
	domain-util.h domain-util.c \
	dns-srv-rr.h

if HAVE_NETLINK
libcatta_la_SOURCES += \
       iface-linux.c iface-linux.h \
       netlink.c netlink.h
else
if HAVE_PF_ROUTE
libcatta_la_SOURCES += \
	iface-pfroute.c iface-pfroute.h
else
if WINDOWS
libcatta_la_SOURCES += \
	iface-windows.c	iface-windows.h
else
libcatta_la_SOURCES += \
	iface-none.c
endif
endif
endif

if WINDOWS
# require Windows Vista or above (for winsock2)
AM_CFLAGS+=-DWINVER=0x0600 -D_WIN32_WINNT=0x0600
endif

libcatta_la_CFLAGS = $(AM_CFLAGS) $(PTHREAD_CFLAGS) -fPIC
libcatta_la_LIBADD = $(AM_LDADD) $(PTHREAD_CFLAGS) $(PTHREAD_LIBS)
libcatta_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(LIBCATTA_VERSION_INFO)

if WINDOWS
libcatta_la_SOURCES += \
	compat/windows/wincompat.c \
	compat/windows/wincompat.h
libcatta_la_LIBADD += -lws2_32 -liphlpapi
libcatta_la_LDFLAGS += -no-undefined -Wl,--kill-at,--output-def=.libs/libcatta-0.def
endif
