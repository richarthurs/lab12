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
char master [2000];
uint8_t xlines = 0;

	//	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	//UARTCharPutNonBlocking(UART0_BASE, '\n');


		//UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

void output(char buffer[]){
		uint32_t i = 0;
		for(i = 0; i <= strlen(buffer); i++){
			UARTCharPutNonBlocking(UART0_BASE, buffer[i]);
		}
		UARTCharPutNonBlocking(UART0_BASE, '\n');
}
uint8_t analyze(char mydata[2000]){
	uint32_t i = 0;
	uint8_t dollas = 0;
	for(i = 0; i <= strlen(mydata); i++){
		if(mydata[i] == '$'){
			dollas++;
		}
	}
	return dollas;
	//UARTCharPutNonBlocking(UART0_BASE, '\n');
	//if(dollas == 9) return true;
	//else return false;
}

void UARTIntHandler(void){

	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status
	UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

	uint32_t counter = 0;	// make a counter to put the new character into the correct array position
	char result;
	//memset(data,0,sizeof(data));	// this zeros out the data array
	while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
		result = (char)UARTCharGet(UART1_BASE);
		data[counter] = result;
		if(data[counter] == '$'){
			xlines++;
		}
	//	UARTCharPutNonBlocking(UART0_BASE, data[counter]); 	// to print out the data itself
		counter++;
	}
	//while(!UARTCharsAvail(UART1_BASE)){
		//// stuff went here
//		UARTCharPutNonBlocking(UART0_BASE, 'p'); 	// enable me to see when the interrupt is firing
	//}



//	uint32_t f = 0;	// for my for loop
//	volatile uint8_t lines = 0;
//	for(f = 0; f <= strlen(data); f++){
//		if(data[f] == '$'){
////			UARTCharPutNonBlocking(UART0_BASE, 'P'); 	// this works to print out a Z
////			UARTCharPutNonBlocking(UART0_BASE, '\n'); 	// this works to print out a Z
//			lines++;
//		}
//	}
//
//		if(lines >= 6){
//			UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
//
//	//		UARTCharPutNonBlocking(UART0_BASE, '\n'); 	// this works to print out a Z
//		//	UARTCharPutNonBlocking(UART0_BASE, 'Z'); 	// this works to print out a Z
//		//	UARTCharPutNonBlocking(UART0_BASE, '\n'); 	// this works to print out a Z
//			memset(master,0,sizeof(master));
//		}




//	UARTCharPutNonBlocking(UART0_BASE, '\n'); 	// this works to print out a Z
//	if(analyze(master) > 8){
//		UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
//		output(master);
//		UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
//
//	}

	//UARTCharPutNonBlocking(UART0_BASE, '\n' );	// snag the character from the UART
		//UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	counter = 0;
}
void thingy(char mydata[]){
//	uint8_t lines = analyze(mydata);
//	if(lines >> 2){
//		output(data);
//	}
//	// let's clear everything out of the data array
	output(data);
	memset(data,0,sizeof(data));

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

	if(xlines >= 8){
				UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
				xlines = 0;
				output(data);
				memset(data,0,sizeof(data));	// this zeros out the data array
				UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
		}

//	thingy(data);
//UARTCharPut(UART0_BASE, UARTCharGet(UART1_BASE));



}
}
