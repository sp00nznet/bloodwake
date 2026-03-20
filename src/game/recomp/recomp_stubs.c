/**
 * Recompilation runtime stubs
 *
 * Provides the runtime support functions needed by generated code:
 * - recomp_lookup_kernel: kernel bridge dispatch
 * - ICALL trace ring buffer globals
 */

#include <stdio.h>
#include <stdint.h>

/* ICALL trace ring buffer */
volatile uint32_t g_icall_trace[16] = {0};
volatile uint32_t g_icall_trace_idx = 0;
volatile uint64_t g_icall_count = 0;

typedef void (*recomp_func_t)(void);

extern uint32_t g_eax;
extern ptrdiff_t g_xbox_mem_offset;

/* Log failed indirect call resolution */
void recomp_icall_fail_log(uint32_t va)
{
    fprintf(stderr, "[ICALL] Failed to resolve VA 0x%08X (total calls: %llu)\n",
            va, (unsigned long long)g_icall_count);

    /* Dump last 16 call targets */
    fprintf(stderr, "  Recent ICALL targets:\n");
    for (int i = 0; i < 16; i++) {
        int idx = (g_icall_trace_idx - 16 + i) & 15;
        if (g_icall_trace[idx])
            fprintf(stderr, "    [%2d] 0x%08X\n", i, g_icall_trace[idx]);
    }
}
