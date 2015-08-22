-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gtimer

gtimer_SOURCES = \
	main.c \
	gtimer-application.c \
	gtimer-window.c

gtimer_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

gtimer_LDADD = \
	$(GTK3_LIBS)

gtimer_LDFLAGS = \
	-export-dynamic -no-undefined

