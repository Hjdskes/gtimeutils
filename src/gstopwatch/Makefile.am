-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gstopwatch

gstopwatch_SOURCES = \
	../../data/gtimeutils-resources.c \
	../radius.c \
	../utils.c \
	gstopwatch-application.c \
	gstopwatch-face.c \
	gstopwatch-radius.c \
	gstopwatch-window.c \
	laps-row.c \
	main.c

gstopwatch_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GSTOPWATCH_CFLAGS) \
	$(AM_CFLAGS)

gstopwatch_LDADD = \
	$(GSTOPWATCH_LIBS) \
	-lm

gstopwatch_LDFLAGS = \
	-export-dynamic -no-undefined

