
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "StateMachine.h"
#include "Model/model.h"
#include "View/view.h"
#include "Communication/uartCommunication.h"
#include <stdio.h>
#include <stdlib.h>

/* ADC results buffer */
static uint16_t resultsBuffer[3];
 char receivedData[20]; // Buffer for received string
 int index = 0;         // Index for buffer tracking
bool stopSending = false; //Bool to manage the UART interrupts
Options currentOption = MANUAL_OPTION;
int buttonPressed = 0; //S2 button toggle
int isControllingServo = 0;
int confirmMenuExit = 0;
//Hold current finger selected in manual state
Hand fingerSelected = MODE;
//Hold the current state
State_t currentState = IDLE;

StateMachine_t StateMachine[] = {
                                 {IDLE, fn_IDLE},
                                 {MANUAL, fn_MANUAL},
                                 {VISION, fn_VISION},
                                 {GESTURES, fn_GESTURES}
};




int main(void)
 {
    hwInit();
    graphicsInit();
    initUART();
    adcInit();

    while(1)
    {
        if(currentState < NUM_STATES)
        {
            (*StateMachine[currentState].func)();
        }
    }
}


void ADC14_IRQHandler(void)
{
    uint64_t status;
    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        if(isControllingServo)
        {
            if(resultsBuffer[1] > JOY_Y_HIGH_SERVO || resultsBuffer[1] < JOY_Y_HIGH_SERVO)
           {
               //The user has selected one of the options
               //The interrupt can be disable at the moment
               ADC14_disableInterrupt(ADC_INT1);  // Disable specific ADC MEM1 interrupt

           }
           if(resultsBuffer[0] < JOY_X_LOW)
           {
               //Exit from the sections
               ADC14_disableInterrupt(ADC_INT1);  // Disable specific ADC MEM1 interrupt
           }
        }
        else
        {
            if(resultsBuffer[1] > JOY_Y_HIGH || resultsBuffer[1] < JOY_Y_LOW)
            {
                ADC14_disableInterrupt(ADC_INT1);  // Disable specific ADC MEM1 interrupt
            }
            if(resultsBuffer[0] < JOY_X_LOW)
            {
                //Exit from the modes
                ADC14_disableInterrupt(ADC_INT1);  // Disable specific ADC MEM1 interrupt
            }
        }

    }
}
// GPIO Interrupt Handler for Port 3
void PORT3_IRQHandler(void)
{
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    if (status & GPIO_PIN5)  // Check if button was pressed
    {
        buttonPressed = 1;  // Set flag to indicate button press
        // Enable GPIO Interrupt in NVIC
    }
}

void PORT5_IRQHandler(void)
{
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN1)  // Check if button was pressed
    {
        bool fingerPressed = true;
        // Enable GPIO Interrupt in NVIC
    }
}

void EUSCIA2_IRQHandler(void)
{  // Correct handler for EUSCI_A2
    if (UART_getInterruptStatus(UART_MODULE2, EUSCI_A_UART_RECEIVE_INTERRUPT)) {
        char receivedChar = UART_receiveData(UART_MODULE2); // Read received character

        if (receivedChar == '\n') { // End of message
            receivedData[index] = '\0'; // Null-terminate string
            int num = atoi(receivedData);   // Convert string to integer

            if(currentState==MANUAL)
            {
                printf("Received: %d\n", num); // Print received data
                //Numbers correspond to the Fingers
                if(num >= 1 && num <= 5)
                {
                    //Meaning the Arduino is letting control the servos to MSP
                    isControllingServo = 1;
                }
            }
            if(currentState==VISION)
            {
                printf("Received: %d\n", num); // Print received data
                int confirmMenuExit = 0;
                if(num==1)
                {
                    //Confirmed exit
                    printf("Connection stopped successfully: %d\n", num); // Print received data
                }
            }
            stopSending = false;
            //STOP RECEIVING DATA
            // Disable UART Receive Interrupt
            UART_disableInterrupt(UART_MODULE2, EUSCI_A_UART_RECEIVE_INTERRUPT);
            Interrupt_disableInterrupt(INT_EUSCIA2);  // Enable UART2 ISR in NVIC

            index = 0; // Reset buffer index
            int i=0;
            for (i = 0; receivedData[i] != '\0'; i++) {
                receivedData[i] = ' ';
            }
        } else if (index < sizeof(receivedData) - 1) {
            receivedData[index++] = receivedChar; // Store character
        }
    }
}

