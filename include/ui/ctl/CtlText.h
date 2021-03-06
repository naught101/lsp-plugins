/*
 * CtlText.h
 *
 *  Created on: 27 июл. 2017 г.
 *      Author: sadko
 */

#ifndef UI_CTL_CTLTEXT_H_
#define UI_CTL_CTLTEXT_H_

namespace lsp
{
    namespace ctl
    {
        class CtlText: public CtlWidget
        {
            protected:
                CtlExpression   sCoord;
                CtlExpression   sBasis;
                CtlColor        sColor;

            protected:
                void        update_coords();

            public:
                CtlText(CtlRegistry *src, LSPText *text);
                virtual ~CtlText();

            public:
                virtual void init();

                virtual void end();

                virtual void set(widget_attribute_t att, const char *value);

                virtual void notify(CtlPort *port);
        };
    
    } /* namespace ctl */
} /* namespace lsp */

#endif /* UI_CTL_CTLTEXT_H_ */
