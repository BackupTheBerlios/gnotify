## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES = \
	$(GTK_CFLAGS)

AM_CFLAGS =\
	`pkg-config --cflags glib-2.0 dbus-glib-1 gconf-2.0 gstreamer-0.8 libxml-2.0`\
	 -Wall\
	 -g

bin_PROGRAMS = gnotify

gnotify_SOURCES = \
	gnotify-backend-dbus.c\
	gnotify-frontend-gtk.c\
	gnotify-startup.c\
	gnotify-backend-gst.c\
	gnotify-backend-gconf.c\
	gnotify-backend-logdog.c\
	base64.c\
	gnotify-frontend-theme-parser.c

gnotify_LDFLAGS = \
	`pkg-config --libs glib-2.0 dbus-glib-1 gconf-2.0 gstreamer-0.8 libxml-2.0`

gnotify_LDADD =  \
	$(GTK_LIBS)

