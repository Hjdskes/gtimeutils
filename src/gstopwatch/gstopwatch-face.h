/* GStopwatchFace
 *
 * GStopwatchFace is a port of Gnome Clock's Stopwatch.Face.
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

#ifndef __GSTOPWATCH_FACE_H__
#define __GSTOPWATCH_FACE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _GStopwatchFace        GStopwatchFace;
typedef struct _GStopwatchFaceClass   GStopwatchFaceClass;
typedef struct _GStopwatchFacePrivate GStopwatchFacePrivate;

#define GSTOPWATCH_FACE_TYPE            (gstopwatch_face_get_type ())
#define GSTOPWATCH_FACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GSTOPWATCH_FACE_TYPE, GStopwatchFace))
#define GSTOPWATCH_FACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GSTOPWATCH_FACE_TYPE, GStopwatchFaceClass))
#define GSTOPWATCH_IS_FACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GSTOPWATCH_FACE_TYPE))
#define GSTOPWATCH_IS_FACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GSTOPWATCH_FACE_TYPE))
#define GSTOPWATCH_FACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GSTOPWATCH_FACE_TYPE, GStopwatchFaceClass))

struct _GStopwatchFace {
	GtkBox base_instance;
};

struct _GStopwatchFaceClass {
	GtkBoxClass parent_class;
};

GType      gstopwatch_face_get_type (void) G_GNUC_CONST;

GtkWidget *gstopwatch_face_new (void);

G_END_DECLS

#endif /* __GSTOPWATCH_FACE_H__ */

