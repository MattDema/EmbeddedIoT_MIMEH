#include <Servo.h>
#include <ArduinoBLE.h>
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
const int ledPin = LED_BUILTIN; // pin to use for the LED
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

  Serial.begin(9600);

  while (!Serial);
  pinMode(ledPin, OUTPUT);
  if (!BLE.begin()) 
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("UNO R4 LED");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  switchCharacteristic.writeValue(0);
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}

void loop() 
{
  BLEDevice central = BLE.central();

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (switchCharacteristic.written()) 
      {
        switch (switchCharacteristic.value()) 
        {
          case 5:
                Serial.println("Thumb on on");
                digitalWrite(ledPin, HIGH);
                if(thumbServo.read() == 180)
                {
                  thumbServo.write(0);
                }
                else
                {
                  thumbServo.write(180);
                }
          break;
          case 6:
                Serial.println("Index on on");
                digitalWrite(ledPin, HIGH);
                if(indexServo.read() == 180)
                {
                  indexServo.write(0);
                }
                else
                {
                  indexServo.write(180);
                }
          break;
          case 7:
                Serial.println("Middle on on");
                digitalWrite(ledPin, HIGH);
               if(middleServo.read() == 180)
                {
                  middleServo.write(0);
                }
                else
                {
                  middleServo.write(180);
                }         
            break;
          case 8:
                Serial.println("Ring on on");
                digitalWrite(ledPin, HIGH);
                if(ringServo.read() == 180)
                {
                  ringServo.write(0);
                }
                else
                {
                  ringServo.write(180);
                }
          break;
          case 9:
                Serial.println("Pinky on on");
                digitalWrite(ledPin, HIGH);
                if(pinkyServo.read() == 180)
                {
                  pinkyServo.write(0);
                }
                else
                {
                  pinkyServo.write(180);
                }
          break;
          case 10:
            //peace sign
            Serial.println("Peace sign on");
            if(thumbServo.read() == 180 && ringServo.read() == 180 && pinkyServo.read() == 180)
            {
                thumbServo.write(0);
                ringServo.write(0);
                pinkyServo.write(0);
            }
            else
            {
                thumbServo.write(180);
                ringServo.write(180);
                pinkyServo.write(180);
            }
          break;
          case 11:
            //California sign
            Serial.println("California sign on");
            if(indexServo.read() == 180 && middleServo.read() == 180 && ringServo.read() == 180)
            {
                indexServo.write(0);
                middleServo.write(0);
                ringServo.write(0);
            }
            else
            {
                indexServo.write(180);
                middleServo.write(180);
                ringServo.write(180);
            }
          break;

          default:
            thumbServo.write(0);
            indexServo.write(0);
            middleServo.write(0);
            ringServo.write(0);
            pinkyServo.write(0);
        } 
      }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

}