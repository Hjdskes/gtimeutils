@INTLTOOL_DESKTOP_RULE@
DESKTOP_IN_FILES = \
	org.unia.gtimeutils.gstopwatch.desktop.in.in \
	org.unia.gtimeutils.gtimer.desktop.in.in
DESKTOP_FILES = $(DESKTOP_IN_FILES:.desktop.in.in=.desktop)

desktopdir = $(datadir)/applications
desktop_DATA = $(DESKTOP_FILES)

EXTRA_DIST = \
	$(DESKTOP_IN_FILES) \
	gtimeutils.gresource.xml \
	gnome-clocks.css

resource_files = $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(top_srcdir)/data --generate-dependencies $(top_srcdir)/data/gtimeutils.gresource.xml)

BUILT_SOURCES = \
	gtimeutils-resources.h \
	gtimeutils-resources.c

gtimeutils-resources.h: $(top_srcdir)/data/gtimeutils.gresource.xml $(resource_files)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-header --c-name gtimeutils $<
gtimeutils-resources.c: $(top_srcdir)/data/gtimeutils.gresource.xml $(resource_files)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-source --c-name gtimeutils $<

DISTCLEANFILES = \
	$(DESKTOP_FILES)
