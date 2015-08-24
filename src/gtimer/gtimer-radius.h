/* GTimerRadius
 *
 * GTimerRadius is a port of Gnome Clock's CountDownFrame.
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

#ifndef __GTIMER_RADIUS_H__
#define __GTIMER_RADIUS_H__

#include <gtk/gtk.h>

#include "../radius.h"

G_BEGIN_DECLS

typedef struct _GTimerRadius        GTimerRadius;
typedef struct _GTimerRadiusClass   GTimerRadiusClass;
typedef struct _GTimerRadiusPrivate GTimerRadiusPrivate;

#define GTIMER_RADIUS_TYPE            (gtimer_radius_get_type ())
#define GTIMER_RADIUS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTIMER_RADIUS_TYPE, GTimerRadius))
#define GTIMER_RADIUS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GTIMER_RADIUS_TYPE, GTimerRadiusClass))
#define GTIMER_IS_RADIUS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTIMER_RADIUS_TYPE))
#define GTIMER_IS_RADIUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GTIMER_RADIUS_TYPE))
#define GTIMER_RADIUS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GTIMER_RADIUS_TYPE, GTimerRadiusClass))

struct _GTimerRadius {
	Radius base_instance;
};

struct _GTimerRadiusClass {
	RadiusClass parent_class;
};

GType         gtimer_radius_get_type (void) G_GNUC_CONST;

GTimerRadius *gtimer_radius_new (GtkWidget *child);

void          gtimer_radius_set_span (GTimerRadius *gtimer_radius, double span);

void          gtimer_radius_reset (GTimerRadius *gtimer_radius);

void          gtimer_radius_pause (GTimerRadius *gtimer_radius);

void          gtimer_radius_update (GTimerRadius *gtimer_radius, double elapsed);

G_END_DECLS

#endif /* __GTIMER_RADIUS_H__ */

