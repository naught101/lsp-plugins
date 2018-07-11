/*
 * sse.cpp
 *
 *  Created on: 04 марта 2016 г.
 *      Author: sadko
 */

#include <core/types.h>
#include <core/debug.h>
#include <core/dsp.h>
#include <core/bits.h>

#include <core/x86/dsp.h>
#include <core/x86/dsp/search.h>
#include <core/x86/dsp/fft.h>
#include <core/x86/dsp/complex.h>
#include <core/x86/dsp/native.h>
#include <core/x86/dsp/vec4.h>

namespace lsp
{
    namespace sse
    {
        void dsp_init()
        {
            lsp_trace("Optimizing DSP for SSE instruction set");

            dsp::copy                       = sse::copy;
            dsp::move                       = sse::move;
            dsp::fill                       = sse::fill;
            dsp::fill_one                   = sse::fill_one;
            dsp::fill_zero                  = sse::fill_zero;
            dsp::fill_minus_one             = sse::fill_minus_one;
    //        dsp::abs                        = sse::abs;
    //        dsp::abs_normalized             = sse::abs_normalized;
            dsp::normalize                  = sse::normalize;
            dsp::min                        = sse::min;
            dsp::max                        = sse::max;
            dsp::abs_max                    = sse::abs_max;
            dsp::minmax                     = sse::minmax;
    //        dsp::min_index                  = sse::min_index;
    //        dsp::max_index                  = sse::max_index;
            dsp::scale                      = sse::scale;
            dsp::multiply                   = sse::multiply;
            dsp::h_sum                      = sse::h_sum;
            dsp::h_sqr_sum                  = sse::h_sqr_sum;
            dsp::h_abs_sum                  = sse::h_abs_sum;
    //        dsp::scalar_mul                 = sse::scalar_mul;
    //        dsp::accumulate                 = sse::accumulate;
//            dsp::add                        = sse::add;
//            dsp::sub                        = sse::sub;
            dsp::add_multiplied             = sse::add_multiplied;
            dsp::sub_multiplied             = sse::sub_multiplied;
            dsp::integrate                  = sse::integrate;
            dsp::mix                        = sse::mix;
            dsp::mix_add                    = sse::mix_add;
            dsp::convolve_single            = sse::convolve_single;
            dsp::convolve                   = sse::convolve;
            dsp::reverse                    = sse::reverse;
            dsp::direct_fft                 = sse::direct_fft;
            dsp::reverse_fft                = sse::reverse_fft;
//            dsp::center_fft                 = sse::center_fft;
//            dsp::combine_fft                = sse::combine_fft;
            dsp::normalize_fft              = sse::normalize_fft;
            dsp::complex_mul                = sse::complex_mul;
//            dsp::complex_cvt2modarg         = sse::complex_cvt2modarg;
//            dsp::complex_cvt2reim           = sse::complex_cvt2reim;
            dsp::complex_mod                = sse::complex_mod;
            dsp::lr_to_ms                   = sse::lr_to_ms;
            dsp::ms_to_lr                   = sse::ms_to_lr;
            dsp::biquad_process             = sse::biquad_process;
            dsp::biquad_process_multi       = sse::biquad_process_multi;

            dsp::vec4_scalar_mul            = sse::vec4_scalar_mul;
            dsp::vec4_push                  = sse::vec4_push;
            dsp::vec4_unshift               = sse::vec4_unshift;
            dsp::vec4_zero                  = sse::vec4_zero;
        }
    }

}
