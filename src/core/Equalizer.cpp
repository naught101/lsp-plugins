/*
 * Equalizer.cpp
 *
 *  Created on: 26 июля 2016 г.
 *      Author: sadko
 */

#include <core/dsp.h>
#include <core/windows.h>
#include <core/Equalizer.h>
#include <core/debug.h>

namespace lsp
{
    Equalizer::Equalizer()
    {
        vFilters        = NULL;
        nFilters        = 0;
        nSampleRate     = 0;
        nConvSize       = 0;
        nFftRank        = 0;
        nLatency        = 0;
        nBufSize        = 0;
        nActiveFilters  = 0;
        nMode           = EQM_BYPASS;
        vFftRe          = NULL;
        vFftIm          = NULL;
        vConvRe         = NULL;
        vConvIm         = NULL;
        vBuffer         = NULL;
        vTmp            = NULL;
        pData           = NULL;
    }

    Equalizer::~Equalizer()
    {
        destroy();
    }

    bool Equalizer::init(size_t filters, size_t conv_rank)
    {
        destroy();
        nSampleRate     = 0;
        vFilters        = new Filter[filters];
        if (vFilters == NULL)
        {
            destroy();
            return false;
        }
        nFilters        = filters;

        // Allocate buffers
        nConvSize           = 1 << conv_rank;
        nFftRank            = conv_rank;
        size_t conv_size    = nConvSize * 2;
        size_t allocate     = conv_size * 6; // fft + conv*2 + buffer + tmp*2
        pData               = new float[allocate];
        if (pData == NULL)
        {
            destroy();
            return false;
        }

        dsp::fill_zero(pData, allocate);

        // Assign pointers
        float *ptr          = pData;
        vFftRe              = ptr;
        ptr                += conv_size;
        vFftIm              = ptr;
        ptr                += conv_size;
        vConvRe             = ptr;
        ptr                += conv_size;
        vConvIm             = ptr;
        ptr                += conv_size;
        vBuffer             = ptr;
        ptr                += conv_size;
        vTmp                = ptr;
        ptr                += conv_size;

        // Initialize filters
        for (size_t i=0; i<filters; ++i)
        {
            if (!vFilters[i].init())
            {
                destroy();
                return false;
            }
        }

        return true;
    }

    void Equalizer::destroy()
    {
        if (vFilters != NULL)
        {
            for (size_t i=0; i<nFilters; ++i)
                vFilters[i].destroy();
            delete [] vFilters;
            vFilters        = NULL;
            nFilters        = 0;
        }

        if (pData != NULL)
        {
            delete [] pData;
            vFftRe          = NULL;
            vFftIm          = NULL;
            vConvRe         = NULL;
            vConvIm         = NULL;
            vBuffer         = NULL;
            vTmp            = NULL;
            pData           = NULL;
        }
    }

    void Equalizer::set_sample_rate(size_t sr)
    {
        if (nSampleRate == sr)
            return;

        filter_params_t fp;
        nSampleRate     = sr;

        for (size_t i=0; i<nFilters; ++i)
        {
            vFilters[i].get_params(&fp);
            vFilters[i].update(nSampleRate, &fp);
        }
    }

    bool Equalizer::set_params(size_t id, const filter_params_t *params)
    {
        if (id >= nFilters)
            return false;

        Filter *f = &vFilters[id];
        if (f->active())
            nActiveFilters--;
        f->update(nSampleRate, params);
        if (f->active())
            nActiveFilters++;
        return true;
    }

    bool Equalizer::get_params(size_t id, filter_params_t *params)
    {
        if (id >= nFilters)
            return false;
        vFilters[id].get_params(params);
        return true;
    }

