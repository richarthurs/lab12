//#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"

void UARTIntHandler(void){
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status
	UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

	while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
		int32_t result =  UARTCharGetNonBlocking(UART1_BASE);
		unsigned char a = '/n';
		if(result == 32){
			UARTCharPutNonBlocking(UART0_BASE,a);
		}

		//	UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART1_BASE));//spit out character
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
//			SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
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
