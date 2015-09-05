/* GStopwatchRadius
 *
 * GStopwatchRadius is a port of Gnome Clock's Stopwatch.Frame.
 *
 * Copyright (C) 2013 Paolo Borelly <pborelli@gnome.org>
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

/**
 * SECTION:GStopwatchRadius
 * @Short_description: A subclass of #Radius, specifically to show an endurance progress.
 * @Title: GStopwatchRadius
 *
 * The #GStopwatchRadius widget is a subclass of the #Radius widget. The #Radius widget is an
 * abstract class; it must be extended to define #Radius::draw_progress(). The #GStopwatchRadius
 * widget implements draw_progress() in such a way that the total endurance of an activity can be
 * monitored.
 */

#include <math.h>
#include <gtk/gtk.h>

#include "../radius.h"
#include "gstopwatch-radius.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#define LINE_WIDTH 6

struct _GStopwatchRadiusPrivate {
	unsigned int seconds;
	double milliseconds;
};

G_DEFINE_TYPE_WITH_PRIVATE (GStopwatchRadius, gstopwatch_radius, RADIUS_TYPE);

static void
gstopwatch_radius_draw_progress (GtkWidget *widget,
				 cairo_t *cr,
				 int center_x,
				 int center_y,
				 int radius)
{
	GStopwatchRadiusPrivate *priv;
	GtkStyleContext *context;
	GdkRGBA color = { 0 };
	double progress;

	priv = gstopwatch_radius_get_instance_private (GSTOPWATCH_RADIUS (widget));

	context = gtk_widget_get_style_context (widget);

	gtk_style_context_save (context);
	gtk_style_context_add_class (context, "progress");

	cairo_set_line_width (cr, LINE_WIDTH);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

	gtk_style_context_get_color (context, GTK_STATE_FLAG_NORMAL, &color);

	progress = ((double) priv->seconds + priv->milliseconds) / 60;
	if (progress > 0) {
		cairo_arc (cr, center_x, center_y, radius - LINE_WIDTH / 2, 1.5 * M_PI,
			   (1.5 + progress * 2) * M_PI);
		cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
		cairo_stroke (cr);
	}

	gtk_style_context_restore (context);

	gtk_style_context_save (context);
	gtk_style_context_add_class (context, "progress-fast");

	cairo_set_line_width (cr, LINE_WIDTH - 2);
	gtk_style_context_get_color (context, GTK_STATE_FLAG_NORMAL, &color);

	progress = priv->milliseconds;
	if (progress > 0) {
		cairo_arc (cr, center_x, center_y, radius - LINE_WIDTH / 2,
			   (1.5 + progress * 2 ) * M_PI - 0.1,
			   (1.5 + progress * 2 ) * M_PI + 0.1);
		cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
		cairo_stroke (cr);
	}

	gtk_style_context_restore (context);
}

static void
gstopwatch_radius_class_init (GStopwatchRadiusClass *gstopwatch_radius_class)
{
	RadiusClass *radius_class = RADIUS_CLASS (gstopwatch_radius_class);

	radius_class->draw_progress = gstopwatch_radius_draw_progress;
}

static void
gstopwatch_radius_init (GStopwatchRadius *radius)
{
	gstopwatch_radius_update (radius, 0, 0);
}

/**
 * gstopwatch_radius_new:
 * @child: (allow-none): The #GtkWidget to contain inside the #GStopwatchRadius.
 *
 * Creates a new #GStopwatchRadius containing the #GtkWidget. You can pass %NULL to get an empty
 * #GStopwatchRadius widget.
 *
 * Returns: The newly created #GStopwatchRadius widget.
 */
GStopwatchRadius *
gstopwatch_radius_new (GtkWidget *child)
{
	GStopwatchRadius *radius;

	radius = g_object_new (GSTOPWATCH_RADIUS_TYPE, NULL);
	if (child) {
		gtk_container_add (GTK_CONTAINER (radius), child);
	}
	return radius;
}

/**
 * gstopwatch_radius_reset:
 * @radius: The #GStopwatchRadius to reset.
 *
 * Resets the #GStopwatchRadius. This sets the elapsed time to zero.
 */
void
gstopwatch_radius_reset (GStopwatchRadius *radius)
{
	gstopwatch_radius_update (radius, 0, 0);
}

/**
 * gstopwatch_radius_update:
 * @radius: The #GStopwatchRadius to update.
 * @seconds: The elapsed seconds.
 * @milliseconds: The elapsed milliseconds.
 *
 * Updates the elapsed time of the #GStopwatchRadius widget. This will cause a call
 * to gtk_widget_queue_draw() to visually update the #GStopwatchRadius widget.
 */
void
gstopwatch_radius_update (GStopwatchRadius *radius, unsigned int seconds, double milliseconds)
{
	GStopwatchRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	g_return_if_fail (GSTOPWATCH_IS_RADIUS (radius));
	g_return_if_fail (milliseconds >= 0);
	priv = gstopwatch_radius_get_instance_private (radius);

	priv->seconds = seconds;
	priv->milliseconds = milliseconds;
	gtk_widget_queue_draw (GTK_WIDGET (radius));
}

