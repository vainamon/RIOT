/*
 * Copyright (C) 2019 Supercomputers and Neurocomputers Research Center
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_stm32f4xx
 * @{
 *
 * @file
 * @brief       Board specific definitions for the China STM32F4XX evaluation board
 *
 * @author      Danilov Igor <danilov@superevm.ru>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name xtimer configuration
 * @{
 */
#define XTIMER_OVERHEAD     (6)
#define XTIMER_BACKOFF      (10)
/** @} */

/**
 * @name LED pin definitions
 * @{
 */
/** @} */

/**
 * @name Macros for controlling the on-board LEDs.
 * @{
 */
#define LED0_PIN            GPIO_PIN(PORT_A, 6)
#define LED1_PIN            GPIO_PIN(PORT_A, 7)

#define LED_PORT            GPIOA
#define LED0_MASK           (1 << 6)
#define LED1_MASK           (1 << 7)

#define LED0_ON             (LED_PORT->BSRR = LED0_MASK)
#define LED0_OFF            (LED_PORT->BSRR = (LED0_MASK << 16))
#define LED0_TOGGLE         (LED_PORT->ODR  ^= LED0_MASK)

#define LED1_ON             (LED_PORT->BSRR = LED1_MASK)
#define LED1_OFF            (LED_PORT->BSRR = (LED1_MASK << 16))
#define LED1_TOGGLE         (LED_PORT->ODR  ^= LED1_MASK)
/** @} */

/**
 * @name User button
 * @{
 */
#define BTN0_PIN            GPIO_PIN(PORT_E, 4)
#define BTN0_MODE           GPIO_IN

#define BTN1_PIN            GPIO_PIN(PORT_E, 3)
#define BTN1_MODE           GPIO_IN
/** @} */


/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
