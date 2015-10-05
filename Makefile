# Makefile.in generated by automake 1.11.6 from Makefile.am.
# src/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Free Software
# Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.





am__make_dryrun = \
  { \
    am__dry=no; \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        echo 'am--echo: ; @echo "AM"  OK' | $(MAKE) -f - 2>/dev/null \
          | grep '^AM OK$$' >/dev/null || am__dry=yes;; \
      *) \
        for am__flg in $$MAKEFLAGS; do \
          case $$am__flg in \
            *=*|--*) ;; \
            *n*) am__dry=yes; break;; \
          esac; \
        done;; \
    esac; \
    test $$am__dry = yes; \
  }
pkgdatadir = $(datadir)/inf8601-lab1
pkgincludedir = $(includedir)/inf8601-lab1
pkglibdir = $(libdir)/inf8601-lab1
pkglibexecdir = $(libexecdir)/inf8601-lab1
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-unknown-linux-gnu
host_triplet = x86_64-unknown-linux-gnu
bin_PROGRAMS = dragonizer$(EXEEXT)
subdir = src
DIST_COMMON = $(srcdir)/Makefile.am $(srcdir)/Makefile.in
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
LIBRARIES = $(noinst_LIBRARIES)
ARFLAGS = cru
AM_V_AR = $(am__v_AR_$(V))
am__v_AR_ = $(am__v_AR_$(AM_DEFAULT_VERBOSITY))
am__v_AR_0 = @echo "  AR    " $@;
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
libdragon_a_AR = $(AR) $(ARFLAGS)
libdragon_a_LIBADD =
am_libdragon_a_OBJECTS = libdragon_a-color.$(OBJEXT) \
	libdragon_a-utils.$(OBJEXT) libdragon_a-dragon.$(OBJEXT)
libdragon_a_OBJECTS = $(am_libdragon_a_OBJECTS)
libdragontbb_a_AR = $(AR) $(ARFLAGS)
libdragontbb_a_DEPENDENCIES = libdragon.a
am_libdragontbb_a_OBJECTS = dragon_tbb.$(OBJEXT) TidMap.$(OBJEXT)
libdragontbb_a_OBJECTS = $(am_libdragontbb_a_OBJECTS)
am__installdirs = "$(DESTDIR)$(bindir)"
PROGRAMS = $(bin_PROGRAMS)
am_dragonizer_OBJECTS = dragonizer-dragon_pthread.$(OBJEXT) \
	dragonizer-dragonizer.$(OBJEXT)
