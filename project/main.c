//#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"

 char data[2000];
 volatile bool fired = false;

	//	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	//UARTCharPutNonBlocking(UART0_BASE, '\n');


		//UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

void output(char buffer[2000]){

	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

		uint32_t i = 0;
		for(i = 0; i <= strlen(buffer); i++){
			if(buffer[i] == 'G' && buffer[i+1] =='P' && buffer[i+2] == 'G' && buffer[i+3] == 'G' && buffer[i+4] == 'A'){
				uint32_t start = i+15;
				uint32_t t = 0;
				for(t = 0; t <= start + 15; t++){
					UARTCharPut(UART0_BASE, data[start+t]);
				}
				break;
			}
			else{

			}
		//	UARTCharPut(UART0_BASE, buffer[i]);
		}

		//UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
	//	UARTCharPutNonBlocking(UART0_BASE, '\n');
}

 UARTIntHandler(void){
	//UARTCharPutNonBlocking(UART0_BASE, 'Z'); 	// to print out the data itself
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status
	UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

	volatile uint32_t counter = strlen(data);	// make a counter to put the new character into the correct array position
	char result;
	//memset(data,0,sizeof(data));	// this zeros out the data array

	if(ui32Status != UART_INT_RT){
		fired = false;
		while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
				result = (char)UARTCharGet(UART1_BASE);
				data[counter] = result;
			//	UARTCharPutNonBlocking(UART0_BASE, data[counter]); 	// to print out the data itself
				counter++;
			//	width++;
		//		if(result == '$'){
		//			lines++;
		//		}
		}
	}


	if(ui32Status == UART_INT_RT && fired == false){		//
		fired = true;
		//UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
	//	output(data);
	//	UARTCharPutNonBlocking(UART0_BASE, 'Z');
		UARTCharPutNonBlocking(UART0_BASE, '\n'); 	// to print out the data itself
		output(data);
		memset(data,0,sizeof(data));	// this zeros out the data array
		UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	}
	//memset(data,0,sizeof(data));	// this zeros out the data array
 }



int main(void) {
SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // setup clock

// Config computer-side UART
SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART hardware
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable Pin hardware
GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO pin for UART RX line
GPIOPinConfigure(GPIO_PA1_U0TX);// Configure GPIO Pin for UART TX line
GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // set pins for UART
UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));




// Config GPS-side UART
SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);// Enable UART hardware
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);// Enable Pin hardware
GPIOPinConfigure(GPIO_PC4_U1RX);// Configure GPIO pin for UART RX line
GPIOPinConfigure(GPIO_PC5_U1TX);// Configure GPIO Pin for UART TX line
GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5); // set pins for UART
UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

// Enable interrupts for GPS-side UART
IntMasterEnable();		// master interrupt enable
IntEnable(INT_UART1);
UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

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
