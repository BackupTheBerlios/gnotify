/***************************************************************************
 *            gnotify-backend-gconf.c
 *
 *  Fri Apr 29 14:25:17 2005
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

gint gnotify_backend_gconf_load(void)
{
	GConfClient *gc=GnotifyConfig.config;
	
	gc=gconf_client_get_default();


	GnotifyConfig.display_mode=(gint)gconf_client_get_int(gc,GCONF_PATH "/display_mode",NULL);
	GnotifyConfig.align_x=(gint)gconf_client_get_int(gc,GCONF_PATH "/align_x",NULL);
	GnotifyConfig.align_y=(gint)gconf_client_get_int(gc,GCONF_PATH "/align_y",NULL);
	GnotifyConfig.popup_timeout=(gint)gconf_client_get_int(gc,GCONF_PATH "/popup_timeout",NULL);
	GnotifyConfig.enable_sound=(gboolean)gconf_client_get_bool(gc,GCONF_PATH "/enable_sound",NULL);
	GnotifyConfig.gst_audiosink=(gchar*)gconf_client_get_string(gc,"/system/default/audiosink",NULL);
	GnotifyConfig.dbus_address=(gchar*)gconf_client_get_string(gc,GCONF_PATH "/dbus_address",NULL);
	GnotifyConfig.enable_logging=(gboolean)gconf_client_get_bool(gc,GCONF_PATH "/enable_logging",NULL);
	
	GnotifyConfig.logfile=g_strdup_printf("%s/%s",g_getenv("HOME"),LOGFILE_NAME);

	if(gconf_client_dir_exists(gc,GCONF_PATH,NULL)==FALSE)
	{
		g_warning("Directory does not exist!\n");
	}
	
	gconf_client_add_dir(gc,GCONF_PATH,GCONF_CLIENT_PRELOAD_NONE,NULL);
	
	/* gconf_client_notify_add(gc,GnotifyConfig.display_mode,gnotify_backend_gconf_notify,NULL,NULL,NULL);
	gconf_client_notify_add(gc,GnotifyConfig.enable_sound,gnotify_backend_gconf_notify,NULL,NULL,NULL);
	gconf_client_notify_add(gc,GnotifyConfig.gst_audiosink,gnotify_backend_gconf_notify,NULL,NULL,NULL);
	gconf_client_notify_add(gc,GnotifyConfig.dbus_address,gnotify_backend_gconf_notify,NULL,NULL,NULL); */
	
	return(0);
}

/*void gnotify_backend_gconf_notify(GConfClient *gc, guint cnxn_id, GConfEntry *entry, gpointer user_data)
{
	GConfValue  *value;

	value=gconf_entry_get_value(entry);
	
	return;	
}*/
