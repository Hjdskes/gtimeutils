/* GTimerWindow
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

#ifndef __GTIMER_WINDOW_H__
#define __GTIMER_WINDOW_H__

#include <gtk/gtk.h>

#include "gtimer-application.h"

G_BEGIN_DECLS

typedef struct _GTimerWindow        GTimerWindow;
typedef struct _GTimerWindowClass   GTimerWindowClass;

#define GTIMER_TYPE_WINDOW            (gtimer_window_get_type ())
#define GTIMER_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTIMER_TYPE_WINDOW, GTimerWindow))
#define GTIMER_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GTIMER_TYPE_WINDOW, GTimerWindowClass))
#define GTIMER_IS_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTIMER_TYPE_WINDOW))
#define GTIMER_IS_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GTIMER_TYPE_WINDOW))
#define GTIMER_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GTIMER_TYPE_WINDOW, GTimerWindowClass))

struct _GTimerWindow {
	GtkApplicationWindow base_instance;
};

struct _GTimerWindowClass {
	GtkApplicationWindowClass parent_class;
};

GType         gtimer_window_get_type (void) G_GNUC_CONST;

GTimerWindow *gtimer_window_new (GTimerApplication *application);

void          gtimer_window_show_about_dialog (GTimerWindow *window);

void          gtimer_window_close (GTimerWindow *window);

G_END_DECLS

#endif /* __GTIMER_WINDOW_H__ */

