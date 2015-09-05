/* GTimerFace
 *
 * GTimerFace is a port of Gnome Clock's Timer.Face.
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
#include <gsound.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "../utils.h"
#include "gtimer-face.h"
#include "gtimer-radius.h"

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/* A textual id for an event sound, in this case a completion. */
#define EVENT_ID "complete"

/* The role this media is played in, is an alarm. */
#define MEDIA_ROLE "alarm"

/* Shortcut to retrieve integer value from a GtkSpinButton. */
#define GET_INT(a) (g_strtod (gtk_entry_get_text (GTK_ENTRY ((a))), NULL));

typedef enum State {
	STOPPED,
	RUNNING,
	PAUSED
} State;

struct _GTimerFacePrivate {
	State state;
	guint tick_id;
	double span;
	GTimer *timer;

	struct {
		GSoundContext *context;
		GCancellable *cancellable;
	} bell;

	GNotification *notification;

	GTimerFace *setup_frame;
	GtkWidget *grid_spinbuttons;
	GtkWidget *h_spinbutton;
	GtkWidget *m_spinbutton;
	GtkWidget *s_spinbutton;
	GtkWidget *entry;
	GtkWidget *start_button;

	GTimerRadius *countdown_frame;
	GtkWidget *h_label;
	GtkWidget *m_label;
	GtkWidget *s_label;
	GtkWidget *entry2;
	GtkWidget *left_button;
};

G_DEFINE_TYPE_WITH_PRIVATE (GTimerFace, gtimer_face, GTK_TYPE_STACK);

static void gtimer_face_send_notification (GTimerFace *gtimer_face);
static void gtimer_face_reset (GTimerFace *gtimer_face);
static void gtimer_face_start (GTimerFace *gtimer_face);
static void gtimer_face_pause (GTimerFace *gtimer_face);

static void
update_countdown_label (GTimerFace *gtimer_face, int hours, int minutes, int seconds)
{
	GTimerFacePrivate *priv;

	priv = gtimer_face_get_instance_private (gtimer_face);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->h_spinbutton), hours);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->m_spinbutton), minutes);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->s_spinbutton), seconds);
}

static void
update_countdown (GTimerFace *gtimer_face, double elapsed)
{
	GTimerFacePrivate *priv;

	priv = gtimer_face_get_instance_private (gtimer_face);

	if (gtk_widget_get_mapped (priv->h_label)) {
		/* ceil () because we count backwards:
		 * with 0.3 seconds we want to show 1 second remaining,
		 * with 59.2 seconds we want to show 1 minute, etc. */
		double remaining = ceil (priv->span - elapsed);
		double remainder;
		int hours, minutes, seconds;

		time_to_hms (remaining, &hours, &minutes, &seconds, &remainder);

		update_countdown_label (gtimer_face, hours, minutes, seconds);
		gtimer_radius_update (priv->countdown_frame, elapsed);
	}
}

static gboolean
count (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	double elapsed;

	priv = gtimer_face_get_instance_private (gtimer_face);

	elapsed = g_timer_elapsed (priv->timer, NULL);
	if (elapsed >= priv->span) {
		update_countdown_label (gtimer_face, 0, 0, 0);
		gtimer_face_send_notification (gtimer_face);
		gtimer_face_reset (gtimer_face);
		return FALSE;
	}

	update_countdown (gtimer_face, elapsed);
	return TRUE;
}

static gboolean
tick_callback (GtkWidget *widget, GdkFrameClock *frame_clock, gpointer user_data)
{
	return count (GTIMER_FACE (widget));
}

static void
add_tick (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;

	priv = gtimer_face_get_instance_private (gtimer_face);

	if (priv->tick_id == 0) {
		priv->tick_id = gtk_widget_add_tick_callback (GTK_WIDGET (gtimer_face),
							      tick_callback,
							      NULL,
							      NULL);
	}
}

