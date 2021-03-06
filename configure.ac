# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

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

AC_PREREQ(2.63)
AC_INIT([catta],[0.1],[])
AC_CONFIG_SRCDIR([src/server.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability silent-rules tar-pax subdir-objects])

AC_SUBST(PACKAGE_URL, [http://github.com/everbase/catta/])

AC_SUBST(LIBCATTA_VERSION_INFO, [0:0:0])

AC_CANONICAL_HOST

AM_SILENT_RULES([yes])

AC_CHECK_PROG([STOW], [stow], [yes], [no])

AS_IF([test "x$STOW" = "xyes" && test -d /usr/local/stow], [
        AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
        ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_MKDIR_P
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_GCC_TRADITIONAL

if test x"$ac_cv_prog_cc_c99" = x"no"; then
    AC_MSG_ERROR([C99 support is required])
fi

# Windows stuff
AC_CHECK_DECL(_WIN32)
AM_CONDITIONAL([WINDOWS], [ test "x$ac_cv_have_decl__WIN32" = "xyes" ])
if test "x$ac_cv_have_decl__WIN32" = "xyes" ; then
    CPPFLAGS="$CPPFLAGS -I`pwd`/src/compat/windows/include"
fi
AC_CHECK_MEMBER([struct in_pktinfo.ipi_spec_dst],
    AC_DEFINE([HAVE_IPI_SPEC_DST], [], [struct in_pktinfo has ipi_spec_dst member]),
    [],
    [[#include <netinet/in.h>]])

# iOS stuff
AC_CHECK_DECL(__APPLE__)
AM_CONDITIONAL([APPLE], [ test "x$ac_cv_have_decl___APPLE__" = "xyes" ])
if test "x$ac_cv_have_decl___APPLE__" = "xyes" ; then
    CPPFLAGS="$CPPFLAGS -I`pwd`/src/compat/ios/include"
fi

# -fstack-protector
AC_ARG_ENABLE([stack-protector],
    [AS_HELP_STRING([--disable-stack-protector],
        [Disable GCC's/libc's stack-smashing protection])],
    [case "${enableval}" in
         yes) enable_ssp=yes ;;
          no) enable_ssp=no ;;
           *) AC_MSG_ERROR([invalid value ${enableval} for --disable-stack-protector]) ;;
     esac],
    [enable_ssp=yes])

if test x"$enable_ssp" = x"yes" && test x"$GCC" != x"yes"; then
    AC_MSG_NOTICE([Disabling stack-smashing protection because compiler is not GCC])
    enable_ssp=no
fi

if test x"$enable_ssp" = x"yes"; then
    # Check for broken ssp in libc: http://www.avahi.org/ticket/105
    # libc's brokenness will get in the way regardless of whether -lssp is
    # provided, but provide it anyway (otherwise non-libc ssp would wrongly
    # break here)

    # Get -lssp if it exists
    GCC_STACK_PROTECT_LIB

    AC_MSG_CHECKING([whether stack-smashing protection is available])
    ssp_old_cflags="$CFLAGS"
    ssp_old_ldflags="$LDFLAGS"
    CFLAGS="$CFLAGS -Werror -fstack-protector-all -fPIC"
    LDFLAGS="$LDFLAGS -Wl,-z,defs"
    cat confdefs.h > conftest.c
    cat >>conftest.c <<_ACEOF
void test_broken_ssp(c)
    const char *c;
{
    char arr[[123]], *p; /* beware of possible double-braces if copying this */
    for (p = arr; *c; ++p) {
        *p = *c;
        ++c;
    }
}
_ACEOF
    rm -f conftest.o

    if $CC -c $CFLAGS $CPPFLAGS -o conftest.o conftest.c >/dev/null 2>&1; then
        AC_MSG_RESULT([yes])
        AC_MSG_CHECKING([whether stack-smashing protection is buggy])
        if $CC -o conftest.so $LDFLAGS -shared conftest.o $LIBS >/dev/null 2>&1; then
            AC_MSG_RESULT([no])
        else
            AC_MSG_RESULT([yes])
            enable_ssp=no
        fi
    else
        AC_MSG_RESULT([no])
    fi

    rm -f conftest.c conftest.o conftest.so

    CFLAGS="$ssp_old_cflags"
    LDFLAGS="$ssp_old_ldflags"
fi

if test x"$enable_ssp" = x"yes"; then
    # Do this the long way so we don't call GCC_STACK_PROTECT_LIB twice
    GCC_STACK_PROTECT_CC
fi

# libtool stuff
LT_INIT([win32-dll])

ACX_PTHREAD(,AC_MSG_ERROR([Missing POSIX Threads support]))

#
# Check for netlink.h
#
AC_CHECK_HEADER(linux/netlink.h,
HAVE_NETLINK=yes
AC_DEFINE([HAVE_NETLINK],[],[Support for Linux netlink])
, [], [
#include <sys/socket.h>
#include <asm/types.h>
])

AM_CONDITIONAL(HAVE_NETLINK, [ test x"$HAVE_NETLINK" = xyes ])

if test "x$HAVE_NETLINK" = "xyes" ; then
    # Test whether rtnetlink.h can be included when compiled with -std=c99
    # some distributions (e.g. archlinux) have broken headers that dont
    # define __u64 with -std=c99
    AC_MSG_CHECKING([checking whether rtnetlink.h can be included with -std=c99])
    AC_TRY_COMPILE([#include <linux/rtnetlink.h>], [],
        [AC_MSG_RESULT([yes])], [AC_MSG_ERROR([no])])
fi

#
# Check for net/route.h
#
AC_CHECK_HEADER(net/route.h,
HAVE_PF_ROUTE=yes
AC_DEFINE([HAVE_PF_ROUTE],[],[Support for PF_ROUTE])
, [], [
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_dl.h>
])

AM_CONDITIONAL(HAVE_PF_ROUTE, [ test x"$HAVE_PF_ROUTE" = xyes ])

#
# Check for sys/filio.h; needed for FIONREAD on Solaris
#
AC_CHECK_HEADER(sys/filio.h,
HAVE_SYS_FILIO_H=yes
AC_DEFINE([HAVE_SYS_FILIO_H],[],[Support for sys/filio.h])
, [], [
])

AM_CONDITIONAL(HAVE_SYS_FILIO_H, [ test x"$HAVE_SYS_FILIO_H" = xyes ])

#
# Check for sys/sysctl.h; not present on Solaris
#
AC_CHECK_HEADER(sys/sysctl.h,
HAVE_SYS_SYSCTL=yes
AC_DEFINE([HAVE_SYS_SYSCTL_H],[],[Support for sys/sysctl.h])
, [], [
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
])

AM_CONDITIONAL(HAVE_SYS_SYSCTL_H, [ test x"$HAVE_SYS_SYSCTL_H" = xyes ])

#
# Check for lifconf struct; only present on Solaris
#
AC_MSG_CHECKING(for struct lifconf)
AC_CACHE_VAL(catta_cv_has_struct_lifconf,
[AC_TRY_COMPILE(
[#include <sys/socket.h>
#include <net/if.h>
],[sizeof (struct lifconf);],
catta_cv_has_struct_lifconf=yes,catta_cv_has_struct_lifconf=no)])
AC_MSG_RESULT($catta_cv_has_struct_lifconf)
if test $catta_cv_has_struct_lifconf = yes; then
    AC_DEFINE(HAVE_STRUCT_LIFCONF,1,[Define if there is a struct lifconf.])
fi

#
# Check for struct ip_mreqn
#
AC_MSG_CHECKING(for struct ip_mreqn)
AC_TRY_COMPILE([#include <netinet/in.h>], [
	struct ip_mreqn mreq;
	mreq.imr_address.s_addr = 0;
], [
	# Yes, we have it...
	AC_MSG_RESULT(yes)
	AC_DEFINE([HAVE_STRUCT_IP_MREQN],[],[Support for struct ip_mreqn])
], [
	# We'll just have to try and use struct ip_mreq
	AC_MSG_RESULT(no)
	AC_MSG_CHECKING(for struct ip_mreq)
	AC_TRY_COMPILE([#include <netinet/in.h>], [
		struct ip_mreq mreq;
		mreq.imr_interface.s_addr = 0;
	], [
		# Yes, we have it...
		AC_MSG_RESULT(yes)
		AC_DEFINE([HAVE_STRUCT_IP_MREQ],[],[Support for struct ip_mreq])
	], [
		# No multicast support
			AC_MSG_RESULT(no)
	])
])

test_gcc_flag() {
    AC_LANG_CONFTEST([AC_LANG_SOURCE([[int main() {}]])])
    $CC -c conftest.c $CFLAGS $@ > /dev/null 2> /dev/null
    ret=$?
    rm -f conftest.o
    return $ret
}

# If using GCC specify some additional parameters
if test "x$GCC" = "xyes" ; then

    DESIRED_FLAGS="-Wall -W -Wextra -pedantic -pipe -Wold-style-definition -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wredundant-decls -Wshadow -Wbad-function-cast -Wwrite-strings -fdiagnostics-show-option -fno-strict-aliasing"

    # when compiling for MingW, -Wmissing-noreturn is triggered a bunch of
    # times from autogenerated libtool wrappers, so leave it out on Windows.
    if test "x$ac_cv_have_decl__WIN32" != "xyes" ; then
        DESIRED_FLAGS="$DESIRED_FLAGS -Wmissing-noreturn"
    fi

    for flag in $DESIRED_FLAGS ; do
        AC_MSG_CHECKING([whether $CC accepts $flag])
        if test_gcc_flag $flag ; then
           CFLAGS="$CFLAGS $flag"
           AC_MSG_RESULT([yes])
        else
           AC_MSG_RESULT([no])
        fi
    done
fi

CFLAGS+=" -fPIC"

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

 # Solaris stuff
 AC_SEARCH_LIBS([inet_ntop],[nsl])
 AC_SEARCH_LIBS([recv],[socket])
 AC_CHECK_DECL([CMSG_SPACE],,CFLAGS="$CFLAGS -D_XOPEN_SOURCE=500 -D__EXTENSIONS__", [[#include <sys/socket.h>]])

# Checks for library functions.
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([gethostname select socket uname strcasecmp gettimeofday strncasecmp strlcpy fcntl])

AC_FUNC_CHOWN
AC_FUNC_STAT
AC_TYPE_MODE_T
AC_TYPE_PID_T

AC_CHECK_DECLS(environ)

# check if gcc's -fvisibility is supported
CHECK_VISIBILITY_HIDDEN

# Check for pkg-config manually first, as if its not installed the
# PKG_PROG_PKG_CONFIG macro won't be defined.
AC_CHECK_PROG(have_pkg_config, pkg-config, yes, no)

if test x"$have_pkg_config" = "xno"; then
    AC_MSG_ERROR(pkg-config is required to install this program)
fi

PKG_PROG_PKG_CONFIG

#
# Doxygen
#
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(catta, doxygen.cfg, doxygen)

#
# Conditionally compile test and example programs
#
AC_ARG_ENABLE(tests,
        AS_HELP_STRING([--enable-tests],[Enable building of tests and examples]),
        [case "${enableval}" in
                yes) ENABLE_TESTS=yes ;;
                no)  ENABLE_TESTS=no ;;
                *) AC_MSG_ERROR(bad value ${enableval} for --enable-tests) ;;
        esac],
        [ENABLE_TESTS=no])

AM_CONDITIONAL([ENABLE_TESTS], [test "x$ENABLE_TESTS" = "xyes"])

# ==========================================================================
AC_CONFIG_FILES([
Makefile
src/Makefile
include/Makefile
tests/Makefile
examples/Makefile
])
AC_OUTPUT

# ==========================================================================
echo "
 ---{ $PACKAGE_NAME $VERSION }---

    prefix:                             ${prefix}
    sysconfdir:                         ${sysconfdir}
    localstatedir:                      ${localstatedir}
    C Compiler:                         ${CC}
    CFLAGS:                             ${CFLAGS}
    Enable stack-smashing protection:   ${enable_ssp}
    Building tests:                     ${ENABLE_TESTS}
"
