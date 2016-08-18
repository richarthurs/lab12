/*
 * richGPS.cpp
 *
 *  Created on: Jul 29, 2016
 *      Author: Richard
 */
#include "richGPS.h"

void GPS::Init(){
	// Config GPS-side UART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);// Enable UART hardware
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);// Enable Pin hardware
	GPIOPinConfigure(GPIO_PC4_U1RX);// Configure GPIO pin for UART RX line
	GPIOPinConfigure(GPIO_PC5_U1TX);// Configure GPIO Pin for UART TX line
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5); // set pins for UART
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void GPS::InterruptEnable(){
	// Enable interrupts for GPS-side UART
	IntMasterEnable();		// master interrupt enable
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
}

void GPS::InterruptDisable(){
	IntDisable(INT_UART1);
	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
}
