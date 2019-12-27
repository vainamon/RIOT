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
 * @brief       Test server application for esp-01s WiFi module
 *
 * @author      Danilov Igor <danilov@superevm.ru>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "at.h"
#include "timex.h"
#include "xtimer.h"

#include "periph/uart.h"

#define INTERVAL (1U * US_PER_SEC)

static at_dev_t at_dev;
static char buf[256];
static char resp[1024];

static int init(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (argc < 3) {
		printf("Usage: %s <uart> <baudrate>\n", argv[0]);
		return 1;
	}

	uint8_t uart = atoi(argv[1]);
	uint32_t baudrate = atoi(argv[2]);

	int res = at_dev_init(&at_dev, UART_DEV(uart), baudrate, buf, sizeof(buf));

	/* check the UART initialization return value and respond as needed */
	if (res == UART_NODEV) {
		puts("Invalid UART device given!");
		return 1;
	}
	else if (res == UART_NOBAUD) {
		puts("Baudrate is not applicable!");
		return 1;
	}

	return res;
}

static int send(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		return 1;
	}

	ssize_t len;
	if ((len = at_send_cmd_get_resp(&at_dev, argv[1], resp, sizeof(resp), 10 * US_PER_SEC)) < 0) {
		puts("Error");
		return 1;
	}

	printf("Response (len=%d): %s\n", (int)len, resp);

	return 0;
}

static int send_ok(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		return 1;
	}

	if (at_send_cmd_wait_ok(&at_dev, argv[1], 10 * US_PER_SEC) < 0) {
		puts("Error");
		return 1;
	}

	puts("OK");

	return 0;
}

//static int send_lines(int argc, char **argv)
//{
//	if (argc < 2) {
//		printf("Usage: %s <command>\n", argv[0]);
//		return 1;
//	}
//
//	ssize_t len;
//	if ((len = at_send_cmd_get_lines(&at_dev, argv[1], resp, sizeof(resp), true, 10 * US_PER_SEC)) < 0) {
//		puts("Error");
//		return 1;
//	}
//
//	printf("Response (len=%d): %s\n", (int)len, resp);
//
//	return 0;
//}

//static int send_recv_bytes(int argc, char **argv)
//{
//	char buffer[64];
//
//	if (argc < 3) {
//		printf("Usage: %s <command> <number of bytes>\n", argv[0]);
//		return 1;
//	}
//
//	sprintf(buffer, "%s%s", argv[1], AT_SEND_EOL);
//	at_send_bytes(&at_dev, buffer, strlen(buffer));
//
//	ssize_t len = at_recv_bytes(&at_dev, buffer, atoi(argv[2]), 10 * US_PER_SEC);
//
//	printf("Response (len=%d): %s\n", (int)len, buffer);
//
//	return 0;
//}

static int send_recv_bytes_until_string(int argc, char **argv)
{
	char buffer[128];
	size_t len = sizeof(buffer);

	memset(buffer, 0, sizeof(buffer));

	if (argc < 3) {
		printf("Usage: %s <command> <string to expect>\n", argv[0]);
		return 1;
	}

	sprintf(buffer, "%s%s", argv[1], AT_SEND_EOL);
	at_send_bytes(&at_dev, buffer, strlen(buffer));
	memset(buffer, 0, sizeof(buffer));

	int res = at_recv_bytes_until_string(&at_dev, argv[2], buffer, &len,
			10 * US_PER_SEC);

	if (res) {
		puts("Error");
		return 1;
	}

	printf("Response (len=%d): %s\n", (int)len, buffer);
	return 0;
}

//static int drain(int argc, char **argv)
//{
//	(void)argc;
//	(void)argv;
//
//	at_drain(&at_dev);
//
//	return 0;
//}

static int power_on(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	at_dev_poweron(&at_dev);

	puts("Powered on");

	return 0;
}

//static int power_off(int argc, char **argv)
//{
//	(void)argc;
//	(void)argv;
//
//	at_dev_poweroff(&at_dev);
//
//	puts("Powered off");
//
//	return 0;
//}

