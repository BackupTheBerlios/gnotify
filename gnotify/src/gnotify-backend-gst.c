/***************************************************************************
 *            gnotify-backend-gst.c
 *
 *  Fri Apr 29 14:24:48 2005
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

/* This code, I've got from the gstreamer docs,
 * I just modified it a bit so that it fits the needs
 * of a GNotify-Soundplay function. */
gboolean gnotify_backend_gst_play(gpointer sfile)
{
	GstElement *pipeline, *filesrc, *decoder, *audiosink;
	
	pipeline = gst_pipeline_new ("pipeline");
	filesrc = gst_element_factory_make ("filesrc", "disk_source");
	g_object_set (G_OBJECT (filesrc), "location", (gchar*)sfile, NULL);
	decoder = gst_element_factory_make ("spider", "decoder");
	audiosink = gst_element_factory_make((const gchar*)GnotifyConfig.gst_audiosink, "play_audio");
	gst_bin_add_many (GST_BIN (pipeline), filesrc, decoder, audiosink, NULL);
	gst_element_link_many (filesrc, decoder, audiosink, NULL);
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
	while (gst_bin_iterate (GST_BIN (pipeline)));
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (pipeline));
	return(FALSE);
}
