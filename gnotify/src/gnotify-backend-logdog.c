/***************************************************************************
 *            gnotify-backend-logdog.c
 *
 *  Fri Apr 29 19:31:30 2005
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
#include <stdio.h>
#include <time.h>

gboolean gnotify_backend_logdog_init(void)
{
	if(GnotifyConfig.enable_logging==FALSE)
		return(TRUE);

	if((GnotifyConfig.logf=fopen(GnotifyConfig.logfile,"a"))==NULL)
	{
		g_warning("Couldn't open logfile '%s' for writing!",GnotifyConfig.logfile);
		return(FALSE);
	}
	return(TRUE);
}

void gnotify_backend_logdog_print(const gchar *format, ...)
{
	va_list ap;
	gchar output[512]; /* more than 512 bytes in one line should not be */
	
	if(GnotifyConfig.enable_logging==FALSE)
		return;

	va_start(ap, format);
    g_vsnprintf(output,511,format,ap);
	va_end(ap);
	fprintf(GnotifyConfig.logf,"%s: %s\n",gnotify_backend_logdog_gtd("%I:%M:%S\t%F"), output);
	return;
}

void gnotify_backend_logdog_exit(void)
{
	fclose(GnotifyConfig.logf);
	return;
}

gchar *gnotify_backend_logdog_gtd(const gchar* format)
{
	static gchar buff[64];
	time_t curtime;
	struct tm *loctime;

	curtime=time(NULL);
	loctime=localtime(&curtime);
	strftime(buff,63,format,loctime);
	return(buff);
}
