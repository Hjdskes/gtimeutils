/* GStopwatchWindow
 *
 * Copyright (C) 2015 Jente Hidskes <hjdskes@gmail.com>
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "gstopwatch-window.h"
#include "gstopwatch-face.h"

/* Copyright years. */
#define COPYRIGHT "2015"

/* The border inside the window. */
#define BORDER_WIDTH 6

G_DEFINE_TYPE (GStopwatchWindow, gstopwatch_window, GTK_TYPE_APPLICATION_WINDOW);

static void
gstopwatch_window_construct_ui (GStopwatchWindow *window)
{
	GtkWidget *headerbar;
	GtkWidget *gstopwatch_face;

	g_return_if_fail (GSTOPWATCH_IS_WINDOW (window));

	headerbar = gtk_header_bar_new ();
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
	gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), g_get_application_name ());
	gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);

	gstopwatch_face = gstopwatch_face_new ();
	gtk_container_add (GTK_CONTAINER (window), gstopwatch_face);

	gtk_widget_show_all (GTK_WIDGET (window));
}

static GObject *
gstopwatch_window_constructor (GType type,
			       guint n_construct_properties,
			       GObjectConstructParam *construct_params)
{
	GObject *object;

	object = G_OBJECT_CLASS (gstopwatch_window_parent_class)->constructor
		(type, n_construct_properties, construct_params);

	gstopwatch_window_construct_ui (GSTOPWATCH_WINDOW (object));

	return object;
}

static void
gstopwatch_window_class_init (GStopwatchWindowClass *gstopwatch_window_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gstopwatch_window_class);

	object_class->constructor = gstopwatch_window_constructor;
}

static void
gstopwatch_window_init (GStopwatchWindow *window)
{
	gtk_container_set_border_width (GTK_CONTAINER (window), BORDER_WIDTH);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
}

GStopwatchWindow *
gstopwatch_window_new (GStopwatchApplication *application)
{
	g_return_val_if_fail (application != NULL, NULL);
	g_return_val_if_fail (GSTOPWATCH_IS_APPLICATION (application), NULL);
	return g_object_new (GSTOPWATCH_TYPE_WINDOW, "application", application, NULL);
}

void
gstopwatch_window_show_about_dialog (GStopwatchWindow *window)
{
	g_return_if_fail (window != NULL);
	g_return_if_fail (GSTOPWATCH_IS_WINDOW (window));

	static const char *authors[] = {
		"Jente Hidskes <hjdskes@gmail.com>",
		NULL,
	};

	gtk_show_about_dialog (GTK_WINDOW (window),
			       "program-name", g_get_application_name (),
			       "version", PACKAGE_VERSION,
			       "copyright", "Copyright \xc2\xa9 "COPYRIGHT" Jente Hidskes",
			       "comments", _("Count down time"),
			       "authors", authors,
			       "translator-credits", _("translator-credits"),
			       "website-label", _("Website"),
			       "website", PACKAGE_URL,
			       "logo-icon-name", "gnome-clocks",
			       "wrap-license", TRUE,
			       "license-type", GTK_LICENSE_GPL_2_0,
			       NULL);
}

void
gstopwatch_window_close (GStopwatchWindow *window)
{
	g_return_if_fail (window != NULL);
	g_return_if_fail (GSTOPWATCH_IS_WINDOW (window));
	gtk_widget_destroy (GTK_WIDGET (window));
}

