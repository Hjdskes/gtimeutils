-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gstopwatch

gstopwatch_SOURCES = \
	main.c \
	gstopwatch-application.c \
	gstopwatch-window.c

gstopwatch_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GSTOPWATCH_CFLAGS) \
	$(AM_CFLAGS)

gstopwatch_LDADD = \
	$(GSTOPWATCH_LIBS)

gstopwatch_LDFLAGS = \
	-export-dynamic -no-undefined

