#include <Servo.h>

//Control Flags
bool speakingMSP = true;
bool speakingArduino = false;
bool speakingPython = true;
bool speakingManual = true;
char changingCondition; 
bool isTrasmissed = false;
bool isVisionConnected = false;
//Message buffers
char message[5] = "1";
char response[5];
char adcNum[10];
int index = 0;
int check = 0;
char signalToMSP = ' ';

enum Mode 
{
  NUL,
  OCV,
  MAN,
  GES,
  NUL2,
  NUL3,
  EXT
};

enum Hand {
  NULB,
  THUMB,
  INDEX,
  MIDDLE,
  RING,
  PINKY
};

//Servos objects for each finger
Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo pinkyServo;

void setup() 
{
// Attach the servos to the pins where they are connected
    thumbServo.attach(5);    // Thumb servo connected to pin 5
    indexServo.attach(6);   // Index servo connected to pin 6
    middleServo.attach(7);  // Middle servo connected to pin 7
    ringServo.attach(9);    // Ring servo connected to pin 9
    pinkyServo.attach(10);   // Pinky servo connected to pin 8

//Setting Servos in ready position
    thumbServo.write(0);
    indexServo.write(0);
    middleServo.write(0);
    ringServo.write(0);
    pinkyServo.write(0);

    Serial.begin(9600);   // USB Serial Monitor
    Serial1.begin(9600);  // Hardware UART on D0 (RX) and D1 (TX)}
}
void loop()
{
   thumbServo.write(180);
    indexServo.write(180);
    middleServo.write(180);
    ringServo.write(180);
    pinkyServo.write(180);
  while(speakingMSP) 
  {
    //Checks if there is data coming in
    if(Serial1.available() > 0) 
    {
      char c = Serial1.read();
      response[index] = c;
      index++;
      //Serial.print(c);
      changingCondition = c;
      speakingArduino = true;
    } 
    else 
    {
      //The response is completed and ready to be processed
      if(speakingArduino && changingCondition == '\n') 
      {
        int choice = atoi(response);
        Serial.println(response);
        index = 0;
      //CleanUp
        for(int i=0; i<5; i++)
        {
          response[i] = ' ';
        }
        changingCondition = ' ';
        speakingArduino = false;

        switch (choice) 
        {
        case OCV:
          Serial.println("OCV selezionato");
          speakingPython = true;
          isVisionConnected = true;
          while(speakingPython) 
          {
            if(Serial.available() > 0 && isVisionConnected) 
            {
              // Read the servo angles (they are sent as a formatted string)
              int thumbAngle = Serial.parseInt();
              int indexAngle = Serial.parseInt();
              int middleAngle = Serial.parseInt();
              int ringAngle = Serial.parseInt();
              int pinkyAngle = Serial.parseInt();
              // Move the servos based on the received angles
              thumbServo.write(thumbAngle);
              indexServo.write(indexAngle);
              middleServo.write(middleAngle);
              ringServo.write(ringAngle);
              pinkyServo.write(pinkyAngle);
            }
            //Check if a connection stop character has arrived 1/0
            if(Serial1.available() && isVisionConnected)
            { 
               char c = Serial1.read();
              char command[1];
               command[0] = c;
              int num = atoi(command);
              if(num==0)
              {
                //Connection still
                isVisionConnected = true;
              }
              else
              {
                //Connection close
                isVisionConnected = false;
                 speakingPython = false;
                if(num == EXT) 
                {
                     speakingPython = false;
                      c = ' ';
                    command[0] = ' ';
                }
              }
            }
          }
          break;

         case MAN:
              //Check for the ADC signal
              check = 0;
              speakingManual = true;
              while(speakingManual) 
             {
                char c = Serial1.read();
                char command[1];
                command[0] = c;

                int selectedFinger = atoi(command);

                switch(selectedFinger)
                {
                  case THUMB:                    
                  //Loop for waiting how much it has to flex, from the MSP via serial comm. Send an acknowledgment message, to notify the MSP that has the control of the servo motor
                    signalToMSP = THUMB + '0';
                    Serial1.println(signalToMSP);
                   // Serial.print("Signal sent:");
                   // Serial.println(signalToMSP);

                    //Starting reading the ADC signal
                    while(check!=EXT)
                    {
                      if(Serial1.available() > 0)
                      {
                        char c = Serial1.read();
                       // Serial.println(c);                                 
                        adcNum[index] = c;
                        index++;    
                        changingCondition = c;
                        isTrasmissed = true;  
                      }
                      else 
                      {
                        if(isTrasmissed && changingCondition == '\n')
                        {
                          int adcValue = atoi(adcNum);
                          bool timeToExit = false;
                          //Check if it is an exit code
                          
                          if (adcValue == 20000)
                          {
                              timeToExit = true;
                          }
                      
                          if(timeToExit)
                          {
                              check = EXT;
                              selectedFinger = EXT;
                          }
                          else
                          {
                            //I can now control the servos
                            //The values from the MSP corresponds from zero to 16500, so every degree corresponds to 88, so for each value that it receives it has to divide by 88
                            int servoAngle = adcValue / 88;
                            if(servoAngle > 180)
                            {
                              servoAngle = 180;
                            }
                            thumbServo.write(servoAngle);
                          }
                          //Cleanup
                          for(int i=0; i<5; i++)
                            {
                              adcNum[i] = ' ';
                            }
                            index = 0;
                            changingCondition= ' ';
                        }
                      }
                    }
                  
                  break;

                  case INDEX:
                         //Loop for waiting how much it has to flex, from the MSP via serial comm. Send an acknowledgment message, to notify the MSP that has the control of the servo motor
                    signalToMSP = INDEX + '0';
                    Serial1.println(signalToMSP);
                   // Serial.print("Signal sent:");
                   // Serial.println(signalToMSP);

                    //Starting reading the ADC signal
                    while(check!=EXT)
                    {
                      if(Serial1.available() > 0)
                      {
                        char c = Serial1.read();
                        //Serial.println(c);                                 
                        adcNum[index] = c;
                        index++;    
                        changingCondition = c;
                        isTrasmissed = true;  
                      }
                      else 
                      {
                        if(isTrasmissed && changingCondition == '\n')
                        {
                          int adcValue = atoi(adcNum);
                          bool timeToExit = false;
                          
                          if (adcValue == 20000)
                          {
                              timeToExit = true;
                          }
                          if(timeToExit)
                          {
                              check = EXT;
                              selectedFinger = EXT;
                          }
                          else
                          {
                            //I can now control the servos
                            int servoAngle = adcValue / 88;
                            if(servoAngle > 180)
                            {
                              servoAngle = 180;
                            }
                            indexServo.write(servoAngle);
                          }
                          //Cleanup
                          for(int i=0; i<5; i++)
                            {
                              adcNum[i] = ' ';
                            }
                            index = 0;
                            changingCondition= ' ';
                        }
                      }
                    }
                        
                  break;

                  case MIDDLE:
                         //Loop for waiting how much it has to flex, from the MSP via serial comm. Send an acknowledgment message, to notify the MSP that has the control of the servo motor
                    signalToMSP = MIDDLE + '0';
                    Serial1.println(signalToMSP);
                    //Serial.print("Signal sent:");
                    //Serial.println(signalToMSP);

                    //Starting reading the ADC signal
                    while(check!=EXT)
                    {
                      if(Serial1.available() > 0)
                      {
                        char c = Serial1.read();
                       // Serial.println(c);                                 
                        adcNum[index] = c;
                        index++;    
                        changingCondition = c;
                        isTrasmissed = true;  
                      }
                      else 
                      {
                        if(isTrasmissed && changingCondition == '\n')
                        {
                          int adcValue = atoi(adcNum);
                         // Serial.println(adcNum);
                          bool timeToExit = false;
                          //Check if it is an exit code
                          
                          if (adcValue == 20000)
                          {
                              timeToExit = true;
                          }
                      
                          if(timeToExit)
                          {
                              check = EXT;
                              selectedFinger = EXT;
                          }
                          else
                          {
                            //I can now control the servos
                            int servoAngle = adcValue / 88;
                            if(servoAngle > 180)
                            {
                              servoAngle = 180;
                            }
                            Serial1.print(servoAngle);
                            middleServo.write(servoAngle);
                          }
                          //Clean up
                          for(int i=0; i<5; i++)
                            {
                              adcNum[i] = ' ';
                            }
                            index = 0;
                            changingCondition= ' ';
                        }
                      }
                    }
                  break;

                  case RING:
                         //Loop for waiting how much it has to flex, from the MSP via serial comm. Send an acknowledgment message, to notify the MSP that has the control of the servo motor
                    signalToMSP = RING + '0';
                    Serial1.println(signalToMSP);
                    //Serial.print("Signal sent:");
                    //Serial.println(signalToMSP);

                    //Starting reading the ADC signal
                    while(check!=EXT)
                    {
                      if(Serial1.available() > 0)
                      {
                        char c = Serial1.read();
                        //Serial.println(c);                                 
                        adcNum[index] = c;
                        index++;    
                        changingCondition = c;
                        isTrasmissed = true;  
                      }
                      else 
                      {
                        if(isTrasmissed && changingCondition == '\n')
                        {
                          int adcValue = atoi(adcNum);
                          //Serial.println(adcNum);
                          bool timeToExit = false;
                          //Check if it is an exit code
                         
                          if (adcValue == 20000)
                          {
                              timeToExit = true;
                          }
                          
                          if(timeToExit)
                          {
                              check = EXT;
                              selectedFinger = EXT;
                          }
                          else
                          {
                            //I can now control the servos
                            int servoAngle = adcValue / 88;
                            if(servoAngle > 180)
                            {
                              servoAngle = 180;
                            }
                            Serial1.print(servoAngle);
                            ringServo.write(servoAngle);
                          }
                          //CleanUp
                          for(int i=0; i<5; i++)
                            {
                              adcNum[i] = ' ';
                            }
                            index = 0;
                            changingCondition= ' ';
                        }
                      }
                    }
                  break;

                  case PINKY:
                         //Loop for waiting how much it has to flex, from the MSP via serial comm. Send an acknowledgment message, to notify the MSP that has the control of the servo motor
                    signalToMSP = PINKY + '0';
                    Serial1.println(signalToMSP);
                    //Serial.print("Signal sent:");
                   // Serial.println(signalToMSP);

                    //Starting reading the ADC signal
                    while(check!=EXT)
                    {
                      if(Serial1.available() > 0)
                      {
                        char c = Serial1.read();
                        //Serial.println(c);                                 
                        adcNum[index] = c;
                        index++;    
                        changingCondition = c;
                        isTrasmissed = true;  
                      }
                      else 
                      {
                        if(isTrasmissed && changingCondition == '\n')
                        {
                          int adcValue = atoi(adcNum);
                          //Serial.println(adcNum);
                          bool timeToExit = false;
                          //Check if it is an exit array
                          for(int i=0;i<index;i++)
                          {
                              if (adcValue == 20000)
                              {
                                  timeToExit = true;
                              }
                          }
                          if(timeToExit)
                          {
                              check = EXT;
                              selectedFinger= EXT;
                          }
                          else
                          {
                            //I can now control the servos
                            int servoAngle = adcValue / 88;
                            if(servoAngle > 180)
                            {
                              servoAngle = 180;
                            }
                            Serial1.print(servoAngle);
                            pinkyServo.write(servoAngle);
                          }
                          //CleanUp
                          for(int i=0; i<5; i++)
                            {
                              adcNum[i] = ' ';
                            }
                            index = 0;
                            changingCondition= ' ';
                        }
                      }
                    }
                  break;
                }

                //Exit section, to enable changing section
                 if(selectedFinger == EXT)
                 {
                  c = ' ';
                  command[0] = ' ';
                  speakingManual = false;
                 }             
             }
         break;

         case GES:
            //Gestures
         break;

         case EXT:
            //Exit
         break;
        }
      }
    }
  }
  }