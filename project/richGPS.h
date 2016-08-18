/*
 * richGPS.h
 *
 *  Created on: Jul 29, 2016
 *      Author: Richard
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"


#ifndef RICHGPS_H_
#define RICHGPS_H_

class GPS{
public:
	void Init();
	void InterruptEnable();
	void InterruptDisable();
private:

};






#endif /* RICHGPS_H_ */
