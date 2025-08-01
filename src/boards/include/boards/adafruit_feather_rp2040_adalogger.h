/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

#ifndef _BOARDS_ADAFRUIT_FEATHER_RP2040_ADALOGGER_H
#define _BOARDS_ADAFRUIT_FEATHER_RP2040_ADALOGGER_H

pico_board_cmake_set(PICO_PLATFORM, rp2040)

// For board detection
#define ADAFRUIT_FEATHER_RP2040_ADALOGGER

// On some samples, the xosc can take longer to stabilize than is usual
#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
#endif

//------------- UART -------------//
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif

#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif

#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1
#endif

//------------- LED -------------//
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 13
#endif

#ifndef PICO_DEFAULT_WS2812_PIN
#define PICO_DEFAULT_WS2812_PIN 17
#endif

//------------- I2C -------------//
#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 1
#endif

#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 2
#endif

#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 3
#endif

//------------- SPI -------------//
#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 1
#endif

#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 15
#endif

#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 8
#endif

#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 14
#endif

//------------- SD -------------//
#ifndef PICO_SD_CARD_DETECT_PIN
#define PICO_SD_CARD_DETECT_PIN 16
#endif

#ifndef PICO_SD_CLK_PIN
#define PICO_SD_CLK_PIN 18
#endif

#ifndef PICO_SD_CMD_PIN
#define PICO_SD_CMD_PIN 19
#endif

#ifndef PICO_SD_DAT0_PIN
#define PICO_SD_DAT0_PIN 20
#endif

#ifndef PICO_SD_DAT_PIN_INCREMENT
#define PICO_SD_DAT_PIN_INCREMENT 1
#endif

#ifndef PICO_SD_DAT_PIN_COUNT
#define PICO_SD_DAT_PIN_COUNT 4
#endif

//------------- FLASH -------------//

// Use slower generic flash access
#define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 4
#endif

pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (8 * 1024 * 1024))
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (8 * 1024 * 1024)
#endif
// All boards have B1 RP2040
#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 0
#endif

#endif
