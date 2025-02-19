#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "uartCommunication.h"
#include <stdio.h>


void initUART()
{
    // Configure UART Pins (P2.3 TX, 3.2 RX) for EUSCI_A1 and EUSCI_A2
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    // UART configuration for 9600 baud rate
    const eUSCI_UART_ConfigV1 uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        78, 2, 0,  // 9600 baud rate with 12 MHz clock
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };

    UART_initModule(UART_MODULE, &uartConfig);  // Initialize UART with config
    UART_enableModule(UART_MODULE);  // Enable UART module
    UART_initModule(UART_MODULE2, &uartConfig);
    UART_enableModule(UART_MODULE2);


}

void sendMessage(char* msg) {
    //char messageStr[20];  // Buffer for the formatted number

    printf("Message: %s\n", msg);  // Debug message

    // Send each character of the message via UART
    int i = 0;
    for (i = 0; msg[i] != '\0'; i++)
    {
        UART_transmitData(UART_MODULE, msg[i]);
    }
}



