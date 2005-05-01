/***************************************************************************
 *            gnotify-backend-dbus.c
 *
 *  Fri Apr 29 14:23:38 2005
 *  Copyright  2005  Marius M. M.
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
 
#include "include/gnotify-backend.h"
#include "include/gnotify-frontend.h"
#include "include/base64.h"

enum
{
	MSG_TITLE=1,
	MSG_ICON,
	MSG_SND,
	MSG_MESSAGE,
	MSG_END
};

DBusServer *gnotify_backend_dbus_init(const gchar *address)
{
	DBusServer *server;
	DBusError error;
	
	dbus_g_thread_init ();
	dbus_error_init (&error);
	
	server = dbus_server_listen(address, &error);
	
	if(server == NULL)
	{
		g_warning("Couldn't start D-BUS backend on %s: %s", address, error.message);
		dbus_error_free(&error);
		return(NULL);
	}
	
	dbus_server_set_new_connection_function(server, gnotify_backend_dbus_connection, NULL, NULL);
	
	dbus_server_setup_with_g_main(server, NULL);
	
	return(server);
}

void gnotify_backend_dbus_connection(DBusServer *server, DBusConnection *con, gpointer user_data)
{
	GnotifyThreadData *data;
	
	dbus_connection_ref(con);
	dbus_connection_setup_with_g_main(con,NULL);
	
	data = gnotify_backend_threaddata_new();
	
	if(!dbus_connection_add_filter(con, gnotify_backend_dbus_message_filter, data, (DBusFreeFunction) gnotify_backend_threaddata_free) &&
		!dbus_connection_add_filter(con,gnotify_backend_dbus_message_filter_disconnect,NULL,NULL))
	{
		g_error("No memory to set up new connection!");
	}
}

DBusHandlerResult gnotify_backend_dbus_message_filter(DBusConnection *con, DBusMessage *msg, gpointer user_data)
{
	GnotifyThreadData *data = user_data;
	DBusMessageIter dbmi;
	GError *err;
	guchar *tmp;
	gchar *tmp2;
	gint i;

	if(!dbus_message_is_method_call(msg, "org.freedesktop.gnotify","Notification"))
	return(DBUS_HANDLER_RESULT_NOT_YET_HANDLED);
  
	dbus_message_iter_init(msg, &dbmi);

	/* Now the strings stuff begin */
	/* A message should look like this: */
	/* [title] [icon] [sound] [message] */
	
	for(i=MSG_TITLE; i<MSG_END;i++)
	{
		if(!dbus_message_iter_next(&dbmi))
		{
			g_print ("Couldn't get arg #%i\n",i);
			return DBUS_HANDLER_RESULT_HANDLED;
		}

		if(dbus_message_iter_get_arg_type (&dbmi)!=DBUS_TYPE_STRING)
		{
			g_print ("Arg #%i not right type\n",i);
			return DBUS_HANDLER_RESULT_HANDLED;
		}

		switch(i)
		{
			case MSG_TITLE: dbus_message_iter_get_basic (&dbmi, &data->title); break;
			case MSG_ICON: dbus_message_iter_get_basic (&dbmi, &tmp); 
							decode_base64(tmp2,tmp);
							g_free(tmp);
							data->icon=gdk_pixbuf_new_from_data(tmp2, GDK_COLORSPACE_RGB, TRUE, 8, 48, 48, 0, gnotify_backend_threaddata_free_pixbuf, NULL);
				break;
			case MSG_SND: dbus_message_iter_get_basic (&dbmi, &data->sound); break;
			case MSG_MESSAGE: dbus_message_iter_get_basic (&dbmi, &data->message); break;
			default: g_error("Wtf? i got too big, check the code!");
		}
	}
		
	if (dbus_message_iter_next (&dbmi))
    {
		 g_print ("Extra args on end of message\n");
    }
  
	dbus_connection_flush(con);

	/* Play sound */
	if((data->SoundThread=g_thread_create((GThreadFunc)gnotify_backend_gst_play,(gpointer *)data->sound, TRUE, &err)) == NULL)
	{
		g_error("Couldn't create Sound-Thread: %s", err->message);
		g_error_free(err);
	};  
	/* Display Notification */
	gnotify_frontend_gtk_show(data);
	
	g_free(data);	
	return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult gnotify_backend_dbus_message_filter_disconnect(DBusConnection *con, DBusMessage *msg, gpointer user_data)
{
	if(!dbus_message_is_signal(msg, DBUS_INTERFACE_LOCAL, "Disconnected"))
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	
	g_print ("Connection disconnected\n");
	
	dbus_connection_unref (con);
  
	return DBUS_HANDLER_RESULT_HANDLED;
}

GnotifyThreadData *gnotify_backend_threaddata_new(void)
{
	GnotifyThreadData *data;
	data = g_new0(GnotifyThreadData, 1);
	return(data);
}

void gnotify_backend_threaddata_free(GnotifyThreadData *data)
{
	g_free(data);
	return;
}

void gnotify_backend_threaddata_free_pixbuf(guchar *pixels, gpointer data)
{
	g_object_unref(G_OBJECT(pixels));
}
