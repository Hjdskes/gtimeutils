/* GStopwatchFace
 *
 * GStopwatchFace is a port of Gnome Clock's Stopwatch.Face.
 *
 * Copyright (C) 2013 Paolo Borelli <pborelli@gnome.org>
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

#include <math.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "../utils.h"
#include "gstopwatch-face.h"
#include "gstopwatch-radius.h"
#include "laps-row.h"

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

typedef enum State {
	RESET,
	RUNNING,
	STOPPED
} State;

typedef enum LapsColumn {
	LAP,
	SPLIT,
	TOTAL
} LapsColumn;

struct _GStopwatchFacePrivate {
	State state;
	guint tick_id;
	unsigned int current_lap;
	double last_lap_time;
	GTimer *timer;

	GStopwatchRadius *analog_frame;
	GtkWidget *time_label;
	GtkWidget *left_button;
	GtkWidget *right_button;
	GtkWidget *laps_scrollwin;
	GtkWidget *laps_list;
};

G_DEFINE_TYPE_WITH_PRIVATE (GStopwatchFace, gstopwatch_face, GTK_TYPE_BOX);

static void gstopwatch_face_lap (GStopwatchFace *gstopwatch_face);
static void gstopwatch_face_reset (GStopwatchFace *gstopwatch_face);
static void gstopwatch_face_stop (GStopwatchFace *gstopwatch_face);
static void gstopwatch_face_start (GStopwatchFace *gstopwatch_face);

static gboolean
update_time_label (GtkWidget *widget, GdkFrameClock *frame_clock, gpointer user_data)
{
	GStopwatchFacePrivate *priv;
	int hours = 0, minutes = 0, seconds = 0;
	double remainder = 0;
	gchar *text = NULL;

	priv = gstopwatch_face_get_instance_private (GSTOPWATCH_FACE (widget));

	if (priv->state != RESET) {
		double elapsed = g_timer_elapsed (priv->timer, NULL);
		time_to_hms (elapsed, &hours, &minutes, &seconds, &remainder);
	}

	int ds = remainder * 10;

	/* Note that the format uses unicode RATIO character.
	 * We also prepend the LTR mark to make sure text is always in this direction. */
	if (hours > 0) {
		text = g_strdup_printf ("%i\u200E:%02i\u200E:%02i.%i", hours, minutes, seconds, ds);
	} else {
		text = g_strdup_printf ("%02i\u200E∶%02i.%i", minutes, seconds, ds);
	}

	gtk_label_set_text (GTK_LABEL (priv->time_label), text);
	g_free (text);

	gstopwatch_radius_update (priv->analog_frame, seconds, remainder);

	return TRUE;
}

static void
add_tick (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	if (priv->tick_id == 0) {
		priv->tick_id = gtk_widget_add_tick_callback (GTK_WIDGET (gstopwatch_face),
							      update_time_label,
							      NULL,
							      NULL);
	}
}

static void
remove_tick (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	if (priv->tick_id != 0) {
		gtk_widget_remove_tick_callback (GTK_WIDGET (gstopwatch_face), priv->tick_id);
		priv->tick_id = 0;
	}
}

static void
on_left_button_clicked (GtkButton *button, gpointer user_data)
{
	GStopwatchFace *gstopwatch_face;
	GStopwatchFacePrivate *priv;

	g_return_if_fail (GSTOPWATCH_IS_FACE (user_data));
	gstopwatch_face = GSTOPWATCH_FACE (user_data);
	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	switch (priv->state) {
		case RESET:
		case STOPPED:
			gstopwatch_face_start (gstopwatch_face);
			break;
		case RUNNING:
			gstopwatch_face_stop (gstopwatch_face);
			break;
		default:
			g_assert_not_reached ();
	}
}

static void
on_right_button_clicked (GtkButton *button, gpointer user_data)
{
	GStopwatchFace *gstopwatch_face;
	GStopwatchFacePrivate *priv;

	g_return_if_fail (GSTOPWATCH_IS_FACE (user_data));
	gstopwatch_face = GSTOPWATCH_FACE (user_data);
	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	switch (priv->state) {
		case STOPPED:
			gstopwatch_face_reset (gstopwatch_face);
			break;
		case RUNNING:
			gstopwatch_face_lap (gstopwatch_face);
			break;
		default:
			g_assert_not_reached ();
	}
}

// FIXME: cleanup
static void
gstopwatch_face_lap (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;
	GtkWidget *row;
	double elapsed, split, remaining;
	int hours, minutes, seconds;
	int split_hours, split_minutes, split_seconds;
	gchar *n_label, *split_label, *tot_label;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	priv->current_lap += 1;
	elapsed = g_timer_elapsed (priv->timer, NULL);
	split = elapsed - priv->last_lap_time;

	/* Discard milliseconds in the saved lap time to ensure
	 * total and split times are consistent: for instance if we saved
	 * 0.108000 and the next lap is 1.202000, we would see on screen 0.10
	 * and 1.20, so we would expect a split time of 1.10, but we would
	 * instead get 1.094000 and thus display 1.09. */
	priv->last_lap_time = floor (elapsed * 100) / 100;

	time_to_hms (elapsed, &hours, &minutes, &seconds, &remaining);
	int cs = (int) (remaining * 100);

	time_to_hms (split, &split_hours, &split_minutes, &split_seconds, &remaining);
	int split_cs = (int) (remaining * 100);

	/* Note that the format uses unicode RATIO character.
	 * We also prepend the LTR mark to make sure text is always in this direction. */
	if (split_hours > 0) {
	    split_label = g_strdup_printf ("%i\u200E∶%02i\u200E∶%02i.%02i",
					   split_hours, split_minutes, split_seconds, split_cs);
	} else {
	    split_label = g_strdup_printf ("%02i\u200E∶%02i.%02i", split_minutes, split_seconds,
					   split_cs);
	}

	if (hours > 0) {
	    tot_label = g_strdup_printf ("%i\u200E∶%02i\u200E∶%02i.%02i", hours, minutes, seconds, cs);
	} else {
	    tot_label = g_strdup_printf ("%02i\u200E∶%02i.%02i", minutes, seconds, cs);
	}

	n_label = g_strdup_printf ("#%d", priv->current_lap);
	row = laps_row_new (n_label, split_label, tot_label);

	// FIXME: we can remove this if ListBox gains support for :first-child
	if (priv->current_lap == 1) {
		GtkStyleContext *context;

		context = gtk_widget_get_style_context (row);
		gtk_style_context_add_class (context, "first-lap-row");
	}

	gtk_list_box_prepend (GTK_LIST_BOX (priv->laps_list), row);
	laps_row_slide_in (LAPS_ROW (row));

	GtkAdjustment *vadjustment =
	gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (priv->laps_scrollwin));
	gtk_adjustment_set_value (vadjustment, gtk_adjustment_get_lower (vadjustment));

	g_free (n_label);
	g_free (split_label);
	g_free (tot_label);
}

