#include <Servo.h>

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
    ringServo.attach(8);    // Ring servo connected to pin 9
    pinkyServo.attach(9);   // Pinky servo connected to pin 8

//Setting Servos in ready position
    thumbServo.write(0);
    indexServo.write(0);
    middleServo.write(0);
    ringServo.write(0);
    pinkyServo.write(0);

    Serial.begin(9600);   // USB Serial Monitor
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) 
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
}
