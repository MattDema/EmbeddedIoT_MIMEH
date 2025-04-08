#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

typedef enum
{
    IDLE,
    MANUAL,
    VISION,
    GESTURES,
    NUM_STATES
}State_t;

typedef struct
{
    State_t state;
    void (*func) (void);
} StateMachine_t;

void fn_IDLE();
void fn_MANUAL();
void fn_VISION();
void fn_GESTURES();


#endif /* STATEMACHINE_H_ */
