#ifndef UARTCOMMUNICATION_H_
#define UARTCOMMUNICATION_H_

#define UART_MODULE EUSCI_A1_BASE  // Use EUSCI_A1 for UART communication
#define UART_MODULE2 EUSCI_A2_BASE  // Using EUSCI_A2 (P3.2 RX, P3.3 TX)

void initUART();
void sendMessage(char* msg);

#endif
