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

 char text[574];
 char temp[16];

void zeros(void){

	uint32_t i = 0;
	for(i = 0; i <= 573; i++){
		text[i] = '\0';
	}

}
void output(void){
	uint32_t length = sizeof(text);
	uint32_t i = 0;
	for(i = 0; i <= length; i++){
		UARTCharPutNonBlocking(UART0_BASE, text[i]);//spit out character
	}
}
void newspit(void){
	UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	uint32_t bufferlength = sizeof(text);
	uint32_t datalength = sizeof(temp);
	static uint8_t lines = 0;

	uint32_t i = 0;
	for(i = 0; i <= datalength; i++ ){
		if(temp[i] == '$'){
			lines++;
		}
		else if(lines == 7){
			break;
		}
		text[i+bufferlength] = temp[i];
	}
	if(lines == 8){
		output();
	}

	uint8_t x = 0;
	for(x = 0; x <= 7; x++){
		temp[i] = '\0';
	}
}
void LatLong(void){
	uint32_t i = 0;
	for(i = 0; i < sizeof(text); i++){
		if(text[i] == 'G'){
			uint32_t x = 0;
			for(x = 0; x <= 26; x++){
				UARTCharPutNonBlocking(UART0_BASE, text[15+x+i]);
			}
			UARTCharPutNonBlocking(UART0_BASE, '\n');
		}
	}
}
bool checkLines(void){
	uint8_t lines = 0;
	uint32_t i = 0;
	for(i = 0; i < sizeof(text); i++){
		if(text[i] == '\n'){
			lines++;
		}
		if(lines >= 7){
			return true;
		}
	}
	return false;
}
void spit(void){
	//UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

	// we now have temp coming in, let's add it to text every time
	//strcat(temp, text); 	// adds temp into text every time

	if(checkLines){
		UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
		//LatLong();
		output();
	//	uint32_t i = 0;

	//	for(i = 0; i<=sizeof(text);i++){
	//	 UARTCharPutNonBlocking(UART0_BASE, text[i]);
	//	}

		/////////////
		//UARTCharPutNonBlocking(UART0_BASE, '\r');

	//	UARTCharPutNonBlocking(UART0_BASE, '\n');
	//	UARTCharPutNonBlocking(UART0_BASE, '\n');
		//SysCtlDelay(1000000);

		zeros();

		UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)
		unsigned char a = 'Z';
				UARTCharPutNonBlocking(UART0_BASE, a);
	}


//	UARTCharPutNonBlocking(UART0_BASE, '\r');//spit out character
//	UARTCharPutNonBlocking(UART0_BASE, '\n');//spit out character
//	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);	// receiver RX and receiver timeout interrupts RT(timeout = 32 bits)

}

void UARTIntHandler(void){
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status
	UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

	int32_t counter = 0;
	int32_t result = 0;



	while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
			result =  UARTCharGetNonBlocking(UART1_BASE);	// snag the character from the UART
			text[counter] = (char)result;

			counter++;




//		 char a = '\n';
//		if(result == 32){
//			UARTCharPutNonBlocking(UART0_BASE,a);
//		}
//		if(result == 36){
//			UARTCharPutNonBlocking(UART0_BASE, '\n');//spit out character
//		}
//		int32_t thingy = 73;

			//UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART1_BASE));//spit out character
		//	UARTCharPutNonBlocking(UART0_BASE, '\n');//spit out character

		//	UARTCharPutNonBlocking(UART0_BASE, result);//spit out character
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
//			SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
	}
	spit();
	counter = 0;

//	int32_t size = counter;
//	int32_t i = 0;
//	for(i = 0; i <= size; i++){	// we now have the string, loop through and spit it back out
//		UARTCharPutNonBlocking(UART0_BASE, string[i]);//spit out character
//	}
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

//UARTCharPut(UART0_BASE, UARTCharGet(UART1_BASE));



}
}
