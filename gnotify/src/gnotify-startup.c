/***************************************************************************
 *            gnotify-startup.c
 *
 *  Fri Apr 29 14:27:14 2005
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
#include <signal.h>

gint main(gint argc, gchar *argv[])
{
	gtk_init(&argc,&argv);

	signal(SIGQUIT,signalhandler);
	signal(SIGKILL,signalhandler);
	signal(SIGTERM,signalhandler);
	signal(SIGSTOP,signalhandler);

	gnotify_backend_gconf_load();
	/*g_thread_init (NULL);*/
	gnotify_backend_dbus_init(GnotifyConfig.dbus_address);
	gnotify_backend_logdog_init();
	gtk_main();
	return(0);
}

void end(void)
{
	gnotify_backend_logdog_exit();
	gtk_main_quit();
	return;
}

void signalhandler(gint sign)
{
	switch(sign)
	{
		case SIGQUIT:
		case SIGKILL:
		case SIGTERM:
		case SIGSTOP: end(); break;
	}
	return;
}
