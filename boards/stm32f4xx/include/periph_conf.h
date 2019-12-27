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
 * @brief       Board specific definitions for the Chinese STM32F4XX evaluation board
 *
 * @author      Danilov Igor <danilov@superevm.ru>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include "periph_cpu.h"
#include "f4/cfg_clock_168_8_0.h"
#include "cfg_spi_divtable.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    DMA streams configuration
 * @{
 */
#ifdef MODULE_PERIPH_DMA
static const dma_conf_t dma_config[] = {
    { .stream = 15 },    /* DMA2 Stream 7 - USART1_TX */
	{ .stream = 10 },    /* DMA2 Stream 2 - SPI1_RX */
	{ .stream = 11 },    /* DMA2 Stream 3 - SPI1_TX */
	{ .stream = 8  },    /* DMA2 Stream 0 - FSMC */
    { .stream = 6  },    /* DMA1 Stream 6 - USART2_TX */
};

#define DMA_0_ISR  			(isr_dma2_stream7)
#define DMA_1_ISR  			(isr_dma2_stream2)
#define DMA_2_ISR  			(isr_dma2_stream3)
#define DMA_3_ISR  			(isr_dma2_stream0)
#define DMA_4_ISR  			(isr_dma1_stream6)

#define DMA_NUMOF           ARRAY_SIZE(dma_config)
#endif
/** @} */

/**
 * @name   Timer configuration
 * @{
 */
static const timer_conf_t timer_config[] = {
    {
        .dev      = TIM2,
        .max      = 0xffffffff,
        .rcc_mask = RCC_APB1ENR_TIM2EN,
        .bus      = APB1,
        .irqn     = TIM2_IRQn
    }
};

#define TIMER_0_ISR         isr_tim2

#define TIMER_NUMOF         ARRAY_SIZE(timer_config)
/** @} */

/**
 * @name   UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = USART1,
        .rcc_mask   = RCC_APB2ENR_USART1EN,
        .rx_pin     = GPIO_PIN(PORT_A, 10),
        .tx_pin     = GPIO_PIN(PORT_A, 9),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB2,
        .irqn       = USART1_IRQn,
#ifdef UART_USE_DMA
        .dma 		= 0,
        .dma_chan   = 4
#endif
    },
    {
        .dev        = USART2,
        .rcc_mask   = RCC_APB1ENR_USART2EN,
        .rx_pin     = GPIO_PIN(PORT_A, 3),
        .tx_pin     = GPIO_PIN(PORT_A, 2),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART2_IRQn,
#ifdef UART_USE_DMA
        .dma 		= 4,
        .dma_chan   = 4
#endif
    },
};

#define UART_0_ISR          (isr_usart1)
#define UART_1_ISR          (isr_usart2)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name   SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
	//	NRF24L01 connector
    {
        .dev      = SPI1,
        .mosi_pin = GPIO_PIN(PORT_B, 5),
        .miso_pin = GPIO_PIN(PORT_B, 4),
        .sclk_pin = GPIO_PIN(PORT_B, 3),
        .cs_pin   = GPIO_PIN(PORT_B, 7),
		.mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI1EN,
        .apbbus   = APB2,
#ifdef SPI_USE_DMA
		.tx_dma 		= 2,
		.tx_dma_chan 	= 3,
		.rx_dma 		= 1,
		.rx_dma_chan 	= 3
#endif
    }
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

/**
 * @name I2C configuration
 * @{
 */
static const i2c_conf_t i2c_config[] = {
    {
        .dev            = I2C3,
        .speed          = I2C_SPEED_NORMAL,
        .scl_pin        = GPIO_PIN(PORT_A, 8),
        .sda_pin        = GPIO_PIN(PORT_C, 9),
        .scl_af         = GPIO_AF4,
        .sda_af         = GPIO_AF4,
        .bus            = APB1,
        .rcc_mask       = RCC_APB1ENR_I2C3EN,
        .clk            = CLOCK_APB1,
        .irqn           = I2C3_EV_IRQn
    }
};

#define I2C_0_ISR           isr_i2c3_ev

#define I2C_NUMOF           ARRAY_SIZE(i2c_config)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