void fn_IDLE(void)
{
    if (GPIO_getInputPinValue(RED_LED_PORT, RED_LED_PIN) == GPIO_INPUT_PIN_LOW)
    {
        GPIO_toggleOutputOnPin(RED_LED_PORT, RED_LED_PIN); //RED PIN
    }

    if (resultsBuffer[1] >= JOY_Y_HIGH)
    {
        drawManualSelected();
        currentOption = MANUAL_OPTION;
        GPIO_setOutputLowOnPin(BLUE_LED_PORT, BLUE_LED_PIN); // Turn off LED initially RED
        GPIO_setOutputLowOnPin(RED_LED_PORT2, RED_LED_PIN2);
        GPIO_setOutputHighOnPin(GREEN_LED_PORT, GREEN_LED_PIN); //YELLOW
    }

    if (resultsBuffer[1] < JOY_Y_LOW)
    {
        if(currentOption == OPENCV_OPTION)
        {
            drawGestureSelected();
            currentOption = GESTURES_OPTION;
            GPIO_setOutputLowOnPin(BLUE_LED_PORT, BLUE_LED_PIN); // Turn off LED initially GREEN
            GPIO_setOutputHighOnPin(GREEN_LED_PORT, GREEN_LED_PIN); //YELLOW
            GPIO_setOutputHighOnPin(RED_LED_PORT2, RED_LED_PIN2);
        }
        else
        {
            drawVisionSelected();
            currentOption = OPENCV_OPTION;
            GPIO_setOutputLowOnPin(GREEN_LED_PORT, GREEN_LED_PIN); // Turn off LED initially GREEN
            GPIO_setOutputLowOnPin(RED_LED_PORT2, RED_LED_PIN2);
            GPIO_setOutputHighOnPin(BLUE_LED_PORT, BLUE_LED_PIN); //BLUE
        }
    }


    ADC14_disableInterrupt(ADC_INT1);  // Disable specific ADC MEM1 interrupt
    __delay_cycles(6000000);

    if (buttonPressed && currentOption==OPENCV_OPTION)
    {
        currentState = VISION;
        //Generate the Fingers Menu
        buttonPressed=0;
        drawVisionMenu();
        char message[5] = "1\n";
        stopSending = workFinger(fingerSelected, stopSending, message);
    }
    else if (!buttonPressed && currentOption==OPENCV_OPTION)
    {
       currentState = IDLE;
       ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt
       //The user may be still choosing option, so the interrupt must be enabled again
    }

    if (buttonPressed && currentOption==MANUAL_OPTION)
    {
      currentState = MANUAL;
      //Generate the Fingers Menu
      buttonPressed=0;
      drawFingersMenu();
      char message[5] = "2\n";
      stopSending = workFinger(fingerSelected, stopSending, message);
    }
    else if (!buttonPressed && currentOption==MANUAL_OPTION)
    {
      currentState = IDLE;
      //The user may be still choosing option, so the interrupt must be enabled again
      ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt
    }

    if (buttonPressed && currentOption==GESTURES_OPTION)
    {
        currentState = GESTURES;
          //Generate the Gestures Menu
          buttonPressed=0;
          char message[5] = "3\n";
          stopSending = workFinger(fingerSelected, stopSending, message);
    }
    else if (!buttonPressed && currentOption==GESTURES_OPTION)
    {
        currentState = IDLE;
        //The user may be still choosing option, so the interrupt must be enabled again
        printf("I AM ENABLING THE ADC HANDLER FROM SELECTION GESTURES!!!!\n");
        ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt
    }
}

