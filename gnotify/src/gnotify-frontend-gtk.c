/***************************************************************************
 *            gnotify-frontend-gtk.c
 *
 *  Fri Apr 29 14:24:19 2005
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

#include "include/gnotify-backend.h"
#include "include/gnotify-frontend.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

GtkWidget *gnotify_frontend_gtk_make_title(gchar* title,gchar* font_size)
{
	GtkWidget* ret;
	gchar* new_title;
	new_title=g_strdup_printf("<span size=\"%s\"><b>%s</b></span>",font_size,title);
	ret=gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(ret),new_title);
	return ret;
}

GtkWidget *gnotify_frontend_gtk_make_body(gchar* body,gchar* font_size)
{
	GtkWidget* ret;
	gchar* new_body;
	new_body=g_strdup_printf("<span size=\"%s\"> %s </span>",font_size,body);
	ret=gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(ret),new_body);
	return ret;
}

GtkWidget *gnotify_frontend_gtk_make_message(
  gchar* title, gchar* body,GdkPixbuf* icon)
{
	GtkWidget* image;
	GtkWidget* lbl_title;
	GtkWidget* lbl_body;
	GtkWidget* hbox;
	GtkWidget* vbox;
	
	image=icon?gtk_image_new_from_pixbuf(icon):NULL;
	if (title)
		lbl_title=gnotify_frontend_gtk_make_title(title,"x-large");
	else lbl_title=NULL;
		
	if (body)
		lbl_body=gnotify_frontend_gtk_make_body(body,"x-large");
	else lbl_body=NULL;
	
	vbox=gtk_vbox_new(0,20);
	gtk_container_set_border_width(GTK_CONTAINER(vbox),20);
	gtk_box_pack_start(GTK_BOX(vbox),lbl_title,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox),lbl_body,1,1,0);
	
	if (!image) return vbox;
	
	hbox=gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(hbox),image,0,0,0);
	gtk_box_pack_start(GTK_BOX(hbox),vbox,1,1,0);
	
	return hbox;
}

GtkWidget *gnotify_frontend_gtk_make_message_themed(
  gchar* title, gchar* body,GdkPixbuf* icon)
{
	GtkWidget *bg;
	GtkWidget* image;
	GtkWidget* background;
	GtkWidget* lbl_title;
	GtkWidget* lbl_body;
	GtkWidget* fixed;
	
	image=icon?gtk_image_new_from_pixbuf(icon):NULL;
	background=ThemeData.background?
		gtk_image_new_from_pixbuf(ThemeData.background):
		NULL;
	
	if (title)
		lbl_title=gnotify_frontend_gtk_make_title(title,ThemeData.title_font_size);
	else lbl_title=NULL;
		
	if (body)
	{
		lbl_body=gnotify_frontend_gtk_make_body(body,ThemeData.message_font_size);
		gtk_label_set_line_wrap(GTK_LABEL(lbl_body),TRUE);
		gtk_widget_set_size_request(lbl_body,ThemeData.message_width,ThemeData.message_height);
	}
	else lbl_body=NULL;
		
	fixed=gtk_fixed_new();

	gtk_widget_set_size_request(fixed,ThemeData.width,ThemeData.height);
	bg=gtk_image_new_from_pixbuf(ThemeData.background);
	gtk_fixed_put(GTK_FIXED(fixed),bg,0,0);
	
	/* FIXME: scale image if needed */
	gtk_fixed_put(GTK_FIXED(fixed),image,
	  ThemeData.icon_pos_x,
	  ThemeData.icon_pos_y);
	
	gtk_fixed_put(GTK_FIXED(fixed),lbl_title,
	  ThemeData.title_pos_x,
	  ThemeData.title_pos_y);
	
	gtk_fixed_put(GTK_FIXED(fixed),lbl_body,
	  ThemeData.message_pos_x,
	  ThemeData.message_pos_y);
	
	
	return fixed;
}

int get_zero()
{
	gtk_main_quit();
	return 0;
}

void gnotify_frontend_gtk_show_popup(gchar* title,gchar* body,GdkPixbuf* icon)
{
	GtkWidget* window;
	GtkWidget* message;
	
	window=gtk_window_new(GTK_WINDOW_POPUP);
	message=gnotify_frontend_gtk_make_message(title,body,icon);
	gtk_container_add(GTK_CONTAINER(window),message);
	
	gtk_widget_show_all(message);
	gtk_widget_realize(window);
	
	gtk_window_move(GTK_WINDOW(window),
	  (gdk_screen_width()-window->allocation.width)*
		((GnotifyConfig.align_x>=0)?(GnotifyConfig.align_x<=100)?
			GnotifyConfig.align_x:100:0)/100.0,
	  (gdk_screen_height()-window->allocation.height)*
		((GnotifyConfig.align_y>=0)?(GnotifyConfig.align_y<=100)?
			GnotifyConfig.align_y:100:0)/100.0
	);
	
	gtk_widget_show_all(window);
	
	g_timeout_add(GnotifyConfig.popup_timeout,get_zero,NULL);
	gtk_main();
	/* FIXME: some sleep function... */
}

void gnotify_frontend_gtk_show_dialog(gchar* title,gchar* body,GdkPixbuf* icon)
{
	GtkWidget* dialog;
	GtkWidget* message;
	gint w,h;
	
	/* create new dialog */
	dialog=gtk_dialog_new_with_buttons(title,NULL,0,
	  GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	message=gnotify_frontend_gtk_make_message(NULL,body,icon);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),message,1,1,10);
	gtk_widget_show_all(message);
	gtk_widget_realize(dialog);
	w=dialog->allocation.width;
	h=dialog->allocation.height;
	/* move window to the right position */
	gtk_window_move(GTK_WINDOW(dialog),
	  (gdk_screen_width()-w)*
		((GnotifyConfig.align_x>=0)?(GnotifyConfig.align_x<=100)?
			GnotifyConfig.align_x:100:0)/100.0,
	  (gdk_screen_height()-h)*
		((GnotifyConfig.align_y>=0)?(GnotifyConfig.align_y<=100)?
			GnotifyConfig.align_y:100:0)/100.0
	);
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void gnotify_frontend_gtk_show(GnotifyThreadData *data)
{
	switch(GnotifyConfig.display_mode)
	{
		case GNOTIFY_FRONTEND_GTK_MODE_DIALOG:
			gnotify_frontend_gtk_show_dialog(data->title,
				data->message,data->icon);
		break;
		case GNOTIFY_FRONTEND_GTK_MODE_POPUP:
		default:
			gnotify_frontend_gtk_show_popup(data->title,
				data->message,data->icon);
		break;
	};
	/* FIXME: stop thread */
}
