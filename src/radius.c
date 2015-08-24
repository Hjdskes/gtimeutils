/* Radius
 *
 * Radius is a port of Gnome Clock's AnalogFrame.
 *
 * Copyright (C) 2013  Paolo Borelli <pborelli@gnome.org>
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
 * SECTION:Radius
 * @Short_description: An abstract base class for a circular widget that can contain other widgets,
 * while showing some sort of progress.
 * @Title: Radius
 *
 * The #Radius widget is a circular widget, used to display progress of some sort. It can contain
 * other widgets. Usually, the #Radius and its child are related, in that the progress displayed in
 * the #Radius is set or controlled through the child widget.
 *
 * Because there are many types of progress, the #Radius widget is an abstract class that does not
 * define how the progress is displayed. Rather, it should be subclassed, with subclasses
 * implementing their own draw_progress().
 *
 * The #Radius widget can hold any valid child widget. That is, it can hold almost any other
 * standard #GtkWidget.
 */

#include <math.h>
#include <gtk/gtk.h>

#include "radius.h"

G_DEFINE_ABSTRACT_TYPE (Radius, radius, GTK_TYPE_FRAME);

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#define LINE_WIDTH 6
#define RADIUS_PAD 48

static int
calculate_diameter (Radius *radius)
{
	GtkWidget *child = NULL;
	int diameter = 2 * RADIUS_PAD;

	child = gtk_bin_get_child (GTK_BIN (radius));
	if (child && gtk_widget_is_visible (child)) {
		int width, height;

		gtk_widget_get_preferred_width (child, &width, NULL);
		gtk_widget_get_preferred_height (child, &height, NULL);
		diameter += sqrt (width * width + height * height);
	}

	return diameter;
}

static void
radius_get_preferred_size (Radius *radius, int *minimal, int *natural)
{
	int diameter;

	diameter = calculate_diameter (radius);
	*minimal = diameter;
	*natural = diameter;
}

static void
radius_get_preferred_width (GtkWidget *widget, int *minimal, int *natural)
{
	radius_get_preferred_size (RADIUS (widget), minimal, natural);
}

static void
radius_get_preferred_height (GtkWidget *widget, int *minimal, int *natural)
{
	radius_get_preferred_size (RADIUS (widget), minimal, natural);
}

static void
radius_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	GtkWidget *child = NULL;

	gtk_widget_set_allocation (widget, allocation);
	child = gtk_bin_get_child (GTK_BIN (widget));
	if (child && gtk_widget_is_visible (child)) {
		GtkAllocation child_allocation = { 0 };
		int width, height;

		gtk_widget_get_preferred_width (child, &width, NULL);
		gtk_widget_get_preferred_height (child, &height, NULL);

		child_allocation.x = allocation->x + (allocation->width - width) / 2;
		child_allocation.y = allocation->y + (allocation->height - height) / 2;
		child_allocation.width = width;
		child_allocation.height = height;
		gtk_widget_size_allocate (child, &child_allocation);
	}
}

static gboolean
radius_draw (GtkWidget *widget, cairo_t *cr)
{
	GtkStyleContext *context;
	GtkAllocation allocation = { 0 };
	GdkRGBA color = { 0 };
	int radius;
	int center_x, center_y;

	context = gtk_widget_get_style_context (widget);
	gtk_widget_get_allocation (widget, &allocation);
	center_x = allocation.width / 2;
	center_y = allocation.height / 2;

	radius = calculate_diameter (RADIUS (widget)) / 2;

	cairo_save (cr);
	cairo_move_to (cr, center_x + radius, center_y);

	gtk_style_context_save (context);
	gtk_style_context_add_class (context, "clocks-analog-frame");

	gtk_style_context_save (context);
	gtk_style_context_add_class (context, GTK_STYLE_CLASS_TROUGH);

	gtk_style_context_get_color (context, GTK_STATE_FLAG_NORMAL, &color);

	cairo_set_line_width (cr, LINE_WIDTH);
	cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
	cairo_arc (cr, center_x, center_y, radius - LINE_WIDTH / 2, 0, 2 * M_PI);
	cairo_stroke (cr);

	gtk_style_context_restore (context);

	RADIUS_GET_CLASS (widget)->draw_progress (widget, cr, center_x, center_y, radius);

	gtk_style_context_restore (context);
	cairo_restore (cr);

	return GTK_WIDGET_CLASS (radius_parent_class)->draw (widget, cr);
}

static void
radius_draw_progress (GtkWidget *widget, cairo_t *cr, int center_x, int center_y, int radius)
{
}

static void
radius_class_init (RadiusClass *radius_class)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (radius_class);

	widget_class->get_preferred_width = radius_get_preferred_width;
	widget_class->get_preferred_height = radius_get_preferred_height;
	widget_class->size_allocate = radius_size_allocate;
	widget_class->draw = radius_draw;

	radius_class->draw_progress = radius_draw_progress;
}

static void
radius_init (Radius *radius)
{
}