static void
remove_child (gpointer data, gpointer user_data)
{
	GtkContainer *list = GTK_CONTAINER (user_data);

	gtk_container_remove (list, GTK_WIDGET (data));
}

// FIXME: cleaner implementation?
static void
remove_all_children (GtkWidget *list)
{
	GList *children = NULL;

	children = gtk_container_get_children (GTK_CONTAINER (list));
	g_list_foreach (children, remove_child, list);
}

static void
gstopwatch_face_reset (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;
	GtkStyleContext *context;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	priv->state = RESET;
	g_timer_reset (priv->timer);
	remove_tick (gstopwatch_face);
	update_time_label (GTK_WIDGET (gstopwatch_face), NULL, NULL);

	context = gtk_widget_get_style_context (priv->left_button);
	gtk_style_context_add_class (context, "suggested-action");
	gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Start"));

	gtk_widget_set_sensitive (priv->right_button, FALSE);
	priv->current_lap = 0;
	priv->last_lap_time = 0;

	remove_all_children (priv->laps_list);
}

static void
gstopwatch_face_stop (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;
	GtkStyleContext *context;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	g_timer_stop (priv->timer);
	priv->state = STOPPED;
	remove_tick (gstopwatch_face);
	context = gtk_widget_get_style_context (priv->left_button);
	gtk_style_context_remove_class (context, "destructive-action");
	gtk_style_context_add_class (context, "suggested-action");
	gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Continue"));

	gtk_widget_set_sensitive (priv->right_button, TRUE);
	gtk_button_set_label (GTK_BUTTON (priv->right_button), _("Reset"));
}

static void
gstopwatch_face_start (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;
	GtkStyleContext *context;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	if (priv->state == RESET) {
		g_timer_start (priv->timer);
	} else {
		g_timer_continue (priv->timer);
	}

	priv->state = RUNNING;
	add_tick (gstopwatch_face);

	context = gtk_widget_get_style_context (priv->left_button);
	gtk_style_context_add_class (context, "destructive-action");
	gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Stop"));

	gtk_widget_set_sensitive (priv->right_button, TRUE);
	gtk_button_set_label (GTK_BUTTON (priv->right_button), _("Lap"));
}

static void
on_map (GtkWidget *widget, gpointer user_data)
{
	GStopwatchFace *gstopwatch_face = GSTOPWATCH_FACE (widget);
	GStopwatchFacePrivate *priv;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	if (priv->state == RUNNING) {
		update_time_label (widget, NULL, NULL);
		add_tick (gstopwatch_face);
	}
}

static void
on_unmap (GtkWidget *widget, gpointer user_data)
{
	GStopwatchFace *gstopwatch_face = GSTOPWATCH_FACE (widget);
	GStopwatchFacePrivate *priv;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	if (priv->state == RUNNING) {
		remove_tick (gstopwatch_face);
	}
}

static void
gstopwatch_face_class_init (GStopwatchFaceClass *gstopwatch_face_class)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (gstopwatch_face_class);

	gtk_widget_class_set_template_from_resource (widget_class,
						     "/org/unia/gtimeutils/gstopwatch/stopwatch.ui");

	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, analog_frame);
	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, time_label);
	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, left_button);
	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, right_button);
	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, laps_scrollwin);
	gtk_widget_class_bind_template_child_private (widget_class, GStopwatchFace, laps_list);

	gtk_widget_class_bind_template_callback (widget_class, on_left_button_clicked);
	gtk_widget_class_bind_template_callback (widget_class, on_right_button_clicked);
}

static void
gstopwatch_face_init (GStopwatchFace *gstopwatch_face)
{
	GStopwatchFacePrivate *priv;

	priv = gstopwatch_face_get_instance_private (gstopwatch_face);

	gtk_widget_init_template (GTK_WIDGET (gstopwatch_face));

	priv->state = RESET;
	priv->tick_id = 0;
	priv->current_lap = 0;
	priv->last_lap_time = 0;
	priv->timer = g_timer_new ();

	g_signal_connect (GTK_WIDGET (gstopwatch_face), "map",
			  G_CALLBACK (on_map), NULL);
	g_signal_connect (GTK_WIDGET (gstopwatch_face), "unmap",
			  G_CALLBACK (on_unmap), NULL);

	gstopwatch_face_reset (gstopwatch_face);
}

GtkWidget *
gstopwatch_face_new (void)
{
	return GTK_WIDGET (g_object_new (GSTOPWATCH_FACE_TYPE, NULL));
}

