#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "model.h"
#include "../Communication/uartCommunication.h"
#include <stdio.h>

void hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Setting LEDs
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // Set P1.0 as output
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2); // Set P2.2 as output
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1); // Set P2.1 as output
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0); // Set P2.0 as output
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // Turn off LED initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // Turn off LED initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2); // Turn off LED initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1); // Turn off LED initially


    //Configuring PORT3 Interrupt
    // Configure button as input with pull-up resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);

    //Configuring PORT3 Interrupt
   // Configure button as input with pull-up resistor
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);

    // Enable interrupt on falling edge (button press)
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);

    // Enable GPIO Interrupt in NVIC
    Interrupt_enableInterrupt(INT_PORT3);


    // Enable interrupt on falling edge (button press)
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    // Enable GPIO Interrupt in NVIC
    Interrupt_enableInterrupt(INT_PORT5);

}

void adcInit()
{
    /* Configures Pin 6.0 and 4.4 and 3.5 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

        /* Initializing ADC (ADCOSC/64/8) */
        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

        /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
             * with internal 2.5v reference */
        ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
        ADC14_configureConversionMemory(ADC_MEM0,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

        ADC14_configureConversionMemory(ADC_MEM1,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

        /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
         *  is complete and enabling conversions */
        ADC14_enableInterrupt(ADC_INT1);

        /* Enabling Interrupts */
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();

        /* Setting up the sample timer to automatically step through the sequence
         * convert.
         */
        ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

        /* Triggering the start of the sample */
        ADC14_enableConversion();
        ADC14_toggleConversionTrigger();
}

//For manual section
bool workFinger(Hand fingerSelected, bool stopSending, char* msg)
{
    //Open an UART COMMUNICATION
       if(!stopSending)
       {
           char message[1];
           //Message to send to Arduino in order to control the single fingers of the robotic hand
           switch(fingerSelected)
           {
               case THUMB:
                   message[0] = '1';
                   sendMessage(message);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;

               case INDEX:
                  message[0] = '2';
                   sendMessage(message);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;

               case MIDDLE:
                    message[0] = '3';
                   sendMessage(message);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;

               case RING:
                   message[0] = '4';
                   sendMessage(message);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;

               case PINKY:
                    message[0] = '5';
                   sendMessage(message);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;

               case MODE:
                   printf("Sono in mode\n");
                   sendMessage(msg);
                   __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
               break;
           }

           stopSending = true;
       }

       //After sending a message the MSP expect a response
       if(stopSending)
       {
       // Enable UART Receive Interrupt
           UART_enableInterrupt(UART_MODULE2, EUSCI_A_UART_RECEIVE_INTERRUPT);
           Interrupt_enableInterrupt(INT_EUSCIA2);  // Enable UART2 ISR in NVIC
       }

       return stopSending;

}

void workServo(Hand fingerSelected, uint16_t adcValue)
{
    //Work off the adcValue
    char message[7];
    sprintf(message, "%d\n", adcValue);
    sendMessage(message);
    __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
}


bool exitWorker(char* msg, bool stopSending)
{
    if(!stopSending)
    {
        sendMessage(msg);
        __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
        stopSending = true;
    }
    if(stopSending)
    {
        // Enable UART Receive Interrupt
        UART_enableInterrupt(UART_MODULE2, EUSCI_A_UART_RECEIVE_INTERRUPT);
        Interrupt_enableInterrupt(INT_EUSCIA2);  // Enable UART2 ISR in NVIC
    }
    return stopSending;

}

bool stopPythonMsg(bool stopSending)
{
    if(!stopSending)
    {
        //Stop Python Connection
        char msg[5] = "1";
        sendMessage(msg);
        __delay_cycles(12000000);  // Approx 1-second delay (adjust as needed)
        stopSending = true;
    }
    if(stopSending)
    {
        // Enable UART Receive Interrupt
        UART_enableInterrupt(UART_MODULE2, EUSCI_A_UART_RECEIVE_INTERRUPT);
        Interrupt_enableInterrupt(INT_EUSCIA2);  // Enable UART2 ISR in NVIC
    }
    return stopSending;
}