static void
remove_tick (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;

	priv = gtimer_face_get_instance_private (gtimer_face);

	if (priv->tick_id != 0) {
		gtk_widget_remove_tick_callback (GTK_WIDGET (gtimer_face), priv->tick_id);
		priv->tick_id = 0;
	}
}

static gboolean
show_leading_zeros (GtkSpinButton *spin, gpointer user_data)
{
	GtkAdjustment *adjustment;
	gchar *text;
	int value;

	adjustment = gtk_spin_button_get_adjustment (spin);
	value = (int) gtk_adjustment_get_value (adjustment);
	text = g_strdup_printf ("%02i", value);
	gtk_entry_set_text (GTK_ENTRY (spin), text);
	g_free (text);

	return TRUE;
}

static void
update_start_button (GtkSpinButton *spin, gpointer user_data)
{
	GTimerFacePrivate *priv;
	GtkStyleContext *context;
	int hours;
	int minutes;
	int seconds;

	g_return_if_fail (GTIMER_IS_FACE (user_data));
	priv = gtimer_face_get_instance_private (GTIMER_FACE (user_data));

	context = gtk_widget_get_style_context (priv->start_button);
	hours = GET_INT (priv->h_spinbutton);
	minutes = GET_INT (priv->m_spinbutton);
	seconds = GET_INT (priv->s_spinbutton);

	if (hours != 0 || minutes != 0 || seconds != 0) {
		gtk_widget_set_sensitive (priv->start_button, TRUE);
		gtk_style_context_add_class (context, "suggested-action");
	} else {
		gtk_widget_set_sensitive (priv->start_button, FALSE);
		gtk_style_context_remove_class (context, "suggested-action");
	}
}

static gint
input_minutes (GtkSpinButton *spin, gdouble *new_value, gpointer user_data)
{
	GTimerFacePrivate *priv;
	int minutes;

	g_return_if_fail (GTIMER_IS_FACE (user_data));
	priv = gtimer_face_get_instance_private (GTIMER_FACE (user_data));

	minutes = GET_INT (spin);

	/* If the user enters a value larger than 59, leak the extra minutes into hours. */
	if (minutes > 59) {
		int hours = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (priv->h_spinbutton));
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->h_spinbutton),
					   MIN (99, hours + minutes / 60));
	}

	*new_value = minutes % 60;
	return TRUE;
}

static gint
input_seconds (GtkSpinButton *spin, gdouble *new_value, gpointer user_data)
{
	GTimerFacePrivate *priv;
	int seconds;

	g_return_if_fail (GTIMER_IS_FACE (user_data));
	priv = gtimer_face_get_instance_private (GTIMER_FACE (user_data));

	seconds = GET_INT (spin);

	/* If the user enters a value larger than 59, leak the extra seconds
	 * into minutes and hours. */
	if (seconds > 59) {
		int minutes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (priv->m_spinbutton));
		int new_minutes = minutes + seconds / 60;

		if (new_minutes > 59) {
			int hours = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (priv->h_spinbutton));
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->h_spinbutton),
						   MIN (99, hours + new_minutes / 60));
			new_minutes %= 60;
		}

		gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->m_spinbutton), new_minutes);
	}

	*new_value = seconds % 60;
	return TRUE;
}

static void
on_start_button_clicked (GtkButton *button, gpointer user_data)
{
	g_return_if_fail (GTIMER_IS_FACE (user_data));
	gtimer_face_start (GTIMER_FACE (user_data));
}

static void
on_left_button_clicked (GtkButton *button, gpointer user_data)
{
	GTimerFacePrivate *priv;
	GtkStyleContext *context;

	g_return_if_fail (GTIMER_IS_FACE (user_data));
	priv = gtimer_face_get_instance_private (GTIMER_FACE (user_data));

	context = gtk_widget_get_style_context (priv->left_button);
	switch (priv->state) {
		case RUNNING:
			gtimer_face_pause (GTIMER_FACE (user_data));
			gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Continue"));
			gtk_style_context_add_class (context, "suggested-action");
			break;
		case PAUSED:
			gtimer_face_start (GTIMER_FACE (user_data));
			gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Pause"));
			gtk_style_context_remove_class (context, "suggested-action");
			break;
		default:
			g_assert_not_reached ();
	}
}

