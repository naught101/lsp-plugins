/*
 * LSPGraph.cpp
 *
 *  Created on: 18 июл. 2017 г.
 *      Author: sadko
 */

#include <ui/tk/tk.h>
#include <ui/tk/helpers/draw.h>
#include <time.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t LSPGraph::metadata = { "LSPGraph", &LSPWidgetContainer::metadata };

        LSPGraph::LSPGraph(LSPDisplay *dpy):
            LSPWidgetContainer(dpy),
            sIPadding(this)
        {
            nMinWidth       = 0;
            nMinHeight      = 0;
            nBorder         = 12;
            nRadius         = 4;
            fCanvasLeft     = 0.0f;
            fCanvasTop      = 0.0f;
            pCanvas         = NULL;
            pGlass          = NULL;
            pClass          = &metadata;

            sIPadding.set(1, 1, 1, 1);

            #ifdef LSP_TRACE
            sClock.tv_sec   = 0;
            sClock.tv_nsec  = 0;
            nFrames         = 0;
            #endif /* LSP_TRACE */
        }

        LSPGraph::~LSPGraph()
        {
            do_destroy();
        }

        status_t LSPGraph::init()
        {
            status_t result = LSPWidgetContainer::init();
            if (result != STATUS_OK)
                return result;

            if (pDisplay != NULL)
            {
                LSPTheme *theme = pDisplay->theme();

                if (theme != NULL)
                {
                    theme->get_color(C_GLASS, &sColor);
                    theme->get_color(C_BACKGROUND, &sBgColor);
                }
            }

            return STATUS_OK;
        }

        void LSPGraph::do_destroy()
        {
            size_t count = vObjects.size();
            for (size_t i=0; i<count; ++i)
                unlink_widget(vObjects.at(i));

            vObjects.flush();
            vAxises.flush();
            vBasises.flush();
            vCenters.flush();

            if (pCanvas != NULL)
            {
                pCanvas->destroy();
                delete pCanvas;
                pCanvas = NULL;
            }
            if (pGlass != NULL)
            {
                pGlass->destroy();
                delete pGlass;
                pGlass = NULL;
            }
        }

        void LSPGraph::destroy()
        {
            do_destroy();
            LSPWidgetContainer::destroy();
        }

        ISurface *LSPGraph::get_canvas(ISurface *s, ssize_t w, ssize_t h)
        {
            // Check surface
            if (pCanvas != NULL)
            {
                if ((w != ssize_t(pCanvas->width())) || (h != ssize_t(pCanvas->height())))
                {
                    pCanvas->destroy();
                    delete pCanvas;
                    pCanvas    = NULL;
                }
            }

            // Create new surface if needed
            if (pCanvas == NULL)
            {
                if (s == NULL)
                    return NULL;
                pCanvas        = s->create(w, h);
                if (pCanvas == NULL)
                    return NULL;
            }

            // Clear canvas
            pCanvas->clear(sColor);

            // Draw all objects
            size_t n_objects = vObjects.size();
            for (size_t i=0; i<n_objects; ++i)
            {
                LSPGraphItem *obj = vObjects.at(i);
                if ((obj == NULL) || (!obj->visible()))
                    continue;
                obj->render(pCanvas, true);
                obj->commit_redraw();
            }

            return pCanvas;
        }

        LSPWidget *LSPGraph::find_widget(ssize_t x, ssize_t y)
        {
            size_t n = vObjects.size();
            for (size_t i=0; i<n; ++i)
            {
                LSPGraphItem *obj = vObjects.at(i);
                if (obj == NULL)
                    continue;
                if (obj->inside(x, y))
                    return obj;
            }
            return NULL;
        }

        bool LSPGraph::center(size_t index, float *x, float *y)
        {
            return center(vCenters.get(index), x, y);
        }

        bool LSPGraph::center(LSPCenter *c, float *x, float *y)
        {
            if ((c == NULL) || (pCanvas == NULL))
            {
                *x      = 0.0f;
                *y      = 0.0f;
                return false;
            }

            float c_width   = ssize_t(pCanvas->width()) - ssize_t(sIPadding.horizontal()) - 2;
            float c_height  = ssize_t(pCanvas->height()) - ssize_t(sIPadding.vertical()) - 2;

            *x  = area_left() + float(sIPadding.left()) + (c->canvas_left() + 1.0f) * c_width * 0.5f;
            *y  = area_bottom() + float(sIPadding.top()) + (1.0f - c->canvas_top()) * c_height * 0.5f;
            return true;
        }

        size_t LSPGraph::get_axes(LSPAxis **dst, size_t start, size_t count)
        {
            size_t n_count = vAxises.size();

            for (size_t i=0; i<count; ++i)
            {
                if (start >= n_count)
                    return i;
                dst[i]      = vAxises.at(start++);
            }

            return count;
        }

        size_t LSPGraph::get_basis_axes(LSPAxis **dst, size_t start, size_t count)
        {
            size_t n_count = vBasises.size();

            for (size_t i=0; i<count; ++i)
            {
                if (start >= n_count)
                    return i;
                dst[i]      = vBasises.at(start++);
            }

            return count;
        }

        size_t LSPGraph::get_items(LSPGraphItem **dst, size_t start, size_t count)
        {
            size_t n_count = vObjects.size();

            for (size_t i=0; i<count; ++i)
            {
                if (start >= n_count)
                    return i;
                dst[i]      = vObjects.at(start++);
            }

            return count;
        }

        void LSPGraph::set_min_width(size_t value)
        {
            if (nMinWidth == value)
                return;
            nMinWidth = value;
            query_resize();
        }

        void LSPGraph::set_min_height(size_t value)
        {
            if (nMinHeight == value)
                return;
            nMinHeight = value;
            query_resize();
        }

        void LSPGraph::set_border(size_t value)
        {
            if (nBorder == value)
                return;
            nBorder = value;
            query_resize();
        }

        void LSPGraph::set_radius(size_t value)
        {
            if (nRadius == value)
                return;
            nRadius = value;
            query_resize();
        }

        void LSPGraph::query_draw(size_t flags)
        {
            LSPWidgetContainer::query_draw(flags | REDRAW_SURFACE);
        }

        void LSPGraph::size_request(size_request_t *r)
        {
            r->nMinWidth    = nMinWidth + sIPadding.horizontal() + (nBorder << 1);
            r->nMinHeight   = nMinHeight + sIPadding.vertical() + (nBorder << 1);
            r->nMaxWidth    = -1;
            r->nMaxHeight   = -1;
        }

        status_t LSPGraph::add(LSPWidget *widget)
        {
            LSPGraphItem *obj = widget_cast<LSPGraphItem>(widget);
            if (obj == NULL)
                return STATUS_BAD_ARGUMENTS;

            obj->set_parent(this);
            vObjects.add(obj);

            LSPAxis *axis = widget_cast<LSPAxis>(widget);
            if (axis != NULL)
            {
                vAxises.add(axis);
                if (axis->is_basis())
                    vBasises.add(axis);
                return STATUS_OK;
            }

            LSPCenter *center = widget_cast<LSPCenter>(widget);
            if (center != NULL)
                vCenters.add(center);

            return STATUS_OK;
        }

        status_t LSPGraph::remove(LSPWidget *widget)
        {
            LSPGraphItem *obj = widget_cast<LSPGraphItem>(widget);
            if (obj == NULL)
                return STATUS_BAD_ARGUMENTS;

            if (!vObjects.remove(obj))
                return STATUS_NOT_FOUND;
            unlink_widget(obj);

            LSPAxis *axis = widget_cast<LSPAxis>(widget);
            if (axis != NULL)
            {
                vAxises.remove(axis);
                vBasises.remove(axis);
                return STATUS_OK;
            }

            LSPCenter *center = widget_cast<LSPCenter>(widget);
            if (center != NULL)
                vCenters.remove(center);

            return STATUS_OK;
        }

        status_t LSPGraph::on_mouse_down(const ws_event_t *e)
        {
            if (e->nType == UIE_MOUSE_DOWN)
                take_focus();
            return LSPWidgetContainer::on_mouse_down(e);
        }

        void LSPGraph::draw(ISurface *s)
        {
//            #ifdef LSP_TRACE
//            if (sClock.tv_sec == 0)
//                clock_gettime(CLOCK_REALTIME, &sClock);
//            #endif /* LSP_TRACE */

            // Draw background
            ssize_t pr = sqrtf(sSize.nWidth*sSize.nWidth + sSize.nHeight*sSize.nHeight);
            s->fill_frame(0, 0, sSize.nWidth, sSize.nHeight,
                    pr, pr, sSize.nWidth - 2*pr, sSize.nHeight - 2*pr,
                    sBgColor);

            size_t bw = nBorder;

            s->fill_round_rect(0, 0, sSize.nWidth, sSize.nHeight, nBorder, SURFMASK_ALL_CORNER, sColor);

            // Draw graph content
            size_t bs   = bw * M_SQRT2 * 0.5;
            ssize_t gw  = sSize.nWidth  - (bs << 1);
            ssize_t gh  = sSize.nHeight - (bs << 1);

            // Draw the internals
            ISurface *cv = get_canvas(s, gw, gh);
            if (cv != NULL)
                s->draw(cv, bs, bs);
            fCanvasLeft = sSize.nLeft + bs;
            fCanvasTop  = sSize.nTop + bs;

            // Draw the glass and the border
            cv = create_border_glass(s, &pGlass, sSize.nWidth, sSize.nHeight, nRadius, nBorder, SURFMASK_ALL_CORNER, sColor);
            if (cv != NULL)
                s->draw(cv, 0, 0);

//            #ifdef LSP_TRACE
//            nFrames ++;
//            struct timespec stime;
//            clock_gettime(CLOCK_REALTIME, &stime);
//            if ((stime.tv_sec - sClock.tv_sec) >= 5)
//            {
//                double dt = double(stime.tv_sec - sClock.tv_sec) + double(stime.tv_nsec - sClock.tv_nsec)*1e-9;
//                lsp_trace("seconds = %.2f, FPS = %.2f", dt, nFrames / dt);
//
//                nFrames = 0;
//                sClock = stime;
//            }
//            #endif /* LSP_TRACE */
        }
    } /* namespace tk */
} /* namespace lsp */
