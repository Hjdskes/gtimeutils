-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gtimer

gtimer_SOURCES = \
	../../data/gtimeutils-resources.c \
	../radius.c \
	../utils.c \
	gtimer-application.c \
	gtimer-face.c \
	gtimer-radius.c \
	gtimer-window.c \
	main.c

gtimer_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GTIMER_CFLAGS) \
	$(AM_CFLAGS)

gtimer_LDADD = \
	$(GTIMER_LIBS) \
	-lm

gtimer_LDFLAGS = \
	-export-dynamic -no-undefined

