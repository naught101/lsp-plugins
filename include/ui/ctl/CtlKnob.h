/*
 * CtlKnob.h
 *
 *  Created on: 11 июл. 2017 г.
 *      Author: sadko
 */

#ifndef UI_CTL_CTLKNOB_H_
#define UI_CTL_CTLKNOB_H_

namespace lsp
{
    namespace ctl
    {
        
        class CtlKnob: public CtlWidget
        {
            protected:
                CtlColor            sColor;
                CtlColor            sBgColor;
                CtlColor            sScaleColor;

                bool                bLog;
                bool                bLogSet;
                CtlPort            *pPort;

            protected:
                static status_t    slot_change(LSPWidget *sender, void *ptr, void *data);
                void        submit_value();
                void        commit_value(float value);

            public:
                CtlKnob(CtlRegistry *src, LSPKnob *widget);
                virtual ~CtlKnob();

            public:

                virtual void init();

                virtual void set(widget_attribute_t att, const char *value);

                virtual void notify(CtlPort *port);

                virtual void end();
        };
    
    } /* namespace ctl */
} /* namespace lsp */

#endif /* UI_CTL_CTLKNOB_H_ */
