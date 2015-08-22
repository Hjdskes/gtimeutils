AC_INIT([GTimeUtils], 2.0, [hjdskes@gmail.com], [gtimeutils],
	[https://github.com/Unia/gtimeutils/])

AM_INIT_AUTOMAKE([1.11 foreign dist-xz no-dist-gzip subdir-objects -Wno-portability])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CC_STDC
AC_CONFIG_HEADERS([config.h])

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

# Internationalization
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.50.1])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package.])

# Package requirements
PKG_CHECK_MODULES([GTK3], [gtk+-3.0 >= 3.12.0])

AC_CONFIG_FILES(Makefile
		data/Makefile
		data/gstopwatch.desktop.in
		data/gtimer.desktop.in
		src/Makefile
		src/gstopwatch/Makefile
		src/gtimer/Makefile
		po/Makefile.in)

AC_OUTPUT

AC_MSG_RESULT([
        $PACKAGE $VERSION
        ========

        prefix:                 ${prefix}
        libdir:                 ${libdir}
        sysconfdir:             ${sysconfdir}
        exec_prefix:            ${exec_prefix}
        bindir:                 ${bindir}
        datarootdir:            ${datarootdir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}
])
