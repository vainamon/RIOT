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

#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    GPIO pin configuration
 */
static const  saul_gpio_params_t saul_gpio_params[] =
{
    {
        .name = "D2",
        .pin = LED0_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "D3",
        .pin = LED1_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "K0",
        .pin  = BTN0_PIN,
        .mode = BTN0_MODE
    },
    {
        .name = "K1",
        .pin  = BTN1_PIN,
        .mode = BTN1_MODE
    },
};

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PARAMS_H */
/** @} */
