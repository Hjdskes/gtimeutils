-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gstopwatch

gstopwatch_SOURCES = \
	main.c \
	gstopwatch-application.c \
	gstopwatch-window.c

gstopwatch_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

gstopwatch_LDADD = \
	$(GTK3_LIBS)

gstopwatch_LDFLAGS = \
	-export-dynamic -no-undefined

