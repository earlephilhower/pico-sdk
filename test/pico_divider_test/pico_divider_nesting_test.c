/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/sync.h"

volatile bool failed;
volatile uint32_t count[3];
volatile bool done;

#define FAILED() ({ failed = true; })
//#define FAILED() ({ failed = true; __breakpoint(); })

//#define DOUBLE_ONLY 1

bool timer_callback(repeating_timer_t *t) {
    count[0]++;
    static int z;
#if !DOUBLE_ONLY
    for (int i=0; i<100;i++) {
        z += 23;
        int a = z / 7;
        int b = z % 7;
        if (z != a * 7 + b) {
            FAILED();
        }
        a = z / -7;
        b = z % -7;
        if (z != a * -7 + b) {
            FAILED();
        }
    }
#else
    for(int i=0;i<10;i++) {
#endif
    float fz = (float) z;
    float fa = fz / 11.0f;
//    float fb = fmodf(fz, 11.0f);
//    if (fabs(fz - (fa * 11.0f + fb)) > 1e-9f) {
//        FAILED();
//    }
    union {
        float f;
        uint32_t i;
    } fi, fi2;
    fi.f = fabsf(fz - fa * 11.0f);
    // make a float which is close to 1ulp
    fi2.i = fi.i & 0x7f800000u;
    fi2.i++;
    if (fi.f > fi2.f) {
        FAILED();
    }
    double dz = z;
    double da = dz / 11.0;
//    double db = fmod(dz, 11.0);
//    if (abs(dz - (da * 11.0 + db)) > 1e-9) {
//        FAILED();
//    }
    if (fabs(dz - da * 11.0) > 1e-6f) {
        FAILED();
    }
#if DOUBLE_ONLY
    }
#endif
    return !done;
}

void do_dma_start(uint ch) {
    static uint32_t word[2];
    assert(ch < 2);
    dma_channel_config c = dma_channel_get_default_config(ch);
    // todo remove this; landing in a separate PR
#ifndef DREQ_DMA_TIMER0
#define DREQ_DMA_TIMER0 0x3b
#endif
    channel_config_set_dreq(&c, DREQ_DMA_TIMER0);
    dma_channel_configure(ch, &c, &word[ch], &word[ch], 513 + ch * 23, true);
}

double d0c, d0s, d0t, dz;
float f0c, f0s, f0t, fz;

double flarn = 25.5;
double zzd = 13.3;

void test_irq_handler0() {
    count[1]++;
    dma_hw->ints0 |= 1u;
    static uint z;
    static uint dz;
    for (int i=0; i<80;i++) {
#if !DOUBLE_ONLY
        z += 31;
        uint a = z / 11;
        uint b = z % 11;
        if (z != a * 11 + b) {
            FAILED();
        }
#else
        zzd += flarn/(flarn + 1.35);
        break;
#endif
    }
    if (done) dma_channel_abort(0);
    else      do_dma_start(0);
}

void test_irq_handler1() {
    static uint z;
    dma_hw->ints1 |= 2u;
    count[2]++;
    for (int i=0; i<130;i++) {
#if !DOUBLE_ONLY
        z += 47;
        uint a = z / -13;
        uint b = z % -13;
        if (z != a * -13 + b) {
            FAILED();
        }
        static uint64_t z64;
        z64 -= 47;
        uint64_t a64 = z64 / -13;
        uint64_t b64 = z64 % -13;
        if (z64 != a64 * -13 + b64) {
            FAILED();
        }
#else
        zzd += flarn/123.3;
        break;
#endif
    }

    if (done) dma_channel_abort(1);
    else      do_dma_start(1);
}

void test_nesting() {
    uint z = 0;

    // We have 3 different IRQ handlers, one for timer, two for DMA completion (on DMA_IRQ0/1)
    // thus we expect re-entrancy even between IRQs
    //
    // They all busily make use of the dividers, to expose any issues with nested use

    repeating_timer_t timer;
    add_repeating_timer_us(929, timer_callback, NULL, &timer);
    irq_set_exclusive_handler(DMA_IRQ_0, test_irq_handler0);
    irq_set_exclusive_handler(DMA_IRQ_1, test_irq_handler1);

    dma_set_irq0_channel_mask_enabled(1u, true);
    dma_set_irq1_channel_mask_enabled(2u, true);
    dma_hw->timer[0] = (1 << 16) | 32; // run at 1/32 system clock

    irq_set_enabled(DMA_IRQ_0, 1);
    irq_set_enabled(DMA_IRQ_1, 1);
    do_dma_start(0);
    do_dma_start(1);
    absolute_time_t end = delayed_by_ms(get_absolute_time(), 10000);
    int count_local=0;
    while (!time_reached(end)) {
        for(uint i=0;i<100;i++) {
            z += 31;
            uint a = z / 11;
            uint b = z % 11;
            if (z != a * 11 + b) {
                FAILED();
            }
            int zz = (int)z;
            int aa = zz / -11;
            int bb = zz % -11;
            if (zz != aa * -11 + bb) {
                FAILED();
            }
            aa = -zz / -11;
            bb = -zz % -11;
            if (-zz != aa * -11 + bb) {
                FAILED();
            }
            aa = -zz / 11;
            bb = -zz % 11;
            if (-zz != aa * 11 + bb) {
                FAILED();
            }
            a = 0xffffffffu / 11;
            b = 0xffffffffu % 11;
            if (0xffffffffu != a * 11 + b) {
                FAILED();
            }
            static uint64_t z64;
            z64 -= 47;
            uint64_t a64 = z64 / -13635;
            uint64_t b64 = z64 % -13635;
            if (z64 != a64 * -13635 + b64) {
                FAILED();
            }
            // specifically check 64/32 divide
            static uint64_t c64 = 0x13ffffffffull;
            static uint32_t cd = 1;
            a64 = c64 / cd;
            b64 = c64 % cd;
            if (c64 != a64 * cd + b64) {
                FAILED();
            }
            cd++;
            static float zf = 1.f;
            float ff = zf / -13635.f;
            if (fabsf(zf - ff * -13635.f) > 1e-2f) {
                FAILED();
            }
            zf += 0.0331f;
            z += (int)ff;
            static double zd = 1.0;
            double dd = zd / -13635.0;
            if (fabs(zd - dd * -13635.0) > 1e-6) {
                FAILED();
            }
            zd += 0.331;
            z += (int)dd;

        }
        // todo this still seems broken on RP2350
#if PICO_RP2040
        // these use the divider
        for(uint i=0;i<=100;i+=20) {
            // both in and out bootrom range (we perform mod in wrapper code if necessarry)
            f0t = tanf(i * 50);
            f0c = cosf(i * 50);
            f0s = sinf(i * 50);
            d0t = tan(i * 1000);
            d0c = cos(i * 1000);
            d0s = sin(i * 1000);
        }
#endif
        count_local++;
    }
    done = true;
    cancel_repeating_timer(&timer);
    printf("%d: %d %d %d\n", count_local, (int)count[0], (int)count[1], (int)count[2]);
    printf("%d\n", z);
    // make sure all the IRQs ran
    if (!(count_local && count[0] && count[1] && count[2])) {
        printf("DID NOT RUN\n");
        exit(1);
    }
    if (failed) {
        printf("FAILED\n");
        exit(1);
    }
}

int main() {
#ifndef uart_default
#warning test/pico_divider requires a default uart
#else
    stdio_init_all();
#endif
    test_nesting();
    printf("PASSED\n");
    return 0;
}