int main(void)
{
	uint16_t connect_tries = 0;
	uint32_t msg_counter = 0;
	char msg_str[64];

	char* power_on_cmd_str[] = {"power_on"};
	char* init_cmd_str[] = {"init", "1", "115200"};
	char* restart_cmd_str[] = {"send_ok", "AT+RST"};
	char* disable_echoing_cmd_str[] = {"send_ok", "ATE0"};
	char* check_at_cmd_str[] = {"send_ok", "AT"};
	char* set_cwmode_at_cmd_str[] = {"send_ok", "AT+CWMODE_CUR=1"};
	char* set_connect_to_ap_at_cmd_str[] = {"send_recv_bytes_until_string", "AT+CWJAP_CUR=\"ESP-01S-RIOT-SERVER\",\"PASS1234567890\"", "WIFI CONNECTED"};
	char* get_ip_at_cmd_str[] = {"send", "AT+CIPSTA_CUR?"};
	char* enable_mux_at_cmd_str[] = {"send_ok", "AT+CIPMUX=1"};
	char* start_udp_conn_at_cmd_str[] = {"send_recv_bytes_until_string", "AT+CIPSTART=0,\"UDP\",\"192.168.1.1\",4445,4445,0", "CONNECT"};
	char* start_send_ex_at_cmd_str[] = {"send_recv_bytes_until_string", "AT+CIPSENDEX=0,64", "OK>"};
	char* start_send_data_at_cmd_str[] = {"send_recv_bytes_until_string", msg_str, "SEND OK"};

	puts("Power on device");
	if (power_on(ARRAY_SIZE(power_on_cmd_str), power_on_cmd_str) != 0 ) {
		puts("Error in power_on_cmd");
		return -1;
	}

	xtimer_usleep(10);

	puts("Init device");
	if (init(ARRAY_SIZE(init_cmd_str), init_cmd_str) != 0 ) {
		puts("Error in init_cmd");
		return -1;
	}

	xtimer_usleep(10);

	puts("Restart device");

	send_ok(ARRAY_SIZE(restart_cmd_str), restart_cmd_str);

	xtimer_sleep(5);

	puts("Change echoing mode to AE0");
	// ignoring echoing error
	send_ok(ARRAY_SIZE(disable_echoing_cmd_str), disable_echoing_cmd_str);

	xtimer_usleep(10);

	puts("Send test AT command");

	if (send_ok(ARRAY_SIZE(check_at_cmd_str), check_at_cmd_str) != 0 ) {
		puts("Error in send_ok_cmd when checking AT");
		return -1;
	}

	xtimer_usleep(10);

	puts("Initialization successful - start connection to the UDP server now");

	puts("Set cw mode");

	if (send_ok(ARRAY_SIZE(set_cwmode_at_cmd_str), set_cwmode_at_cmd_str) != 0 ) {
		puts("Error in set_cwmode");
		return -1;
	}

	xtimer_usleep(10);

	puts("Connect to AP");

	while (send_recv_bytes_until_string(ARRAY_SIZE(set_connect_to_ap_at_cmd_str), set_connect_to_ap_at_cmd_str) != 0 ) {
		puts("Error when connect to AP");

		if (++connect_tries > 3)
			return -1;

		xtimer_sleep(1);
	}

	xtimer_sleep(3);

	if (send(ARRAY_SIZE(get_ip_at_cmd_str), get_ip_at_cmd_str) != 0 ) {
		puts("Error when getting IP");
		return -1;
	}

	xtimer_usleep(10);

	puts("Enable multiple connections");

	if (send_ok(ARRAY_SIZE(enable_mux_at_cmd_str), enable_mux_at_cmd_str) != 0 ) {
		puts("Error when enabling multiple connections");
		return -1;
	}

	xtimer_usleep(10);

	puts("Start UDP connection");

	if (send_recv_bytes_until_string(ARRAY_SIZE(start_udp_conn_at_cmd_str), start_udp_conn_at_cmd_str) != 0 ) {
		puts("Error when starting UDP connection");
		return -1;
	}

	xtimer_usleep(10);
	xtimer_ticks32_t last_wakeup = xtimer_now();

	while(1) {
		xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

        sprintf(msg_str, "ESP-01S test message #%lu\\0", msg_counter);

        printf("Start sending message (counter = %lu)\n", msg_counter);

    	if (send_recv_bytes_until_string(ARRAY_SIZE(start_send_ex_at_cmd_str), start_send_ex_at_cmd_str) != 0 ) {
    		puts("Error when initiate send_ex command");
    		return -1;
    	}

    	if (send_recv_bytes_until_string(ARRAY_SIZE(start_send_data_at_cmd_str), start_send_data_at_cmd_str) != 0 ) {
    		puts("Error when data is sending");
    		return -1;
    	}

		msg_counter++;
		LED0_TOGGLE;
	}

	return 0;
}
