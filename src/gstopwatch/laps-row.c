/* LapsRow
 *
 * LapsRow is a port of Gnome Clock's Stopwatch.LapsRow.
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

#include <gtk/gtk.h>

#include "laps-row.h"

enum {
	PROP_0,
	PROP_NUM,
	PROP_SPLIT,
	PROP_TOT
};

struct _LapsRowPrivate {
	GtkWidget *slider;
	GtkWidget *num_label;
	GtkWidget *split_label;
	GtkWidget *tot_label;
};

G_DEFINE_TYPE_WITH_PRIVATE (LapsRow, laps_row, GTK_TYPE_LIST_BOX_ROW);

static void
laps_row_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	LapsRowPrivate *priv;

	priv = laps_row_get_instance_private (LAPS_ROW (object));

	switch (prop_id) {
		case PROP_NUM:
			{
				const gchar *label = g_value_get_string (value);
				gtk_label_set_text (GTK_LABEL (priv->num_label), label);
			}
			break;
		case PROP_SPLIT:
			{
				const gchar *label = g_value_get_string (value);
				gtk_label_set_text (GTK_LABEL (priv->split_label), label);
			}
			break;
		case PROP_TOT:
			{
				const gchar *label = g_value_get_string (value);
				gtk_label_set_text (GTK_LABEL (priv->tot_label), label);
			}
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
laps_row_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	LapsRowPrivate *priv;

	priv = laps_row_get_instance_private (LAPS_ROW (object));

	switch (prop_id) {
		case PROP_NUM:
			{
				const gchar *label = gtk_label_get_text (GTK_LABEL (priv->num_label));
				g_value_set_string (value, label);
			}
			break;
		case PROP_SPLIT:
			{
				const gchar *label = gtk_label_get_text (GTK_LABEL (priv->split_label));
				g_value_set_string (value, label);
			}
			break;
		case PROP_TOT:
			{
				const gchar *label = gtk_label_get_text (GTK_LABEL (priv->tot_label));
				g_value_set_string (value, label);
			}
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
laps_row_class_init (LapsRowClass *row_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (row_class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (row_class);

	object_class->get_property = laps_row_get_property;
	object_class->set_property = laps_row_set_property;

	gtk_widget_class_set_template_from_resource (widget_class,
						     "/org/unia/gtimeutils/gstopwatch/laps-row.ui");

	gtk_widget_class_bind_template_child_private (widget_class, LapsRow, slider);
	gtk_widget_class_bind_template_child_private (widget_class, LapsRow, num_label);
	gtk_widget_class_bind_template_child_private (widget_class, LapsRow, split_label);
	gtk_widget_class_bind_template_child_private (widget_class, LapsRow, tot_label);

	g_object_class_install_property (
			object_class, PROP_NUM,
			g_param_spec_string ("lap-number", NULL, NULL, "0",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_NAME)
		);
	g_object_class_install_property (
			object_class, PROP_SPLIT,
			g_param_spec_string ("lap-time", NULL, NULL, "0",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_NAME)
		);
	g_object_class_install_property (
			object_class, PROP_TOT,
			g_param_spec_string ("total-time", NULL, NULL, "0",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_NAME)
		);
}

static void
laps_row_init (LapsRow *row)
{
	gtk_widget_init_template (GTK_WIDGET (row));
}

GtkWidget *
laps_row_new (const gchar *n_label, const gchar *split_label, const gchar *tot_label)
{
	return GTK_WIDGET (g_object_new (LAPS_ROW_TYPE,
					 "lap-number", n_label,
					 "lap-time", split_label,
					 "total-time", tot_label,
					 NULL));
}

void
laps_row_slide_in (LapsRow *row)
{
	LapsRowPrivate *priv;

	priv = laps_row_get_instance_private (row);

	gtk_revealer_set_reveal_child (GTK_REVEALER (priv->slider), TRUE);
}

