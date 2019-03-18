/*
 * bswap.h
 *
 *  Created on: 18 мар. 2019 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_AARCH64_BSWAP_H_
#define DSP_ARCH_AARCH64_BSWAP_H_

inline uint16_t __lsp_forced_inline    byte_swap(uint16_t v)
{
    ARCH_AARCH64_ASM (
        __ASM_EMIT("rev16   %[v], %[v]")
        : [v] "+r"(v)
        : :
    );
    return v;
}

inline uint32_t __lsp_forced_inline    byte_swap(uint32_t v)
{
    ARCH_AARCH64_ASM (
        __ASM_EMIT("rev32   %[v], %[v]")
        : [v] "+r"(v)
        : :
    );
    return v;
}

inline uint64_t __lsp_forced_inline    byte_swap(uint64_t v)
{
    ARCH_AARCH64_ASM (
        __ASM_EMIT("rev    %[v], %[v]")
        : [v] "+r"(v)
        : :
    );
    return v;
}

inline float __lsp_forced_inline    byte_swap(float v)
{
    ARCH_AARCH64_ASM (
        __ASM_EMIT("rev32   %[v], %[v]")
        : [v] "+r"(v)
        : :
    );
    return v;
}

inline double __lsp_forced_inline    byte_swap(double v)
{
    ARCH_AARCH64_ASM (
        __ASM_EMIT("rev     %[v], %[v]")
        : [v] "+r"(v)
        : :
    );
    return v;
}

inline void __lsp_forced_inline    byte_swap(uint16_t *v, size_t n)
{
    uint64_t xtmp;
    uint32_t wtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("subs        %[n], $4")
        __ASM_EMIT("b.lo        2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev16       %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $4")
        __ASM_EMIT("b.ge        1b")

        __ASM_EMIT("2:")
        __ASM_EMIT("adds        %[n], $3")
        __ASM_EMIT("b.lt        4f")

        __ASM_EMIT("3:")
        __ASM_EMIT("ldrh        %[wtmp], [%[v]]")
        __ASM_EMIT("rev16       %[wtmp], %[wtmp]")
        __ASM_EMIT("strh        %[wtmp], [%[v]], $2")
        __ASM_EMIT("subs        %[n], $1")
        __ASM_EMIT("b.ge        3b")

        __ASM_EMIT("4:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp), [wtmp] "=&r"(wtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(int16_t *v, size_t n)
{
    uint64_t xtmp;
    uint32_t wtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("subs        %[n], $4")
        __ASM_EMIT("b.lo        2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev16       %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $4")
        __ASM_EMIT("b.ge        1b")

        __ASM_EMIT("2:")
        __ASM_EMIT("adds        %[n], $3")
        __ASM_EMIT("b.lt        4f")

        __ASM_EMIT("3:")
        __ASM_EMIT("ldrh        %[wtmp], [%[v]]")
        __ASM_EMIT("rev16       %[wtmp], %[wtmp]")
        __ASM_EMIT("strh        %[wtmp], [%[v]], $2")
        __ASM_EMIT("subs        %[n], $1")
        __ASM_EMIT("b.ge        3b")

        __ASM_EMIT("4:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp), [wtmp] "=&r"(wtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(uint32_t *v, size_t n)
{
    uint64_t xtmp;
    uint32_t wtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.lo        2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev32       %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.ge        1b")

        __ASM_EMIT("2:")
        __ASM_EMIT("adds        %[n], $1")
        __ASM_EMIT("b.lt        4f")
        __ASM_EMIT("ldr         %[wtmp], [%[v]]")
        __ASM_EMIT("rev32       %[wtmp], %[wtmp]")
        __ASM_EMIT("str         %[wtmp], [%[v]], $2")

        __ASM_EMIT("4:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp), [wtmp] "=&r"(wtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(int32_t *v, size_t n)
{
    uint64_t xtmp;
    uint32_t wtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.lo        2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev32       %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.ge        1b")

        __ASM_EMIT("2:")
        __ASM_EMIT("adds        %[n], $1")
        __ASM_EMIT("b.lt        4f")
        __ASM_EMIT("ldr         %[wtmp], [%[v]]")
        __ASM_EMIT("rev32       %[wtmp], %[wtmp]")
        __ASM_EMIT("str         %[wtmp], [%[v]], $2")

        __ASM_EMIT("4:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp), [wtmp] "=&r"(wtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(float *v, size_t n)
{
    uint64_t xtmp;
    uint32_t wtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.lo        2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev32       %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $2")
        __ASM_EMIT("b.ge        1b")

        __ASM_EMIT("2:")
        __ASM_EMIT("adds        %[n], $1")
        __ASM_EMIT("b.lt        4f")
        __ASM_EMIT("ldr         %[wtmp], [%[v]]")
        __ASM_EMIT("rev32       %[wtmp], %[wtmp]")
        __ASM_EMIT("str         %[wtmp], [%[v]], $2")

        __ASM_EMIT("4:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp), [wtmp] "=&r"(wtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(uint64_t *v, size_t n)
{
    uint64_t xtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("cbz         %[n], 2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev         %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $1")
        __ASM_EMIT("b.ne        1b")

        __ASM_EMIT("2:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(int64_t *v, size_t n)
{
    uint64_t xtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("cbz         %[n], 2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev         %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $1")
        __ASM_EMIT("b.ne        1b")

        __ASM_EMIT("2:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp)
        : : "cc", "memory"
    );
}

inline void __lsp_forced_inline    byte_swap(double *v, size_t n)
{
    uint64_t xtmp;

    ARCH_AARCH64_ASM (
        __ASM_EMIT("cbz         %[n], 2f")

        __ASM_EMIT("1:")
        __ASM_EMIT("ldr         %[xtmp], [%[v]]")
        __ASM_EMIT("rev         %[xtmp], %[xtmp]")
        __ASM_EMIT("str         %[xtmp], [%[v]], $8")
        __ASM_EMIT("subs        %[n], $1")
        __ASM_EMIT("b.ne        1b")

        __ASM_EMIT("2:")
        : [v] "+r"(v), [n] "+r" (n), [xtmp] "=&r"(xtmp)
        : : "cc", "memory"
    );
}

#endif /* DSP_ARCH_AARCH64_BSWAP_H_ */
