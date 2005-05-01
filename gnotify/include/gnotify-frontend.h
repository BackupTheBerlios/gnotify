/***************************************************************************
 *            gnotify-frontend.h
 *
 *  Fri Apr 29 14:25:54 2005
 *  Copyright  2005  Sebastian Kaminski
 *  kaugume@sebk.de
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
 
#ifndef _GNOTIFY_FRONTEND_H
#define _GNOTIFY_FRONTEND_H

#include "include/gnotify-backend.h"
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* GTK start */
	
enum {
	GNOTIFY_FRONTEND_GTK_MODE_POPUP,
	GNOTIFY_FRONTEND_GTK_MODE_DIALOG
	};
	
struct _ThemeData
{
	GdkPixbuf* background;
	
	gint width;
	gint height;
	
	gint title_pos_x;
	gint title_pos_y;
	gint title_width;
	gchar* title_font_size;
	gchar *title_font_color;
	
	gint icon_pos_x;
	gint icon_pos_y;
	gint icon_scale;
	
	gint message_pos_x;
	gint message_pos_y;
	gint message_width;
	gint message_height;
	gchar* message_font_size;
	gchar* message_font_color;
}ThemeData;

GtkWidget *gnotify_frontend_gtk_make_message(gchar* title, gchar* body,GdkPixbuf* icon);
void gnotify_frontend_gtk_show_popup(gchar* title,gchar* body,GdkPixbuf* icon);
void gnotify_frontend_gtk_show_dialog(gchar* title,gchar* body,GdkPixbuf* icon);
void gnotify_frontend_gtk_show(GnotifyThreadData *data);
gboolean gnotify_frontend_theme_parser_load(const gchar *filename);

/* GTK end */


#ifdef __cplusplus
}
#endif

#endif /* _GNOTIFY-FRONTEND_H */
