/* LapsRow
 *
 * LapsRow is a port of Gnome Clock's Stopwatch.LapsRow.
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

#ifndef __LAPS_ROW_H__
#define __LAPS_ROW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _LapsRow        LapsRow;
typedef struct _LapsRowClass   LapsRowClass;
typedef struct _LapsRowPrivate LapsRowPrivate;

#define LAPS_ROW_TYPE            (laps_row_get_type ())
#define LAPS_ROW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), LAPS_ROW_TYPE, LapsRow))
#define LAPS_ROW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  LAPS_ROW_TYPE, LapsRowClass))
#define IS_LAPS_ROW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAPS_ROW_TYPE))
#define IS_LAPS_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  LAPS_ROW_TYPE))
#define LAPS_ROW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  LAPS_ROW_TYPE, LapsRowClass))

struct _LapsRow {
	GtkListBoxRow base_instance;
};

struct _LapsRowClass {
	GtkListBoxRowClass parent_class;
};

GType      laps_row_get_type (void) G_GNUC_CONST;

GtkWidget *laps_row_new (const gchar *n_label,
			 const gchar *split_label,
			 const gchar *tot_label);

void       laps_row_slide_in (LapsRow *row);

G_END_DECLS

#endif /* __LAPS_ROW_H__ */

