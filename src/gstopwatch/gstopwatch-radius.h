/* GStopwatchRadius
 *
 * GStopwatchRadius is a port of Gnome Clock's Stopwatch.Frame.
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

#ifndef __GSTOPWATCH_RADIUS_H__
#define __GSTOPWATCH_RADIUS_H__

#include <gtk/gtk.h>

#include "../radius.h"

G_BEGIN_DECLS

typedef struct _GStopwatchRadius        GStopwatchRadius;
typedef struct _GStopwatchRadiusClass   GStopwatchRadiusClass;
typedef struct _GStopwatchRadiusPrivate GStopwatchRadiusPrivate;

#define GSTOPWATCH_RADIUS_TYPE            (gstopwatch_radius_get_type ())
#define GSTOPWATCH_RADIUS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GSTOPWATCH_RADIUS_TYPE, GStopwatchRadius))
#define GSTOPWATCH_RADIUS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GSTOPWATCH_RADIUS_TYPE, GStopwatchRadiusClass))
#define GSTOPWATCH_IS_RADIUS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GSTOPWATCH_RADIUS_TYPE))
#define GSTOPWATCH_IS_RADIUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GSTOPWATCH_RADIUS_TYPE))
#define GSTOPWATCH_RADIUS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GSTOPWATCH_RADIUS_TYPE, GStopwatchRadiusClass))

struct _GStopwatchRadius {
	Radius base_instance;
};

struct _GStopwatchRadiusClass {
	RadiusClass parent_class;
};

GType             gstopwatch_radius_get_type (void) G_GNUC_CONST;

GStopwatchRadius *gstopwatch_radius_new (GtkWidget *child);

void              gstopwatch_radius_reset (GStopwatchRadius *gstopwatch_radius);

void              gstopwatch_radius_update (GStopwatchRadius *gstopwatch_radius,
					    unsigned int seconds,
					    double milliseconds);

G_END_DECLS

#endif /* __GSTOPWATCH_RADIUS_H__ */