dragonizer_OBJECTS = $(am_dragonizer_OBJECTS)
dragonizer_DEPENDENCIES = libdragontbb.a libdragon.a
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
dragonizer_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(dragonizer_CFLAGS) \
	$(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
DEFAULT_INCLUDES = -I. -I$(top_builddir)
depcomp = $(SHELL) $(top_srcdir)/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC    " $@;
CCLD = $(CC)
LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD  " $@;
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
AM_V_CXX = $(am__v_CXX_$(V))
am__v_CXX_ = $(am__v_CXX_$(AM_DEFAULT_VERBOSITY))
am__v_CXX_0 = @echo "  CXX   " $@;
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CXXLD = $(am__v_CXXLD_$(V))
am__v_CXXLD_ = $(am__v_CXXLD_$(AM_DEFAULT_VERBOSITY))
am__v_CXXLD_0 = @echo "  CXXLD " $@;
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN   " $@;
SOURCES = $(libdragon_a_SOURCES) $(libdragontbb_a_SOURCES) \
	$(dragonizer_SOURCES)
DIST_SOURCES = $(libdragon_a_SOURCES) $(libdragontbb_a_SOURCES) \
	$(dragonizer_SOURCES)
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
ETAGS = etags
CTAGS = ctags
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
ACLOCAL = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/missing --run aclocal-1.11
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 0
AR = ar
AUTOCONF = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/missing --run autoconf
AUTOHEADER = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/missing --run autoheader
AUTOMAKE = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/missing --run automake-1.11
AWK = gawk
CC = gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -Wall -O2 -fomit-frame-pointer
CPP = gcc -E
CPPFLAGS = 
CXX = g++
CXXCPP = g++ -E
CXXDEPMODE = depmode=gcc3
CXXFLAGS = -Wall -O2 -fomit-frame-pointer -std=c++0x
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DSYMUTIL = 
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /usr/bin/grep -E
EXEEXT = 
FGREP = /usr/bin/grep -F
GREP = /usr/bin/grep
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LD = /usr/bin/ld -m elf_x86_64
LDFLAGS = 
LIBOBJS = 
LIBS = -lstdc++ -lm -ltbb -lpthread 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIPO = 
LN_S = ln -s
LTLIBOBJS = 
MAKEINFO = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/missing --run makeinfo
MANIFEST_TOOL = :
MKDIR_P = /usr/bin/mkdir -p
NM = /usr/bin/nm -B
NMEDIT = 
OBJDUMP = objdump
OBJEXT = o
OPENMP_CFLAGS = -fopenmp
OTOOL = 
OTOOL64 = 
PACKAGE = inf8601-lab1
PACKAGE_BUGREPORT = 
PACKAGE_NAME = INF8601-LAB1
PACKAGE_STRING = INF8601-LAB1 2.1.0
PACKAGE_TARNAME = inf8601-lab1
PACKAGE_URL = 
PACKAGE_VERSION = 2.1.0
PATH_SEPARATOR = :
RANLIB = ranlib
SED = /usr/bin/sed
SET_MAKE = 
SHELL = /bin/sh
STRIP = strip
VERSION = 2.1.0
abs_builddir = /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/src
abs_srcdir = /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/src
abs_top_builddir = /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0
abs_top_srcdir = /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_CXX = g++
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build = x86_64-unknown-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = unknown
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host = x86_64-unknown-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = unknown
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /usagers/frtru3/Documents/dev/INF8601/tp-inf8601/TP1/inf8601-lab1-2.1.0/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = /usr/bin/mkdir -p
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
dragonizer_SOURCES = dragon_pthread.c dragon_pthread.h dragonizer.c
dragonizer_LDADD = libdragontbb.a libdragon.a
dragonizer_CFLAGS = $(OPENMP_CFLAGS)
noinst_LIBRARIES = libdragontbb.a libdragon.a
libdragon_a_SOURCES = color.c color.h utils.c utils.h dragon.c dragon.h
libdragon_a_CFLAGS = $(OPENMP_CFLAGS)
libdragontbb_a_SOURCES = dragon_tbb.cpp dragon_tbb.h TidMap.h TidMap.cpp
libdragontbb_a_LIBADD = libdragon.a
all: all-am

.SUFFIXES:
.SUFFIXES: .c .cpp .lo .o .obj
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --foreign src/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --foreign src/Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

clean-noinstLIBRARIES:
	-test -z "$(noinst_LIBRARIES)" || rm -f $(noinst_LIBRARIES)
libdragon.a: $(libdragon_a_OBJECTS) $(libdragon_a_DEPENDENCIES) $(EXTRA_libdragon_a_DEPENDENCIES) 
	$(AM_V_at)-rm -f libdragon.a
	$(AM_V_AR)$(libdragon_a_AR) libdragon.a $(libdragon_a_OBJECTS) $(libdragon_a_LIBADD)
	$(AM_V_at)$(RANLIB) libdragon.a
libdragontbb.a: $(libdragontbb_a_OBJECTS) $(libdragontbb_a_DEPENDENCIES) $(EXTRA_libdragontbb_a_DEPENDENCIES) 
	$(AM_V_at)-rm -f libdragontbb.a
	$(AM_V_AR)$(libdragontbb_a_AR) libdragontbb.a $(libdragontbb_a_OBJECTS) $(libdragontbb_a_LIBADD)
	$(AM_V_at)$(RANLIB) libdragontbb.a
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(bindir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(bindir)" || exit 1; \
	fi; \
	for p in $$list; do echo "$$p $$p"; done | \
	sed 's/$(EXEEXT)$$//' | \
	while read p p1; do if test -f $$p || test -f $$p1; \
	  then echo "$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n;h' -e 's|.*|.|' \
	    -e 'p;x;s,.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/' | \
	sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1 } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) files[d] = files[d] " " $$1; \
	    else { print "f", $$3 "/" $$4, $$1; } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	    if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	    test -z "$$files" || { \
	    echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files '$(DESTDIR)$(bindir)$$dir'"; \
	    $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files "$(DESTDIR)$(bindir)$$dir" || exit $$?; \
	    } \
	; done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	files=`for p in $$list; do echo "$$p"; done | \
	  sed -e 'h;s,^.*/,,;s/$(EXEEXT)$$//;$(transform)' \
	      -e 's/$$/$(EXEEXT)/' `; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(bindir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(bindir)" && rm -f $$files

clean-binPROGRAMS:
	@list='$(bin_PROGRAMS)'; test -n "$$list" || exit 0; \
	echo " rm -f" $$list; \
	rm -f $$list || exit $$?; \
	test -n "$(EXEEXT)" || exit 0; \
	list=`for p in $$list; do echo "$$p"; done | sed 's/$(EXEEXT)$$//'`; \
	echo " rm -f" $$list; \
	rm -f $$list
dragonizer$(EXEEXT): $(dragonizer_OBJECTS) $(dragonizer_DEPENDENCIES) $(EXTRA_dragonizer_DEPENDENCIES) 
	@rm -f dragonizer$(EXEEXT)
	$(AM_V_CCLD)$(dragonizer_LINK) $(dragonizer_OBJECTS) $(dragonizer_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/TidMap.Po
include ./$(DEPDIR)/dragon_tbb.Po
include ./$(DEPDIR)/dragonizer-dragon_pthread.Po
include ./$(DEPDIR)/dragonizer-dragonizer.Po
include ./$(DEPDIR)/libdragon_a-color.Po
include ./$(DEPDIR)/libdragon_a-dragon.Po
include ./$(DEPDIR)/libdragon_a-utils.Po

.c.o:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c $<

.c.obj:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c `$(CYGPATH_W) '$<'`

.c.lo:
	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	$(AM_V_CC)source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(LTCOMPILE) -c -o $@ $<

libdragon_a-color.o: color.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-color.o -MD -MP -MF $(DEPDIR)/libdragon_a-color.Tpo -c -o libdragon_a-color.o `test -f 'color.c' || echo '$(srcdir)/'`color.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-color.Tpo $(DEPDIR)/libdragon_a-color.Po
#	$(AM_V_CC)source='color.c' object='libdragon_a-color.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-color.o `test -f 'color.c' || echo '$(srcdir)/'`color.c

libdragon_a-color.obj: color.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-color.obj -MD -MP -MF $(DEPDIR)/libdragon_a-color.Tpo -c -o libdragon_a-color.obj `if test -f 'color.c'; then $(CYGPATH_W) 'color.c'; else $(CYGPATH_W) '$(srcdir)/color.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-color.Tpo $(DEPDIR)/libdragon_a-color.Po
#	$(AM_V_CC)source='color.c' object='libdragon_a-color.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-color.obj `if test -f 'color.c'; then $(CYGPATH_W) 'color.c'; else $(CYGPATH_W) '$(srcdir)/color.c'; fi`

libdragon_a-utils.o: utils.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-utils.o -MD -MP -MF $(DEPDIR)/libdragon_a-utils.Tpo -c -o libdragon_a-utils.o `test -f 'utils.c' || echo '$(srcdir)/'`utils.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-utils.Tpo $(DEPDIR)/libdragon_a-utils.Po
#	$(AM_V_CC)source='utils.c' object='libdragon_a-utils.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-utils.o `test -f 'utils.c' || echo '$(srcdir)/'`utils.c

libdragon_a-utils.obj: utils.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-utils.obj -MD -MP -MF $(DEPDIR)/libdragon_a-utils.Tpo -c -o libdragon_a-utils.obj `if test -f 'utils.c'; then $(CYGPATH_W) 'utils.c'; else $(CYGPATH_W) '$(srcdir)/utils.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-utils.Tpo $(DEPDIR)/libdragon_a-utils.Po
#	$(AM_V_CC)source='utils.c' object='libdragon_a-utils.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-utils.obj `if test -f 'utils.c'; then $(CYGPATH_W) 'utils.c'; else $(CYGPATH_W) '$(srcdir)/utils.c'; fi`

libdragon_a-dragon.o: dragon.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-dragon.o -MD -MP -MF $(DEPDIR)/libdragon_a-dragon.Tpo -c -o libdragon_a-dragon.o `test -f 'dragon.c' || echo '$(srcdir)/'`dragon.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-dragon.Tpo $(DEPDIR)/libdragon_a-dragon.Po
#	$(AM_V_CC)source='dragon.c' object='libdragon_a-dragon.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-dragon.o `test -f 'dragon.c' || echo '$(srcdir)/'`dragon.c

libdragon_a-dragon.obj: dragon.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -MT libdragon_a-dragon.obj -MD -MP -MF $(DEPDIR)/libdragon_a-dragon.Tpo -c -o libdragon_a-dragon.obj `if test -f 'dragon.c'; then $(CYGPATH_W) 'dragon.c'; else $(CYGPATH_W) '$(srcdir)/dragon.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/libdragon_a-dragon.Tpo $(DEPDIR)/libdragon_a-dragon.Po
#	$(AM_V_CC)source='dragon.c' object='libdragon_a-dragon.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libdragon_a_CFLAGS) $(CFLAGS) -c -o libdragon_a-dragon.obj `if test -f 'dragon.c'; then $(CYGPATH_W) 'dragon.c'; else $(CYGPATH_W) '$(srcdir)/dragon.c'; fi`

dragonizer-dragon_pthread.o: dragon_pthread.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -MT dragonizer-dragon_pthread.o -MD -MP -MF $(DEPDIR)/dragonizer-dragon_pthread.Tpo -c -o dragonizer-dragon_pthread.o `test -f 'dragon_pthread.c' || echo '$(srcdir)/'`dragon_pthread.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/dragonizer-dragon_pthread.Tpo $(DEPDIR)/dragonizer-dragon_pthread.Po
#	$(AM_V_CC)source='dragon_pthread.c' object='dragonizer-dragon_pthread.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -c -o dragonizer-dragon_pthread.o `test -f 'dragon_pthread.c' || echo '$(srcdir)/'`dragon_pthread.c

dragonizer-dragon_pthread.obj: dragon_pthread.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -MT dragonizer-dragon_pthread.obj -MD -MP -MF $(DEPDIR)/dragonizer-dragon_pthread.Tpo -c -o dragonizer-dragon_pthread.obj `if test -f 'dragon_pthread.c'; then $(CYGPATH_W) 'dragon_pthread.c'; else $(CYGPATH_W) '$(srcdir)/dragon_pthread.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/dragonizer-dragon_pthread.Tpo $(DEPDIR)/dragonizer-dragon_pthread.Po
#	$(AM_V_CC)source='dragon_pthread.c' object='dragonizer-dragon_pthread.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -c -o dragonizer-dragon_pthread.obj `if test -f 'dragon_pthread.c'; then $(CYGPATH_W) 'dragon_pthread.c'; else $(CYGPATH_W) '$(srcdir)/dragon_pthread.c'; fi`

dragonizer-dragonizer.o: dragonizer.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -MT dragonizer-dragonizer.o -MD -MP -MF $(DEPDIR)/dragonizer-dragonizer.Tpo -c -o dragonizer-dragonizer.o `test -f 'dragonizer.c' || echo '$(srcdir)/'`dragonizer.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/dragonizer-dragonizer.Tpo $(DEPDIR)/dragonizer-dragonizer.Po
#	$(AM_V_CC)source='dragonizer.c' object='dragonizer-dragonizer.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -c -o dragonizer-dragonizer.o `test -f 'dragonizer.c' || echo '$(srcdir)/'`dragonizer.c

dragonizer-dragonizer.obj: dragonizer.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -MT dragonizer-dragonizer.obj -MD -MP -MF $(DEPDIR)/dragonizer-dragonizer.Tpo -c -o dragonizer-dragonizer.obj `if test -f 'dragonizer.c'; then $(CYGPATH_W) 'dragonizer.c'; else $(CYGPATH_W) '$(srcdir)/dragonizer.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/dragonizer-dragonizer.Tpo $(DEPDIR)/dragonizer-dragonizer.Po
#	$(AM_V_CC)source='dragonizer.c' object='dragonizer-dragonizer.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dragonizer_CFLAGS) $(CFLAGS) -c -o dragonizer-dragonizer.obj `if test -f 'dragonizer.c'; then $(CYGPATH_W) 'dragonizer.c'; else $(CYGPATH_W) '$(srcdir)/dragonizer.c'; fi`

.cpp.o:
	$(AM_V_CXX)$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CXX)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) \
