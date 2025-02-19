#ifndef MODEL_H_
#define MODEL_H_


#define RED_LED_PORT GPIO_PORT_P1
#define RED_LED_PIN GPIO_PIN0
#define BLUE_LED_PORT GPIO_PORT_P2
#define BLUE_LED_PIN GPIO_PIN2
#define GREEN_LED_PORT GPIO_PORT_P2
#define GREEN_LED_PIN GPIO_PIN1
#define RED_LED_PORT2 GPIO_PORT_P2
#define RED_LED_PIN2 GPIO_PIN0

#define JOY_Y_HIGH_SERVO 9000
#define JOY_Y_LOW_SERVO 3500

#define JOY_Y_HIGH 14000
#define JOY_Y_LOW 500
#define JOY_X_LOW 500

typedef enum
{
    MANUAL_OPTION,
    OPENCV_OPTION,
    GESTURES_OPTION
} Options;


typedef enum
{
    THUMB,
    INDEX,
    MIDDLE,
    RING,
    PINKY,
    MODE
} Hand;


void hwInit();
void adcInit();
bool workFinger(Hand fingerSelected, bool stopSending, char* msg);
void workServo(Hand fingerSelected,uint16_t adcValue);
bool exitWorker(char* msg, bool stopSending);
bool stopPythonMsg(bool stopSending);
#endif /* MODEL_H_ */
