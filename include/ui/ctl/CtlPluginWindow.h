/*
 * CtlPluginWindow.h
 *
 *  Created on: 27 июн. 2017 г.
 *      Author: sadko
 */

#ifndef UI_CTL_CTLPLUGINWINDOW_H_
#define UI_CTL_CTLPLUGINWINDOW_H_

namespace lsp
{
    class plugin_ui;

    namespace ctl
    {
        class CtlPluginWindow: public CtlWidget
        {
            protected:
                bool                bResizable;
                LSPWindow          *pWnd;
                LSPWindow          *pMessage;
                LSPBox             *pBox;
                cvector<LSPWidget>  vWidgets;
                LSPWidget          *vMStud[3];
                LSPMenu            *pMenu;
                LSPFileDialog      *pImport;
                LSPFileDialog      *pExport;
                plugin_ui          *pUI;
                CtlPort            *pPMStud;
                CtlPort            *pPVersion;
                CtlPort            *pPBypass;
                CtlPort            *pPath;

            protected:
                static status_t slot_window_close(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_window_show(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_message_close(LSPWidget *sender, void *ptr, void *data);

                static status_t slot_export_settings(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_import_settings(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_toggle_rack_mount(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_show_menu_top(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_show_menu_left(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_show_menu_right(LSPWidget *sender, void *ptr, void *data);

                static status_t slot_call_export_settings(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_call_import_settings(LSPWidget *sender, void *ptr, void *data);

                static status_t slot_fetch_path(LSPWidget *sender, void *ptr, void *data);
                static status_t slot_commit_path(LSPWidget *sender, void *ptr, void *data);

                status_t show_notification();
                status_t show_menu(size_t actor_id, void *data);
                LSPLabel *create_label(LSPWidgetContainer *dst, const char *text, float halign = 0.0f);
                LSPHyperlink *create_hlink(LSPWidgetContainer *dst, const char *text, float halign = 0.0f);

            public:
                CtlPluginWindow(plugin_ui *src, LSPWindow *wnd);
                virtual ~CtlPluginWindow();

            public:
                /** Init widget
                 *
                 */
                virtual void init();

                /** Set attribute
                 *
                 * @param att widget attribute
                 * @param value widget value
                 */
                virtual void set(widget_attribute_t att, const char *value);

                /** Add child widget
                 *
                 * @param child child widget to add
                 */
                virtual status_t add(LSPWidget *child);

                /** End initialization of controller
                 *
                 */
                virtual void end();

                virtual void notify(CtlPort *port);
        };
    
    } /* namespace ctl */
} /* namespace lsp */

#endif /* UI_CTL_CTLPLUGINWINDOW_H_ */
