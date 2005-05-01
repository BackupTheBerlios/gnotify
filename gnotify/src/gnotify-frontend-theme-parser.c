/***************************************************************************
 *            gnotify-frontend-theme-parser.c
 *
 *  Sat Apr 30 17:16:39 2005
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
 
#include "include/gnotify-frontend.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define THEME_ENGINE "2.0"

gboolean gnotify_frontend_theme_parser_load(const gchar *filename)
{
	xmlDocPtr xmldoc=NULL;
	xmlNodePtr cur=NULL;
	gchar *foo;

	if(filename==NULL)
		return(FALSE);
	
	if((xmldoc=xmlParseFile(filename))==NULL)
	{
		g_warning("Couldn't parse Theme!");
		return(FALSE);
	}
	if((cur=xmlDocGetRootElement(xmldoc))==NULL)
	{
		g_warning("Couldn't parse Theme: empty!");
		xmlFreeDoc(xmldoc);
		return(FALSE);
	}
	if(xmlStrcmp(cur->name,(const xmlChar *)"gnotify-theme"))
	{
		g_warning("This doesn't seem to be a Gnotify Theme!");
		xmlFreeDoc(xmldoc);
		return(FALSE);
	}
	if(strcmp((foo=xmlGetProp(cur,(const xmlChar *)"engine-version")),THEME_ENGINE)!=0)
	{
		g_warning("The Theme is made for another Theme-Engine (Version: %s) than yours!",foo);
		xmlFreeDoc(xmldoc);
		return(FALSE);
	}
	cur=cur->xmlChildrenNode;
	while(cur!=NULL)
	{
		/* ... */
		if((!xmlStrcmp(cur->name,(const xmlChar *)"window")))
		{
			ThemeData.width=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"width"),NULL);
			ThemeData.height=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"height"),NULL);
			ThemeData.background=gdk_pixbuf_new_from_data(xmlGetProp(cur,(const xmlChar *)"background"),GDK_COLORSPACE_RGB, TRUE, 8, ThemeData.width, ThemeData.height, 0, NULL, NULL);
		}
		else if((!xmlStrcmp(cur->name,(const xmlChar *)"icon")))
		{
			ThemeData.icon_scale=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"scale"),NULL);
			ThemeData.icon_pos_x=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"x"),NULL);
			ThemeData.icon_pos_y=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"y"),NULL);
		}
		else if((!xmlStrcmp(cur->name,(const xmlChar *)"title")))
		{
			ThemeData.title_width=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"width"),NULL);
			ThemeData.title_pos_x=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"x"),NULL);
			ThemeData.title_pos_y=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"y"),NULL);
			ThemeData.title_font_size=xmlGetProp(cur,(const xmlChar *)"font-size");
			ThemeData.title_font_color=xmlGetProp(cur,(const xmlChar *)"font-color");
		}
		else if((!xmlStrcmp(cur->name,(const xmlChar *)"message")))
		{
			ThemeData.message_width=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"width"),NULL);
			ThemeData.message_height=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"width"),NULL);
			ThemeData.message_pos_x=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"x"),NULL);
			ThemeData.message_pos_y=g_ascii_strtod(xmlGetProp(cur,(const xmlChar *)"y"),NULL);
			ThemeData.message_font_size=xmlGetProp(cur,(const xmlChar *)"font-size");
			ThemeData.message_font_color=xmlGetProp(cur,(const xmlChar *)"font-color");
		}
		cur=cur->next;
	}
	xmlFreeDoc(xmldoc);
	return(TRUE);
}
