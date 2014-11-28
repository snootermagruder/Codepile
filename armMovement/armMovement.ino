/*
 Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <XBOXUSB.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
XBOXUSB Xbox(&Usb);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//define the min and max for each servo attached to each channel
#define SERVOMIN_0  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_0  505 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_1  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_1  550 // this is the 'maximum' pulse length count (out of 4096)
void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.println(F("\r\nXBOX USB Library Started"));
  Serial.println("Servos online");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}
void loop() {
  Usb.Task();
  if (Xbox.Xbox360Connected) {
    if (Xbox.getButtonPress(L2) || Xbox.getButtonPress(R2)) {
      Serial.print("L2: ");
      Serial.print(Xbox.getButtonPress(L2));
      Serial.print("\tR2: ");
      Serial.println(Xbox.getButtonPress(R2));
      Xbox.setRumbleOn(Xbox.getButtonPress(L2), Xbox.getButtonPress(R2));
    } 
    else
      Xbox.setRumbleOn(0, 0);

    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX));
        Serial.print("\t");
        int val_0 = Xbox.getAnalogHat(LeftHatX);
        val_0 = map(val_0, -33000, 33000, SERVOMIN_0, SERVOMAX_0);
        pwm.setPWM(0,0,val_0);
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");
        int val_1 = Xbox.getAnalogHat(LeftHatY);
        val_1 = map(val_1, -33000, 33000, SERVOMIN_1, SERVOMAX_1);
        pwm.setPWM(1,0,val_1);
      }
      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");

        /*    int pos_0 = val_0;
         if (pos_0 < 0){
         pos_0 =pos_0 - 1; 
         pos_0 > SERVOMIN_0;
         pwm.setPWM(0,0,pos_0);
         }
         if (Xbox.getAnalogHat(RightHatX) > 0){
         pos_0 = pos_0 + 1; 
         pos_0 < SERVOMAX_0;
         pwm.setPWM(0,0,pos_0);
         }
         if (Xbox.getAnalogHat(RightHatX) == 0){
         pos_0 = pos_0;
         pwm.setPWM(0,0,pos_0);
         }*/
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));
        /*  int pos_1= 350;
         if (Xbox.getAnalogHat(RightHatY) < 0){
         pos_1 = pos_1 - 1 ; 
         pos_1 > SERVOMIN_1;
         pwm.setPWM(1,0,pos_1);
         }
         if (Xbox.getAnalogHat(RightHatY) > 0){
         pos_1 = pos_1 + 1 ; 
         pos_1 < SERVOMAX_1;
         pwm.setPWM(1,0,pos_1);
         }
         if (Xbox.getAnalogHat(RightHatY) == 0){
         pos_1 = pos_1;
         pwm.setPWM(1,0,pos_1);
         }*/
      }
      Serial.println();
    }

    if (Xbox.getButtonClick(UP)) {
      Xbox.setLedOn(LED1);
      Serial.println(F("Up"));
    }
    if (Xbox.getButtonClick(DOWN)) {
      Xbox.setLedOn(LED4);
      Serial.println(F("Down"));
    }
    if (Xbox.getButtonClick(LEFT)) {
      Xbox.setLedOn(LED3);
      Serial.println(F("Left"));
    }
    if (Xbox.getButtonClick(RIGHT)) {
      Xbox.setLedOn(LED2);
      Serial.println(F("Right"));
    }

    if (Xbox.getButtonClick(START)) {
      Xbox.setLedMode(ALTERNATING);
      Serial.println(F("Start"));
    }
    if (Xbox.getButtonClick(BACK)) {
      Xbox.setLedBlink(ALL);
      Serial.println(F("Back"));
    }
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));

    if (Xbox.getButtonClick(L1))
      Serial.println(F("L1"));
    if (Xbox.getButtonClick(R1))
      Serial.println(F("R1"));
    if (Xbox.getButtonClick(XBOX)) {
      Xbox.setLedMode(ROTATING);
      Serial.println(F("Xbox"));
    }

    if (Xbox.getButtonClick(A))
      Serial.println(F("A"));
    if (Xbox.getButtonClick(B))
      Serial.println(F("B"));
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y))
      Serial.println(F("Y"));
  }
  delay(1);
}





