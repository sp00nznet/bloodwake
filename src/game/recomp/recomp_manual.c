/**
 * Blood Wake - Manual Function Overrides
 *
 * Hand-written replacements for recompiled functions that:
 * - Access hardware we haven't emulated yet
 * - Have bugs in the mechanical translation
 * - Need tracing/debugging instrumentation
 *
 * Each override replaces a function at a specific Xbox VA.
 * Register it in recomp_lookup_manual() below.
 */

#include <stdio.h>
#include <stdint.h>

typedef void (*recomp_func_t)(void);

extern uint32_t g_eax, g_ecx, g_edx, g_esp;
extern uint32_t g_ebx, g_esi, g_edi;
extern ptrdiff_t g_xbox_mem_offset;

/* ── Manual override lookup ─────────────────────────────── */

/**
 * Look up a manually overridden function by Xbox VA.
 * Returns NULL if no override exists for this address.
 *
 * Add overrides here as you discover functions that need
 * hand-written replacements during debugging.
 *
 * Example:
 *   if (xbox_va == 0x00012345) return my_override_function;
 */
recomp_func_t recomp_lookup_manual(uint32_t xbox_va)
{
    /* No overrides yet - add them as needed during debugging */
    (void)xbox_va;
    return (recomp_func_t)0;
}
