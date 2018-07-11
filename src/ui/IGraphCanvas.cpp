/*
 * IGraphCanvas.cpp
 *
 *  Created on: 27 нояб. 2015 г.
 *      Author: sadko
 */

#include <ui/ui.h>

namespace lsp
{
    IGraphCanvas::IGraphCanvas(IGraph *graph)
    {
        nWidth          = 0;
        nHeight         = 0;
        nLeft           = 0;
        nTop            = 0;
        pGraph          = graph;
        bAntiAliasing   = false;
        sColor.set_rgb(1.0, 1.0, 1.0);
    }

    void IGraphCanvas::set_size(ssize_t width, ssize_t height)
    {
        nWidth          = width;
        nHeight         = height;
        nLeft           = - (nWidth >> 1);
        nTop            = (nHeight >> 1);
    }

    IGraphCanvas::~IGraphCanvas()
    {
    }

    void IGraphCanvas::line(ssize_t x1, ssize_t y1, ssize_t x2, ssize_t y2)
    {
        move_to(x1, y1);
        line_to(x2, y2);
        stroke();
    }

    void IGraphCanvas::line(float a, float b, float c)
    {
        if (fabs(a) > fabs(b))
            line(-(c + b*nTop) / a, nTop, -(c - b *nTop)/ a, -nTop);
        else
            line(nLeft, -(c + a*nLeft) / b, -nLeft, -(c - a *nLeft)/ b);
    }

    void IGraphCanvas::move_to(ssize_t x, ssize_t y)
    {
    }

    void IGraphCanvas::line_to(ssize_t x, ssize_t y)
    {
    }

    void IGraphCanvas::draw_lines(float *x, float *y, size_t count)
    {
    }

    void IGraphCanvas::draw_poly(float *x, float *y, size_t count, const Color &stroke, const Color &fill)
    {
    }

    void IGraphCanvas::stroke()
    {
    }

    void IGraphCanvas::set_color(const Color &c)
    {
        sColor.copy(c);
    }

    void IGraphCanvas::set_color_rgb(float r, float g, float b)
    {
        sColor.set_rgb(r, g, b);
    }

    void IGraphCanvas::set_line_width(size_t width)
    {
    }

    void IGraphCanvas::clear()
    {
    }

    void IGraphCanvas::center(size_t id, float *x, float *y)
    {
        return center(pGraph->center(id), x, y);
    }

    void IGraphCanvas::center(Center *c, float *x, float *y)
    {
        if (c == NULL)
        {
            *x      = 0.0f;
            *y      = 0.0f;
            return;
        }

        *x  = c->left() * (nWidth >> 1);
        *y  = c->top() * (nHeight >> 1);
    }

    void IGraphCanvas::circle(ssize_t x, ssize_t y, ssize_t r)
    {
    }

    void IGraphCanvas::radial_gradient(ssize_t x, ssize_t y, const Color &c1, const Color &c2, ssize_t r)
    {
    }

    void IGraphCanvas::out_text(ssize_t x, ssize_t y, float h_pos, float v_pos, float size, const char *text)
    {
    }

    bool IGraphCanvas::set_anti_aliasing(bool enable)
    {
        bool old = bAntiAliasing;
        bAntiAliasing = enable;
        return old;
    }
} /* namespace lsp */
