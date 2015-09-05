/* GStopwatchApplication
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

#ifndef __GSTOPWATCH_APPLICATION_H__
#define __GSTOPWATCH_APPLICATION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _GStopwatchApplication        GStopwatchApplication;
typedef struct _GStopwatchApplicationClass   GStopwatchApplicationClass;

#define GSTOPWATCH_TYPE_APPLICATION            (gstopwatch_application_get_type ())
#define GSTOPWATCH_APPLICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GSTOPWATCH_TYPE_APPLICATION, GStopwatchApplication))
#define GSTOPWATCH_APPLICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GSTOPWATCH_TYPE_APPLICATION, GStopwatchApplicationClass))
#define GSTOPWATCH_IS_APPLICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GSTOPWATCH_TYPE_APPLICATION))
#define GSTOPWATCH_IS_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GSTOPWATCH_TYPE_APPLICATION))
#define GSTOPWATCH_APPLICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GSTOPWATCH_TYPE_APPLICATION, GStopwatchApplicationClass))

struct _GStopwatchApplication {
	GtkApplication base_instance;
};

struct _GStopwatchApplicationClass {
	GtkApplicationClass parent_class;
};

GType                  gstopwatch_application_get_type (void) G_GNUC_CONST;

GStopwatchApplication *gstopwatch_application_new (void);

G_END_DECLS

#endif /* __GSTOPWATCH_APPLICATION_H__ */

