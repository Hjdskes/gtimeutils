/* Radius
 *
 * Radius is a port of Gnome Clock's AnalogFrame.
 *
 * Copyright (C) 2015 Jente Hidskes
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

#ifndef __RADIUS_H__
#define __RADIUS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _Radius      Radius;
typedef struct _RadiusClass RadiusClass;

#define RADIUS_TYPE            (radius_get_type ())
#define RADIUS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), RADIUS_TYPE, Radius))
#define RADIUS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  RADIUS_TYPE, RadiusClass))
#define IS_RADIUS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), RADIUS_TYPE))
#define IS_RADIUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  RADIUS_TYPE))
#define RADIUS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  RADIUS_TYPE, RadiusClass))

struct _Radius {
	/*< private >*/
	GtkFrame base_instance;
};

/**
 * RadiusClass:
 * @draw_progress: Method used to draw progress inside the radius.
 */
struct _RadiusClass {
	/*< private >*/
	GtkFrameClass parent_class;

	/*< public >*/
	void (* draw_progress) (GtkWidget *widget, cairo_t *cr, int center_x, int center_y, int radius);
};

GType radius_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __RADIUS_H__ */

