/* GTimerRadius
 *
 * GTimerRadius is a port of Gnome Clock's CountDownFrame.
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
 * SECTION:GTimerRadius
 * @Short_description: A subclass of #Radius, specifically to show a countdown progess.
 * @Title: GTimerRadius
 *
 * The #GTimerRadius widget is a subclass of the #Radius widget. The #Radius widget is an abstract
 * class; it must be extended to define #Radius::draw_progress(). The #GTimerRadius widget
 * implements draw_progress() in such a way that a countdown is displayed.
 */

#include <math.h>
#include <gtk/gtk.h>

#include "../radius.h"
#include "gtimer-radius.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#define LINE_WIDTH 6

struct _GTimerRadiusPrivate {
	double span;
	double elapsed_before_pause;
	double elapsed;
};

G_DEFINE_TYPE_WITH_PRIVATE (GTimerRadius, gtimer_radius, RADIUS_TYPE);

static double
get_progress (GTimerRadius *radius)
{
	GTimerRadiusPrivate *priv;

	priv = gtimer_radius_get_instance_private (radius);

	return priv->span != 0 ? (priv->elapsed_before_pause + priv->elapsed) / priv->span : 0;
}

static void
gtimer_radius_draw_progress (GtkWidget *widget,
			    cairo_t *cr,
			    int center_x,
			    int center_y,
			    int radius)
{
	GtkStyleContext *context;
	GdkRGBA color = { 0 };
	double progress;

	context = gtk_widget_get_style_context (widget);

	gtk_style_context_save (context);
	gtk_style_context_add_class (context, "progress");

	cairo_set_line_width (cr, LINE_WIDTH);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

	gtk_style_context_get_color (context, GTK_STATE_FLAG_NORMAL, &color);

	progress = get_progress (GTIMER_RADIUS (widget));
	cairo_arc (cr, center_x, center_y, radius - LINE_WIDTH / 2, 1.5  * M_PI,
		   (1.5 + (1 - progress) * 2 ) * M_PI);
	cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
	cairo_stroke (cr);

	gtk_style_context_restore (context);
}

static void
gtimer_radius_class_init (GTimerRadiusClass *gtimer_radius_class)
{
	RadiusClass *radius_class = RADIUS_CLASS (gtimer_radius_class);

	radius_class->draw_progress = gtimer_radius_draw_progress;
}

static void
gtimer_radius_init (GTimerRadius *radius)
{
	GTimerRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	priv = gtimer_radius_get_instance_private (radius);

	priv->span = 0;
	gtimer_radius_reset (radius);
}

/**
 * gtimer_radius_new:
 * @child: (allow-none): The #GtkWidget to contain inside the #GTimerRadius.
 *
 * Creates a new #GTimerRadius containing the #GtkWidget. You can pass %NULL to get an empty
 * #GTimerRadius widget.
 *
 * Returns: The newly created #GTimerRadius widget.
 */
GTimerRadius *
gtimer_radius_new (GtkWidget *child)
{
	GTimerRadius *radius;

	radius = g_object_new (GTIMER_RADIUS_TYPE, NULL);
	if (child) {
		gtk_container_add (GTK_CONTAINER (radius), child);
	}
	return radius;
}

/**
 * gtimer_radius_set_span:
 * @radius: The #GTimerRadius whose span to set.
 * @span: The (positive) span to set.
 *
 * Sets the span (the total time to count down from) of the #GTimerRadius.
 */
void
gtimer_radius_set_span (GTimerRadius *radius, double span)
{
	GTimerRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	g_return_if_fail (GTIMER_IS_RADIUS (radius));
	g_return_if_fail (span >= 0);
	priv = gtimer_radius_get_instance_private (radius);

	priv->span = span;
}

/**
 * gtimer_radius_reset:
 * @radius: The #GTimerRadius to reset.
 *
 * Resets the #GTimerRadius. This sets its span and elapsed time to zero.
 */
void
gtimer_radius_reset (GTimerRadius *radius)
{
	GTimerRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	g_return_if_fail (GTIMER_IS_RADIUS (radius));
	priv = gtimer_radius_get_instance_private (radius);

	priv->elapsed_before_pause = 0;
	priv->elapsed = 0;
}

/**
 * gtimer_radius_pause:
 * @radius: The #GTimerRadius to pause.
 *
 * Pauses the #GTimerRadius widget's countdown.
 */
void
gtimer_radius_pause (GTimerRadius *radius)
{
	GTimerRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	g_return_if_fail (GTIMER_IS_RADIUS (radius));
	priv = gtimer_radius_get_instance_private (radius);

	priv->elapsed_before_pause += priv->elapsed;
	priv->elapsed = 0;
}

/**
 * gtimer_radius_update:
 * @radius: The #GTimerRadius to update.
 * @elapsed: The total (positive) elapsed time.
 *
 * Updates the elapsed time of the #GTimerRadius widget. This will cause a call
 * to gtk_widget_queue_draw() to visually update the #GTimerRadius widget.
 */
void
gtimer_radius_update (GTimerRadius *radius, double elapsed)
{
	GTimerRadiusPrivate *priv;

	g_return_if_fail (radius != NULL);
	g_return_if_fail (GTIMER_IS_RADIUS (radius));
	g_return_if_fail (elapsed >= 0);
	priv = gtimer_radius_get_instance_private (radius);

	priv->elapsed = elapsed;
	gtk_widget_queue_draw (GTK_WIDGET (radius));
}

