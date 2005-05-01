/***************************************************************************
 *            gnotify-backend.h
 *
 *  Fri Apr 29 14:25:38 2005
 *  Copyright  2005  Marius M. M., Sebastian Kaminski
 *  DevilX33@hotmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef _GNOTIFY_BACKEND_H
#define _GNOTIFY_BACKEND_H

#include <glib.h>
#include <gconf/gconf-client.h>
#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus-glib-lowlevel.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gst/gst.h>
#include <string.h>

#define NO_THREADS	5
#define GCONF_PATH "/apps/gnotify"
#define LOGFILE_NAME "gnotify.log"

#ifdef __cplusplus
extern "C"
{
#endif

struct _GnotifyConfig
{
	GConfClient *config;
	
	gint display_mode; /* Popup? Dialog? Whatever? */
	gint align_x; /* horizontal alignment (0-100) */
	gint align_y; /* vertical alignment (0-100) */

	gint popup_timeout;

	gboolean enable_sound; /* Should sound be enabled? */
	gchar *gst_audiosink;
	gchar *dbus_address; /* e.g. unix:abstract=/var/run/dbus/system_bus_socket */
	
	gboolean enable_logging; /* Should logging be enabled? */
	gchar *logfile;
	FILE *logf;
	
}GnotifyConfig;

typedef struct 
{
	gchar *title;
	gchar *sound;
	gchar *message;
	GdkPixbuf *icon;
	GThread	*SoundThread;
}GnotifyThreadData;

/* Gnotify-Startup */
void end(void);
void signalhandler(gint sign);


/* D-BUS Backend */
DBusServer *gnotify_backend_dbus_init(const gchar *address);
void gnotify_backend_dbus_connection(DBusServer *server, DBusConnection *con, gpointer user_data);

DBusHandlerResult gnotify_backend_dbus_message_filter(DBusConnection *con, DBusMessage *msg, gpointer user_data);
DBusHandlerResult gnotify_backend_dbus_message_filter_disconnect(DBusConnection *con, DBusMessage *msg, gpointer user_data);

GnotifyThreadData *gnotify_backend_threaddata_new(void);
void gnotify_backend_threaddata_free(GnotifyThreadData *data);

void gnotify_backend_threaddata_free_pixbuf(guchar *pixels, gpointer data);

/* GConf Backend */
gint gnotify_backend_gconf_load(void);


/* GST Backend */
gboolean gnotify_backend_gst_play(gpointer sfile);


/* LogDog Backend */
gboolean gnotify_backend_logdog_init(void);
void gnotify_backend_logdog_print(const gchar *format, ...);
void gnotify_backend_logdog_exit(void);
gchar *gnotify_backend_logdog_gtd(const gchar* format);


#ifdef __cplusplus
}
#endif

#endif /* _GNOTIFY-BACKEND_H */
