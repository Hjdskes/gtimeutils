/* GTimerApplication
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

#ifndef __GTIMER_APPLICATION_H__
#define __GTIMER_APPLICATION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _GTimerApplication        GTimerApplication;
typedef struct _GTimerApplicationClass   GTimerApplicationClass;

#define GTIMER_TYPE_APPLICATION            (gtimer_application_get_type ())
#define GTIMER_APPLICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTIMER_TYPE_APPLICATION, GTimerApplication))
#define GTIMER_APPLICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GTIMER_TYPE_APPLICATION, GTimerApplicationClass))
#define GTIMER_IS_APPLICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTIMER_TYPE_APPLICATION))
#define GTIMER_IS_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GTIMER_TYPE_APPLICATION))
#define GTIMER_APPLICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GTIMER_TYPE_APPLICATION, GTimerApplicationClass))

struct _GTimerApplication {
	GtkApplication base_instance;
};

struct _GTimerApplicationClass {
	GtkApplicationClass parent_class;
};

GType              gtimer_application_get_type (void) G_GNUC_CONST;

GTimerApplication *gtimer_application_new (void);

G_END_DECLS

#endif /* __GTIMER_APPLICATION_H__ */

