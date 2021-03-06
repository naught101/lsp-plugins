/*
 * CtlHyperlink.cpp
 *
 *  Created on: 23 окт. 2017 г.
 *      Author: sadko
 */

#include <ui/ctl/ctl.h>

namespace lsp
{
    namespace ctl
    {
        CtlHyperlink::CtlHyperlink(CtlRegistry *src, LSPHyperlink *widget, ctl_label_type_t type): CtlWidget(src, widget)
        {
        }

        CtlHyperlink::~CtlHyperlink()
        {
        }

        void CtlHyperlink::init()
        {
            CtlWidget::init();

            if (pWidget == NULL)
                return;

            LSPHyperlink *hlink = widget_cast<LSPHyperlink>(pWidget);
            if (hlink == NULL)
                return;

            // Initialize color controllers
            sColor.init_hsl(pRegistry, hlink, hlink->font()->color(), A_COLOR, A_HUE_ID, A_SAT_ID, A_LIGHT_ID);
            sHoverColor.init_hsl(pRegistry, hlink, hlink->hover(), A_COLOR, A_HUE_ID, A_SAT_ID, A_LIGHT_ID);
            sBgColor.init_basic(pRegistry, hlink, hlink->bg_color(), A_BG_COLOR);
        }

        void CtlHyperlink::set(widget_attribute_t att, const char *value)
        {
            LSPHyperlink *hlink = widget_cast<LSPHyperlink>(pWidget);

            switch (att)
            {
                case A_TEXT:
                    if (hlink != NULL)
                        hlink->set_text(value);
                    break;
                case A_URL:
                    if (hlink != NULL)
                        hlink->set_url(value);
                    break;
                case A_FONT_SIZE:
                    if (hlink != NULL)
                        PARSE_FLOAT(value, hlink->font()->set_size(__));
                    break;
                case A_VALIGN:
                    if (hlink != NULL)
                        PARSE_FLOAT(value, hlink->set_valign(__));
                    break;
                case A_HALIGN:
                    if (hlink != NULL)
                        PARSE_FLOAT(value, hlink->set_halign(__));
                    break;
                case A_BORDER:
                    PARSE_INT(value, hlink->set_border(__));
                    break;
                default:
                {
                    bool set = sColor.set(att, value);
                    set |= sBgColor.set(att, value);
                    set |= sHoverColor.set(att, value);

                    if (!set)
                        CtlWidget::set(att, value);
                    break;
                }
            }
        }

    } /* namespace ctl */
} /* namespace lsp */
