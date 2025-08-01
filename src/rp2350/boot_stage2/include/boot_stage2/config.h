/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOOT_STAGE2_CONFIG_H_
#define _BOOT_STAGE2_CONFIG_H_

// NOTE THIS HEADER IS INCLUDED FROM ASSEMBLY

#include "pico.h"

// PICO_CONFIG: PICO_FLASH_SPI_CLKDIV, Clock divider from clk_sys to use for serial flash communications in boot stage 2. On RP2040 this must be a multiple of 2. This define applies to compilation of the boot stage 2 not the main application, type=int, default=varies; often specified in board header, advanced=true, group=boot_stage2
// PICO_CONFIG: PICO_FLASH_SPI_RXDELAY, Receive delay in 1/2 clock cycles to use for serial flash communications in boot stage 2. This define applies to compilation of the boot stage 2 not the main application, type=int, default=varies; often specified in board header, advanced=true, group=boot_stage2

// PICO_CONFIG: PICO_BUILD_BOOT_STAGE2_NAME, Name of the boot stage 2 if selected in the build system. This define applies to compilation of the boot stage 2 not the main application, group=boot_stage2
#ifdef PICO_BUILD_BOOT_STAGE2_NAME
    #define _BOOT_STAGE2_SELECTED
#else
    // check that multiple boot stage 2 options haven't been set...

// PICO_CONFIG: PICO_BOOT_STAGE2_CHOOSE_IS25LP080, Select boot2_is25lp080 as the boot stage 2 when no boot stage 2 selection is made by the CMake build. This define applies to compilation of the boot stage 2 not the main application, type=bool, default=0, group=boot_stage2
#ifndef PICO_BOOT_STAGE2_CHOOSE_IS25LP080
    #define PICO_BOOT_STAGE2_CHOOSE_IS25LP080 0
#elif PICO_BOOT_STAGE2_CHOOSE_IS25LP080
    #ifdef _BOOT_STAGE2_SELECTED
        #error multiple boot stage 2 options chosen
    #endif
    #define _BOOT_STAGE2_SELECTED
#endif
// PICO_CONFIG: PICO_BOOT_STAGE2_CHOOSE_W25Q080, Select boot2_w25q080 as the boot stage 2 when no boot stage 2 selection is made by the CMake build. This define applies to compilation of the boot stage 2 not the main application, type=bool, default=0, group=boot_stage2
#ifndef PICO_BOOT_STAGE2_CHOOSE_W25Q080
    #define PICO_BOOT_STAGE2_CHOOSE_W25Q080 0
#elif PICO_BOOT_STAGE2_CHOOSE_W25Q080
    #ifdef _BOOT_STAGE2_SELECTED
        #error multiple boot stage 2 options chosen
    #endif
    #define _BOOT_STAGE2_SELECTED
#endif
// PICO_CONFIG: PICO_BOOT_STAGE2_CHOOSE_W25X10CL, Select boot2_w25x10cl as the boot stage 2 when no boot stage 2 selection is made by the CMake build. This define applies to compilation of the boot stage 2 not the main application, type=bool, default=0, group=boot_stage2
#ifndef PICO_BOOT_STAGE2_CHOOSE_W25X10CL
    #define PICO_BOOT_STAGE2_CHOOSE_W25X10CL 0
#elif PICO_BOOT_STAGE2_CHOOSE_W25X10CL
    #ifdef _BOOT_STAGE2_SELECTED
        #error multiple boot stage 2 options chosen
    #endif
    #define _BOOT_STAGE2_SELECTED
#endif
// PICO_CONFIG: PICO_BOOT_STAGE2_CHOOSE_AT25SF128A, Select boot2_at25sf128a as the boot stage 2 when no boot stage 2 selection is made by the CMake build. This define applies to compilation of the boot stage 2 not the main application, type=bool, default=0, group=boot_stage2
#ifndef PICO_BOOT_STAGE2_CHOOSE_AT25SF128A
    #define PICO_BOOT_STAGE2_CHOOSE_AT25SF128A 0
#elif PICO_BOOT_STAGE2_CHOOSE_AT25SF128A
    #ifdef _BOOT_STAGE2_SELECTED
        #error multiple boot stage 2 options chosen
    #endif
    #define _BOOT_STAGE2_SELECTED
#endif

// PICO_CONFIG: PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H, Select boot2_generic_03h as the boot stage 2 when no boot stage 2 selection is made by the CMake build. This define applies to compilation of the boot stage 2 not the main application, type=bool, default=1, group=boot_stage2
#if defined(PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H) && PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H
    #ifdef _BOOT_STAGE2_SELECTED
        #error multiple boot stage 2 options chosen
    #endif
    #define _BOOT_STAGE2_SELECTED
#endif

#endif // PICO_BUILD_BOOT_STAGE2_NAME

#ifdef PICO_BUILD_BOOT_STAGE2_NAME
    // boot stage 2 is configured by cmake, so use the name specified there
    #define PICO_BOOT_STAGE2_NAME PICO_BUILD_BOOT_STAGE2_NAME
#else
    // boot stage 2 is selected by board config header, so we have to do some work
    #if PICO_BOOT_STAGE2_CHOOSE_IS25LP080
        #error "IS25LP080 boot2 is broken (FIXME)"
        #define _BOOT_STAGE2 boot2_is25lp080
    #elif PICO_BOOT_STAGE2_CHOOSE_W25Q080
        #define _BOOT_STAGE2 boot2_w25q080
    #elif PICO_BOOT_STAGE2_CHOOSE_W25X10CL
        #error "W25X10CL boot2 is broken (FIXME)"
        #define _BOOT_STAGE2 boot2_w25x10cl
    #elif PICO_BOOT_STAGE2_CHOOSE_AT25SF128A
        #error "AT25SF128A boot2 is broken (FIXME)"
        #define _BOOT_STAGE2 boot2_at25sf128a
    #elif !defined(PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H) || PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H
        #undef PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H
        #define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H 1
        #define _BOOT_STAGE2 boot2_generic_03h
    #else
        #error no boot stage 2 is defined by PICO_BOOT_STAGE2_CHOOSE_ macro
    #endif
    // we can't include cdefs in assembly, so define our own, but avoid conflict with real ones for c inclusion
    #define _PICO__STRING(x) #x
    #define _PICO__XSTRING(x) _PICO__STRING(x)
    #define _PICO__CONCAT1(x, y) x ## y
    #define PICO_BOOT_STAGE2_NAME _PICO__XSTRING(_BOOT_STAGE2)
    #define PICO_BOOT_STAGE2_ASM _PICO__XSTRING(_PICO__CONCAT1(_BOOT_STAGE2,.S))
#endif
#endif
