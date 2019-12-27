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
 * @brief       Test application for MPU9X50 driver
 *
 * @author      Danilov Igor <danilov@superevm.ru>
 */

#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "mpu9x50.h"
#include "mpu9x50_params.h"

/* set interval to 1 second */
#define INTERVAL (1U * US_PER_SEC)

int main(void)
{
	xtimer_ticks32_t last_wakeup = xtimer_now();

	mpu9x50_t dev;
	mpu9x50_results_t measurement;
	int32_t temperature;
	int result;

	puts("MPU-9X50 test application");

	puts("+------------Initializing------------+");
	result = mpu9x50_init(&dev, &mpu9x50_params[0]);

	if (result == -1) {
		puts("[Error] The given i2c is not enabled");
		return 1;
	}
	else if (result == -2) {
		puts("[Error] The compass did not answer correctly on the given address");
		return 1;
	}

	mpu9x50_set_sample_rate(&dev, 200);
	if (dev.conf.sample_rate != 200) {
		puts("[Error] The sample rate was not set correctly");
		return 1;
	}
	mpu9x50_set_compass_sample_rate(&dev, 100);
	if (dev.conf.compass_sample_rate != 100) {
		puts("[Error] The compass sample rate was not set correctly");
		return 1;
	}

	puts("Initialization successful");
	printf("+------------Configuration------------+\n");
	printf("Sample rate: %"PRIu16" Hz\n", dev.conf.sample_rate);
	printf("Compass sample rate: %"PRIu8" Hz\n", dev.conf.compass_sample_rate);
	printf("Gyro full-scale range: 2000 DPS\n");
	printf("Accel full-scale range: 2 G\n");
	printf("Compass X axis factory adjustment: %"PRIu8"\n", dev.conf.compass_x_adj);
	printf("Compass Y axis factory adjustment: %"PRIu8"\n", dev.conf.compass_y_adj);
	printf("Compass Z axis factory adjustment: %"PRIu8"\n", dev.conf.compass_z_adj);

	printf("\n+--------Starting Measurements--------+\n");

	while(1) {
		/* Get accel data in milli g */
		mpu9x50_read_accel(&dev, &measurement);
		printf("Accel data [milli g] - X: %"PRId16"   Y: %"PRId16"   Z: %"PRId16"\n",
				measurement.x_axis, measurement.y_axis, measurement.z_axis);
		/* Get gyro data in dps */
		mpu9x50_read_gyro(&dev, &measurement);
		printf("Gyro data [dps] - X: %"PRId16"   Y: %"PRId16"   Z: %"PRId16"\n",
				measurement.x_axis, measurement.y_axis, measurement.z_axis);
		/* Get compass data in mikro Tesla */
		mpu9x50_read_compass(&dev, &measurement);
		printf("Compass data [micro T] - X: %"PRId16"   Y: %"PRId16"   Z: %"PRId16"\n",
				measurement.x_axis, measurement.y_axis, measurement.z_axis);
		/* Get temperature in milli degrees celsius */
		mpu9x50_read_temperature(&dev, &temperature);
		printf("Temperature [milli deg] : %"PRId32"\n", temperature);
		printf("\n+-------------------------------------+\n");

		xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

		LED0_TOGGLE;
	}

	return 0;
}
