/* GTimerFace
 *
 * GTimerFace is a port of Gnome Clock's Timer.Face.
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

#ifndef __GTIMER_FACE_H__
#define __GTIMER_FACE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _GTimerFace        GTimerFace;
typedef struct _GTimerFaceClass   GTimerFaceClass;
typedef struct _GTimerFacePrivate GTimerFacePrivate;

#define GTIMER_FACE_TYPE            (gtimer_face_get_type ())
#define GTIMER_FACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTIMER_FACE_TYPE, GTimerFace))
#define GTIMER_FACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GTIMER_FACE_TYPE, GTimerFaceClass))
#define GTIMER_IS_FACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTIMER_FACE_TYPE))
#define GTIMER_IS_FACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GTIMER_FACE_TYPE))
#define GTIMER_FACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GTIMER_FACE_TYPE, GTimerFaceClass))

struct _GTimerFace {
	GtkStack base_instance;
};

struct _GTimerFaceClass {
	GtkStackClass parent_class;
};

GType      gtimer_face_get_type (void) G_GNUC_CONST;

GtkWidget *gtimer_face_new (void);

G_END_DECLS

#endif /* __GTIMER_FACE_H__ */

