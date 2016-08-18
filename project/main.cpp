#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

extern "C"{
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
}


#include "richGPS.h"

GPS gps;


void output(char buffer[1000]){
		uint32_t i = 0;

		for(i = 0; i < strlen(buffer); i++){
			UARTCharPut(UART0_BASE, buffer[i]);	// enable me to show the output ish
		}

		UARTCharPut(UART0_BASE, '\n');
}

 extern "C++" void UARTIntHandler(void){

	 static char data[1000];	// this will keep its value between calls to the interrupt
	 static uint8_t lines = 0;	// we count the number of lines so that we get a complete set of sentences

	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true); //get GPS interrupt status
	UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

	volatile uint32_t counter = strlen(data);	// make a counter to put the new character into the correct array position
	static uint8_t numLines = 7;

	if(ui32Status != UART_INT_RT && ui32Status == UART_INT_RX){	// we haven't timed out but we did receive a character

		char result;
		while (lines < numLines){	// check for characters until we reach the correct number of lines
			while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
				result = (char)UARTCharGet(UART1_BASE);

				data[counter] = result;	// put the char at the end of the string
				counter++;


				if (result == '\n'){
					lines++;
				}


				if (counter >> 9 && data[(strlen(data) - 1)] == 'V' && data[(strlen(data) - 2)] == 'S'){		// we need to determine the number of satellites seen
						numLines = 9;//(5 + (result - '0'));
				}
			}
		}

	}


	if (lines == numLines){	// when we have 8 complete lines, do other stuff
		UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// turn off RX and RT (timeout) interrupts so we don't interrupt ourselves

		output(data);
		lines = 0;	// we need to make sure to do this since it's static and will persist between ISR calls

		memset(data,0,sizeof(data));	// zero out the data array
		UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// turn interrupts on again so we're good to go
	}

 }



int main(void) {
SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // setup clock

// Config computer-side UART
SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART hardware
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable Pin hardware
GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO pin for UART RX line
GPIOPinConfigure(GPIO_PA1_U0TX);// Configure GPIO Pin for UART TX line
GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // set pins for UART
UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 57600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


gps.Init();
gps.InterruptEnable();



// Turn on GPIO for example
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);



while (1){
	//output(data);
//if(lines >=8 ){
//	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
//	lines = 0;
//	output(data);
//	memset(data,0,sizeof(data));	// this zeros out the data array
//	width = 0;
//	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
//}

//	thingy(data);
//UARTCharPut(UART0_BASE, UARTCharGet(UART1_BASE));



}
}
