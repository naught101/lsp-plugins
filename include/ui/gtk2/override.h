/*
 * override.h
 *
 *  Created on: 10 нояб. 2015 г.
 *      Author: sadko
 */

#ifndef UI_GTK2_OVERRIDE_H_
#define UI_GTK2_OVERRIDE_H_

#include <ui/gtk2/ui.h>

#define OVERRIDE_GTK2_CONTROL(gtk_type, gtk_class, gtk_widget, class)   \
    typedef struct class ## Impl { \
        gtk_widget widget; \
        class *impl; \
    } class ## Impl; \
    \
    typedef struct class ## Class { \
        gtk_class parent; \
        void (* size_request) (GtkWidget *widget, GtkRequisition *requisition); \
    } class ## Class; \
    \
    static GType class ## Impl_get_type (void); \
    \
    static void class ## Impl_size_request(GtkWidget *widget, GtkRequisition *requisition) \
    { \
        class ## Impl *_this = G_TYPE_CHECK_INSTANCE_CAST (widget, class ## Impl_get_type(), class ## Impl); \
        class ## Class *cl = G_TYPE_INSTANCE_GET_CLASS (widget, class ## Impl_get_type(), class ## Class); \
        if (cl->size_request != NULL) \
            cl->size_request(widget, requisition); \
        \
        ssize_t w = requisition->width, h = requisition->height; \
        if (_this->impl != NULL) \
            _this->impl->resize(w, h); \
        requisition->width  = w;    \
        requisition->height = h;   \
    } \
    \
    static gboolean class ## Impl_expose (GtkWidget *widget, GdkEventExpose *event) \
    { \
        g_assert(G_TYPE_CHECK_INSTANCE_TYPE (widget, class ## Impl_get_type())); \
        if (gtk_widget_is_drawable (widget)) \
        { \
            class ## Impl *_this = G_TYPE_CHECK_INSTANCE_CAST (widget, class ## Impl_get_type(), class ## Impl); \
            if (_this->impl != NULL) \
                _this->impl->render(); \
        } \
    \
        if (GTK_IS_CONTAINER(widget)) \
        { \
            GList *child = gtk_container_get_children(GTK_CONTAINER(widget)); \
            while (child != NULL) \
            { \
                if (GTK_IS_WIDGET(child->data)) \
                    gtk_container_propagate_expose(GTK_CONTAINER(widget), GTK_WIDGET(child->data), event); \
     \
                child = child->next; \
            } \
        } \
     \
        return FALSE; \
    } \
     \
    static void class ## Impl_class_init (class ## Class *cl) \
    { \
        GtkWidgetClass *wc = GTK_WIDGET_CLASS(cl); \
        cl->size_request = wc->size_request; \
        wc->expose_event = class ## Impl_expose; \
        wc->size_request = class ## Impl_size_request; \
    } \
     \
    static void class ## Impl_init(class ## Impl *self) \
    { \
        GtkWidget *widget = GTK_WIDGET(self); \
        widget->requisition.width   = 32; \
        widget->requisition.height  = 32; \
    } \
     \
    static GType class ## Impl_get_type (void) \
    { \
        static GType type = 0; \
        \
        if (!type) \
        { \
            static const GTypeInfo type_info = \
            { \
                sizeof(class ## Class), \
                NULL, /* base_init */ \
                NULL, /* base_finalize */ \
                reinterpret_cast<GClassInitFunc>(class ## Impl_class_init), \
                NULL, /* class_finalize */ \
                NULL, /* class_data */ \
                sizeof(class ## Impl), \
                0,    /* n_preallocs */ \
                reinterpret_cast<GInstanceInitFunc>(class ## Impl_init) \
            }; \
     \
            for (int i = 0; ; i++) \
            { \
                gchar *name = g_strdup_printf("Custom%s-v%d", #gtk_widget, int(i)); \
                if (!g_type_from_name(name)) \
                { \
                    type = g_type_register_static(gtk_type, name, &type_info, (GTypeFlags)0); \
                    g_free(name); \
                    break; \
                } \
                g_free(name); \
            } \
        } \
        return type; \
    } \
    \
    static GtkWidget *class ## Impl_new(class *self) \
    { \
        GtkWidget *widget = GTK_WIDGET( g_object_new (class ## Impl_get_type(), NULL )); \
        class ## Impl *_this = G_TYPE_CHECK_INSTANCE_CAST (widget, class ## Impl_get_type(), class ## Impl); \
        _this->impl = self; \
        \
        return widget; \
    } \
    \
    static void class ## Impl_delete(GtkWidget *widget) \
    { \
        class ## Impl *_this = G_TYPE_CHECK_INSTANCE_CAST (widget, class ## Impl_get_type(), class ## Impl); \
        _this->impl = NULL; \
    }


#endif /* UI_GTK2_OVERRIDE_H_ */
