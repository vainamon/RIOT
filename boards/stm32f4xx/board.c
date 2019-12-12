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

#include "board.h"
#include "periph/gpio.h"

void board_init(void)
{
    /* initialize the CPU */
    cpu_init();

    /* initialize the boards LEDs */
    gpio_init(LED0_PIN, GPIO_OUT);
    gpio_init(LED1_PIN, GPIO_OUT);
}