static void
on_right_button_clicked (GtkButton *button, gpointer user_data)
{
	GTimerFacePrivate *priv;

	g_return_if_fail (GTIMER_IS_FACE (user_data));
	priv = gtimer_face_get_instance_private (GTIMER_FACE (user_data));

	gtimer_face_reset (GTIMER_FACE (user_data));
	gtk_button_set_label (GTK_BUTTON (priv->left_button), _("Pause"));
}

static void
gtimer_face_send_notification_text (GApplication *application,
				   GNotification *notification,
				   GtkEntry *entry)
{
	if (gtk_entry_get_text_length (entry)) {
			const char *title = gtk_entry_get_text (entry);
			g_notification_set_title (notification, title);
	}

	g_application_send_notification (application, "gtimer", notification);
}

static void
gtimer_face_send_notification_bell (GSoundContext *context, GCancellable *cancellable)
{
	GError *error = NULL;

	g_return_if_fail (context != NULL);

	gsound_context_play_simple (context, cancellable, &error,
				    GSOUND_ATTR_EVENT_ID, EVENT_ID,
				    GSOUND_ATTR_MEDIA_ROLE, MEDIA_ROLE,
				    NULL);

	if (error) {
		g_warning (_("Error playing sound: %s"), error->message);
		g_clear_error (&error);
	}
}

static void
gtimer_face_send_notification (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	GApplication *application = NULL;

	priv = gtimer_face_get_instance_private (gtimer_face);

	application = g_application_get_default ();
	if (application) {
		gtimer_face_send_notification_text (application, priv->notification,
						   GTK_ENTRY (priv->entry));
		gtimer_face_send_notification_bell (priv->bell.context, priv->bell.cancellable);
	}
}

static void
gtimer_face_reset (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	GtkStyleContext *context;

	priv = gtimer_face_get_instance_private (gtimer_face);

	priv->state = STOPPED;
	g_timer_reset (priv->timer);
	remove_tick (gtimer_face);
	update_countdown_label (gtimer_face, (int) priv->span / 3600, (int) priv->span / 60, (int) priv->span % 60);

	context = gtk_widget_get_style_context (priv->left_button);
	gtk_style_context_remove_class (context, "clocks-go");

	context = gtk_widget_get_style_context (GTK_WIDGET (priv->countdown_frame));
	gtk_style_context_remove_class (context, "clocks-paused");

	gtk_widget_set_sensitive (priv->start_button, priv->span > 0);
	gtimer_radius_reset (priv->countdown_frame);
	gtk_stack_set_visible_child (GTK_STACK (gtimer_face), GTK_WIDGET (priv->setup_frame));
}

static void
gtimer_face_start (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	GtkStyleContext *context;

	priv = gtimer_face_get_instance_private (gtimer_face);

	context = gtk_widget_get_style_context (GTK_WIDGET (priv->countdown_frame));
	gtk_style_context_remove_class (context, "clocks-paused");

	if (priv->state == STOPPED && priv->tick_id == 0) {
		int hours = GET_INT (priv->h_spinbutton);
		int minutes = GET_INT (priv->m_spinbutton);
		int seconds = GET_INT (priv->s_spinbutton);

		priv->span = hours * 3600 + minutes * 60 + seconds;
		gtimer_radius_set_span (priv->countdown_frame, priv->span);
		gtk_stack_set_visible_child (GTK_STACK (gtimer_face), GTK_WIDGET (priv->countdown_frame));

		update_countdown_label (gtimer_face, hours, minutes, seconds);
	}

	priv->state = RUNNING;
	g_timer_start (priv->timer);
	add_tick (gtimer_face);
}

