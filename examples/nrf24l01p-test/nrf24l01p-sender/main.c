/*
 * Copyright (C) 2019 Supercomputers and Neurocomputers Research Center
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Test application for nrf24l01p driver
 *
 * @author      Danilov Igor <danilov@superevm.ru>
 */


#ifndef SPI_PORT
#error "SPI_PORT not defined"
#endif
#ifndef CE_PIN
#error "CE_PIN not defined"
#endif
#ifndef CS_PIN
#error "CS_PIN not defined"
#endif
#ifndef IRQ_PIN
#error "IRQ_PIN not defined"
#endif


#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <board.h>
#include <time.h>

#include "nrf24l01p.h"
#include "nrf24l01p_settings.h"
#include "periph/spi.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define INTERVAL (1U * US_PER_SEC)

static int cmd_send(int argc, char **argv);
static int cmd_its(int argc, char **argv);

static nrf24l01p_t nrf24l01p_0;

/**
 * @init transceiver
 */
int cmd_its(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	puts("Init Transceiver\n");

	/* initialize transceiver device */
	if (nrf24l01p_init(&nrf24l01p_0, SPI_PORT, CE_PIN, CS_PIN, IRQ_PIN) < 0) {
		puts("Error in nrf24l01p_init");
		return 1;
	}

	return 0;
}

/**
 * @set TX mode
 */
int cmd_send(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	puts("Send");

	int status = 0;
	char tx_buf[NRF24L01P_MAX_DATA_LENGTH];

	/* fill TX buffer with numbers 32..1 */
	for (size_t i = 0; i < sizeof(tx_buf); i++) {
		tx_buf[i] = NRF24L01P_MAX_DATA_LENGTH - i;
	}
	/* power on the device */
	if (nrf24l01p_on(&nrf24l01p_0) < 0) {
		puts("Error in nrf24l01p_on");
		return 1;
	}
	/* setup device as transmitter */
	if (nrf24l01p_set_txmode(&nrf24l01p_0) < 0) {
		puts("Error in nrf24l01p_set_txmode");
		return 1;
	}
	/* load data to transmit into device */
	if (nrf24l01p_preload(&nrf24l01p_0, tx_buf, NRF24L01P_MAX_DATA_LENGTH) < 0) {
		puts("Error in nrf24l01p_preload");
		return 1;
	}
	/* trigger transmitting */
	nrf24l01p_transmit(&nrf24l01p_0);
	/* wait while data is pysically transmitted  */
	xtimer_usleep(DELAY_DATA_ON_AIR);
	/* get status of the transceiver */
	status = nrf24l01p_get_status(&nrf24l01p_0);
	if (status < 0) {
		puts("Error in nrf24l01p_get_status");
	}
	if (status & TX_DS) {
		puts("Sent Packet");
	}

	return 0;
}

int main(void)
{
	xtimer_ticks32_t last_wakeup = xtimer_now();

	puts("Welcome to RIOT!");

	cmd_its(0, NULL);

	nrf24l01p_set_power(&nrf24l01p_0, -18);

	puts("Initialization successful - starting the messages sending now");

	while(1) {
		xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

		if (cmd_send(0, NULL) != 0 ) {
			puts("Error in send_cmd");
			return -1;
		}

		LED0_TOGGLE;
	}

	return 0;
}
