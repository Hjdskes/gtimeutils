/* GStopwatchWindow
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

#ifndef __GSTOPWATCH_WINDOW_H__
#define __GSTOPWATCH_WINDOW_H__

#include <gtk/gtk.h>

#include "gstopwatch-application.h"

G_BEGIN_DECLS

typedef struct _GStopwatchWindow        GStopwatchWindow;
typedef struct _GStopwatchWindowClass   GStopwatchWindowClass;

#define GSTOPWATCH_TYPE_WINDOW            (gstopwatch_window_get_type ())
#define GSTOPWATCH_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GSTOPWATCH_TYPE_WINDOW, GStopwatchWindow))
#define GSTOPWATCH_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GSTOPWATCH_TYPE_WINDOW, GStopwatchWindowClass))
#define GSTOPWATCH_IS_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GSTOPWATCH_TYPE_WINDOW))
#define GSTOPWATCH_IS_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GSTOPWATCH_TYPE_WINDOW))
#define GSTOPWATCH_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GSTOPWATCH_TYPE_WINDOW, GStopwatchWindowClass))

struct _GStopwatchWindow {
	GtkApplicationWindow base_instance;
};

struct _GStopwatchWindowClass {
	GtkApplicationWindowClass parent_class;
};

GType             gstopwatch_window_get_type (void) G_GNUC_CONST;

GStopwatchWindow *gstopwatch_window_new (GStopwatchApplication *application);

void              gstopwatch_window_show_about_dialog (GStopwatchWindow *window);

void              gstopwatch_window_close (GStopwatchWindow *window);

G_END_DECLS

#endif /* __GSTOPWATCH_WINDOW_H__ */