static void
gtimer_face_pause (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	GtkStyleContext *context;

	priv = gtimer_face_get_instance_private (gtimer_face);

	priv->state = PAUSED;
	g_timer_stop (priv->timer);

	priv->span -= g_timer_elapsed (priv->timer, NULL);
	context = gtk_widget_get_style_context (GTK_WIDGET (priv->countdown_frame));
	gtk_style_context_add_class (context, "clocks-paused");
	gtimer_radius_pause (priv->countdown_frame);
	remove_tick (gtimer_face);
}

static void
gtimer_face_dispose (GObject *object)
{
	GTimerFacePrivate *priv;

	g_return_if_fail (object != NULL);
	priv = gtimer_face_get_instance_private (GTIMER_FACE (object));

	if (priv->notification) {
		g_clear_object (&priv->notification);
	}

	if (priv->bell.context) {
		g_clear_object (&priv->bell.context);
	}

	G_OBJECT_CLASS (gtimer_face_parent_class)->dispose (object);
}

static void
gtimer_face_class_init (GTimerFaceClass *gtimer_face_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gtimer_face_class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (gtimer_face_class);

	object_class->dispose = gtimer_face_dispose;

	gtk_widget_class_set_template_from_resource (widget_class,
						     "/org/unia/gtimeutils/gtimer/timer.ui");

	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, setup_frame);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, grid_spinbuttons);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, h_spinbutton);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, m_spinbutton);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, s_spinbutton);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, entry);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, start_button);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, countdown_frame);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, h_label);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, m_label);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, s_label);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, entry2);
	gtk_widget_class_bind_template_child_private (widget_class, GTimerFace, left_button);

	gtk_widget_class_bind_template_callback (widget_class, show_leading_zeros);
	gtk_widget_class_bind_template_callback (widget_class, update_start_button);
	gtk_widget_class_bind_template_callback (widget_class, input_minutes);
	gtk_widget_class_bind_template_callback (widget_class, input_seconds);
	gtk_widget_class_bind_template_callback (widget_class, on_start_button_clicked);
	gtk_widget_class_bind_template_callback (widget_class, on_left_button_clicked);
	gtk_widget_class_bind_template_callback (widget_class, on_right_button_clicked);
}

static void
gtimer_face_init (GTimerFace *gtimer_face)
{
	GTimerFacePrivate *priv;
	GError *error = NULL;

	priv = gtimer_face_get_instance_private (gtimer_face);

	gtk_widget_init_template (GTK_WIDGET (gtimer_face));

	priv->state = STOPPED;
	priv->tick_id = 0;
	priv->span = 0;
	priv->timer = g_timer_new ();
	priv->bell.cancellable = g_cancellable_new ();
	priv->bell.context = gsound_context_new (priv->bell.cancellable, &error);
	if (error) {
		g_warning (_("Sound can not be played: %s"), error->message);
		g_clear_error (&error);
	}

	priv->notification = g_notification_new (_("Time is up!"));
	g_notification_set_body (priv->notification, _("Timer countdown finished"));

	/* Bind entry's text to entry2's text. */
	g_object_bind_property (priv->entry, "text", priv->entry2, "text", G_BINDING_DEFAULT);

	/* Force LTR since we do not want to reverse [hh] : [mm] : [ss]. */
	gtk_widget_set_direction (priv->grid_spinbuttons, GTK_TEXT_DIR_LTR);

	gtk_stack_set_transition_type (GTK_STACK (gtimer_face), GTK_STACK_TRANSITION_TYPE_CROSSFADE);

	gtimer_face_reset (gtimer_face);
}

GtkWidget *
gtimer_face_new (void)
{
	return GTK_WIDGET (g_object_new (GTIMER_FACE_TYPE, NULL));
}