    void Equalizer::reconfigure()
    {
        if ((nMode == EQM_IIR) || (nActiveFilters <= 0))
            return;

        size_t conv_len     = nConvSize << 1;
        size_t half_size    = nConvSize >> 1;
        float *conv_re      = vConvRe;
        float *conv_im      = vConvIm;

        // Backup input buffer
        dsp::copy(vTmp, vFftRe, conv_len);

        // Init convolution
        dsp::fill_one(conv_re, nConvSize);

        if (nMode == EQM_FIR)
        {
            // Clear buffers
            windows::window(conv_im, nConvSize*2, windows::BLACKMAN_NUTTALL);

            // Get the FIR responses of all filters and convolve them
            for (size_t i=0; i<nFilters; ++i)
            {
                if (vFilters[i].inactive())
                    continue;

                // Get the impulse response of the filter
                vFilters[i].impulse_response(vFftRe, nConvSize);
                dsp::fill_zero(vFftIm, nConvSize);
                dsp::multiply(vFftRe, vFftRe, &conv_im[nConvSize], nConvSize);  // Apply window function to the impulse response

                // Do the FFT of the impulse response
                dsp::direct_fft(vFftRe, vFftIm, vFftRe, vFftIm, nFftRank);
                dsp::complex_mod(vFftRe, vFftRe, vFftIm, nConvSize);
                dsp::multiply(conv_re, conv_re, vFftRe, nConvSize);             // Apply the frequency chart relative to the IR
            }
        }
        else if (nMode == EQM_FFT)
        {
            // Initialize frequencies
            ssize_t n_freqs         = nConvSize >> 1;
            float kf                = float(nSampleRate) / nConvSize;
            for (ssize_t i=0; i<=n_freqs; ++i)
                conv_im[i]              = i * kf;

            // Build frequency chart for all filters
            for (size_t i=0; i<nFilters; ++i)
            {
                if (vFilters[i].inactive())
                    continue;

                // Get the frequency chart of the filter
                vFilters[i].freq_chart(vFftRe, vFftIm, conv_im, n_freqs+1);
                dsp::complex_mod(vFftRe, vFftRe, vFftIm, n_freqs+1);
                dsp::multiply(conv_re, conv_re, vFftRe, n_freqs+1);
            }

            // Finally, build the correct frequency chart for reverse FFT
            float *tail_re          = &conv_re[nConvSize];
            for (ssize_t i=1; i<n_freqs; ++i)
                tail_re[-i]     = conv_re[i];
        }

        // Now we have the frequency chart in conv_re
        dsp::fill_zero(conv_im, nConvSize);
        dsp::reverse_fft(vFftRe, vFftIm, conv_re, conv_im, nFftRank);   // Apply reversive FFT transform to get impulse response

        dsp::copy(vFftIm, &vFftRe[half_size], half_size);               // Make the impulse response symmetric
        dsp::copy(&vFftIm[half_size], vFftRe, half_size);

        windows::window(conv_im, nConvSize, windows::BLACKMAN_NUTTALL);
        dsp::multiply(vFftRe, vFftIm, conv_im, nConvSize);              // Apply window to the impulse response

        // Get the final convolution spectrum
        dsp::fill_zero(&vFftRe[nConvSize], nConvSize);
        dsp::fill_zero(vFftIm, conv_len);
        dsp::direct_fft(conv_re, conv_im, vFftRe, vFftIm, nFftRank+1);

        // Restore input buffer
        dsp::copy(vFftRe, vTmp, conv_len);
        nLatency    = nConvSize;
    }

    void Equalizer::set_mode(equalizer_mode_t mode)
    {
        if (mode == nMode)
            return;
        nMode   = mode;
        reconfigure();
    }

    bool Equalizer::freq_chart(size_t id, float *re, float *im, const float *f, size_t count)
    {
        if (id >= nFilters)
            return false;
        vFilters[id].freq_chart(re, im, f, count);
        return true;
    }

    void Equalizer::process(float *out, const float *in, size_t samples)
    {
        switch (nMode)
        {
            case EQM_BYPASS:
            {
                dsp::copy(out, in, samples);
                break;
            }

            case EQM_IIR:
            {
                const float *src = in;
                for (size_t i=0; i<nFilters; ++i)
                {
                    if (!vFilters[i].inactive())
                    {
                        vFilters[i].process(out, src, samples);
                        src     = out; // Actual data is in output buffer
                    }
                }
                break;
            }

            case EQM_FIR:
            case EQM_FFT:
            default:
            {
                while (samples > 0)
                {
                    if (nBufSize >= nConvSize)
                    {
                        size_t conv_len = nConvSize * 2;

                        dsp::fill_zero(&vFftRe[nConvSize], nConvSize);

                        if (nActiveFilters > 0)
                        {
                            // Perform the direct FFT of the input signal
                            dsp::fill_zero(vFftIm, conv_len);
                            dsp::direct_fft(vFftRe, vFftIm, vFftRe, vFftIm, nFftRank + 1);

                            // Perform convolution
                            dsp::complex_mul(vFftRe, vFftIm, vFftRe, vFftIm, vConvRe, vConvIm, conv_len);

                            // Perform the reverse FFT
                            dsp::reverse_fft(vFftRe, vFftIm, vFftRe, vFftIm, nFftRank + 1);
                        }

                        // Apply previous convolution tail
                        dsp::add(vFftRe, &vBuffer[nConvSize], nConvSize);

                        // Update the buffer
                        dsp::copy(vBuffer, vFftRe, conv_len);

                        // Reset the buffer size
                        nBufSize    = 0;
                    }

                    // Determine number of samples to process
                    size_t to_process = nConvSize - nBufSize;
                    if (to_process > samples)
                        to_process      = samples;

                    // Push new data for processing and emit processed data
                    dsp::copy(&vFftRe[nBufSize], in, to_process);
                    dsp::copy(out, &vBuffer[nBufSize], to_process);

                    // Update pointers and counters
                    nBufSize       += to_process;
                    out            += to_process;
                    in             += to_process;
                    samples        -= to_process;
                }

                break;
            }
        }
    }

} /* namespace lsp */