void fn_MANUAL(void)
{
    currentState = MANUAL;
    char message[5] = "     ";
    GPIO_setOutputLowOnPin(RED_LED_PORT, RED_LED_PIN); // Turn off LED initially RED

    //Start enabling the ADC Selector
    printf("I AM ENABLING THE ADC HANDLER FROM SELECTION MANUAL!!!!\n");
    ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt

    if (resultsBuffer[1] > JOY_Y_HIGH && !isControllingServo)
    {
        drawThumbSelected();
        fingerSelected = THUMB;
        stopSending = false;
    }
    if (resultsBuffer[1] < JOY_Y_LOW && !isControllingServo)
    {
        if(fingerSelected == RING)
        {
            //Meaning i am going to select pinky
            drawPinkySelected();
            fingerSelected = PINKY;
            stopSending = false;
        }
        else
        {
            if(fingerSelected == MIDDLE)
            {
                //Meaning i am going to select ring
                drawRingSelected();
                fingerSelected = RING;
                stopSending = false;
            }
            else
            {
                if(fingerSelected == INDEX)
                {
                    //Meaning i am going to select middle
                    drawMiddleSelected();
                    fingerSelected = MIDDLE;
                    stopSending = false;
                }
                else
                {
                    drawIndexSelected();
                    fingerSelected = INDEX;
                    stopSending = false;
                }
            }
        }
    }


    if(resultsBuffer[1] > JOY_Y_HIGH_SERVO && isControllingServo || resultsBuffer[1] < JOY_Y_LOW_SERVO  && isControllingServo)
    {
        //Is sending adc data to Arduino
        workServo(fingerSelected, resultsBuffer[1]);
    }


    if(resultsBuffer[0] < JOY_X_LOW)
    {
        if(isControllingServo)
        {
            //Send a 20000 value for exiting servo mode
            uint16_t exitValue = 20000;
            workServo(fingerSelected, exitValue);
            isControllingServo = 0;
        }
        else
        {
            //Exit from the section 6 == EXT in Arduino
            message[0] = '6';
            message[1] = '\n';
            stopSending = false;
            currentState = IDLE;
            fingerSelected = MODE;
            drawIdleMenu();
        }
    }
    __delay_cycles(6000000); //Delay to permit the user to click the S2 button before ADC14 Interrupt being served

    if(buttonPressed && !isControllingServo)
    {
        stopSending = workFinger(fingerSelected, stopSending, message);
        buttonPressed = 0;
        //So i am working with the finger, no need to change status, but i have to check how to exit
    }

}

void fn_VISION(void)
{
    currentState = VISION;
    GPIO_setOutputLowOnPin(RED_LED_PORT, RED_LED_PIN); // Turn off LED initially RED
    //Start enabling the ADC Selector
    ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt

    if(resultsBuffer[0] < JOY_X_LOW)
    {
        //Exit from the OCV section
        if(confirmMenuExit)
        {
            char message[5] = "     ";
              message[0] = '6';
              message[1] = '\n';
              stopSending = false;
              currentState = IDLE;
              stopSending = exitWorker(message, stopSending);
              drawIdleMenu();
        }
    }
    if(resultsBuffer[1] > JOY_Y_HIGH)
    {
        //Interrupt python execution
        drawConfirmMenu();
        confirmMenuExit = 1;
    }

    __delay_cycles(6000000);

    if(buttonPressed)
    {
        if(confirmMenuExit)
        {
            //Exit the connection -- send Message to Arduino
            stopSending = stopPythonMsg(stopSending);
            buttonPressed = 0;
        }
    }
}

void fn_GESTURES(void)
{
    currentState = GESTURES;
    GPIO_setOutputLowOnPin(RED_LED_PORT, RED_LED_PIN); // Turn off LED initially RED
    ADC14_enableInterrupt(ADC_INT1);  // Enable specific ADC MEM1 interrupt
    //Open an UART COMMUNICATION
    /** To be defined **/
}