#	$(AM_V_CXX_no)$(CXXCOMPILE) -c -o $@ $<

.cpp.obj:
	$(AM_V_CXX)$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CXX)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) \
#	$(AM_V_CXX_no)$(CXXCOMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

.cpp.lo:
	$(AM_V_CXX)$(LTCXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	$(AM_V_CXX)source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) \
#	$(AM_V_CXX_no)$(LTCXXCOMPILE) -c -o $@ $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	mkid -fID $$unique
tags: TAGS

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	set x; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: CTAGS
CTAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(LIBRARIES) $(PROGRAMS)
installdirs:
	for dir in "$(DESTDIR)$(bindir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-binPROGRAMS clean-generic clean-libtool \
	clean-noinstLIBRARIES mostlyclean-am

distclean: distclean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am: install-binPROGRAMS

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am: uninstall-binPROGRAMS

.MAKE: install-am install-strip

.PHONY: CTAGS GTAGS all all-am check check-am clean clean-binPROGRAMS \
	clean-generic clean-libtool clean-noinstLIBRARIES ctags \
	distclean distclean-compile distclean-generic \
	distclean-libtool distclean-tags distdir dvi dvi-am html \
	html-am info info-am install install-am install-binPROGRAMS \
	install-data install-data-am install-dvi install-dvi-am \
	install-exec install-exec-am install-html install-html-am \
	install-info install-info-am install-man install-pdf \
	install-pdf-am install-ps install-ps-am install-strip \
	installcheck installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags uninstall uninstall-am uninstall-binPROGRAMS


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
