
#include <Wire.h>

#include "utility/Adafruit_PWMServoDriver.h"
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <XBOXUSB.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
XBOXUSB Xbox(&Usb);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);

//define the min and max for each servo attached to each channel
#define SERVOMIN_0  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_0  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_1  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_1  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_2  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_2  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_3  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_3  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_4  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_4  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_5  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_5  540 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_6  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_6  540 // this is the 'maximum' pulse length count (out of 4096)



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
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz 

  // Set the speed to start, from 0 (off) to 255 (max speed)
  Motor1->setSpeed(150);
  Motor1->run(FORWARD);  // turn on motor
  Motor1->run(RELEASE);

  Motor3->setSpeed(150);
  Motor3->run(FORWARD);  // turn on motor
  Motor3->run(RELEASE);
}
void loop() {
  Usb.Task();
  /* int motion = 1;*/

  //CHASSIS MOTORS//
  if (Xbox.Xbox360Connected) {
    if (Xbox.getButtonPress(L2) || Xbox.getButtonPress(R2)) {
      Serial.print("L2: ");
      Serial.print(Xbox.getButtonPress(L2));
      Motor3->run(FORWARD);
      Motor3->setSpeed(Xbox.getButtonPress(L2));  

      Serial.print("\tR2: ");
      Serial.println(Xbox.getButtonPress(R2));
      Motor1->run(FORWARD);
      Motor1->setSpeed(Xbox.getButtonPress(R2));

      Xbox.setRumbleOn(Xbox.getButtonPress(L2), Xbox.getButtonPress(R2));
    } 
    else
      Xbox.setRumbleOn(0, 0);
    Motor1->run(RELEASE);
    Motor3->run(RELEASE);

    //ROBOT ARMS//
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX));
        Serial.print("\t");

        int val_0 = Xbox.getAnalogHat(LeftHatX);
        val_0 = map(val_0, -33000, 33000, SERVOMIN_0, SERVOMAX_0);
        static int pos_0 = val_0;
        if (val_0 < pos_0){
          //pos_0 =pos_0 - motion;
          pos_0 =pos_0 - 1;
          pos_0 > SERVOMIN_0;
          pwm.setPWM(0,0,pos_0);
        }
        if (val_0 > pos_0){
          //pos_0 = pos_0 + motion;
          pos_0 =pos_0 + 1;
          pos_0 < SERVOMAX_0;
          pwm.setPWM(0,0,pos_0);
        }
        if (val_0 == pos_0){
          pos_0 = pos_0;
          pwm.setPWM(0,0,pos_0);
        }
      }
      
      //Assumes 1 and 2 work in tandem and have the same limits
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");

        int val_1 = Xbox.getAnalogHat(LeftHatY);
        val_1 = map(val_1, -33000, 33000, SERVOMIN_1, SERVOMAX_1);
        static int pos_1 = val_1;
        if (val_1 < pos_1){
          pos_1 =pos_1 - 1; 
          pos_1 > SERVOMIN_1;
          pwm.setPWM(1,0,pos_1);
          pwm.setPWM(2,0,pos_1);
        }
        if (val_1 > pos_1){
          pos_1 = pos_1 + 1; 
          pos_1 < SERVOMAX_1;
          pwm.setPWM(1,0,pos_1);
          pwm.setPWM(2,0,pos_1);
        }
        if (val_1 == pos_1){
          pos_1 = pos_1;
          pwm.setPWM(1,0,pos_1);
           pwm.setPWM(2,0,pos_1);
        }
     }
      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");

        int val_4 = Xbox.getAnalogHat(RightHatX);
        val_4 = map(val_4, -33000, 33000, SERVOMIN_4, SERVOMAX_4);
        static int pos_4 = val_4;
        if (val_4 < pos_4){
          pos_4 =pos_4 - 1; 
          pos_4 > SERVOMIN_4;
          pwm.setPWM(4,0,pos_4);
        }
        if (val_4 > pos_4){
          pos_4 = pos_4 + 1; 
          pos_4 < SERVOMAX_4;
          pwm.setPWM(4,0,pos_4);
        }
        if (val_4 == pos_4){
          pos_4 = pos_4;
          pwm.setPWM(4,0,pos_4);
        }
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));

        int val_3 = Xbox.getAnalogHat(RightHatY);
        val_3 = map(val_3, -33000, 33000, SERVOMIN_3, SERVOMAX_3);
        static int pos_3 = val_3;
        if (val_3 < pos_3){
          pos_3 =pos_3 - 1; 
          pos_3 > SERVOMIN_3;
          pwm.setPWM(3,0,pos_3);
        }
        if (val_3 > pos_3){
          pos_3 = pos_3 + 1; 
          pos_3 < SERVOMAX_3;
          pwm.setPWM(3,0,pos_3);
        }
        if (val_3 == pos_3){
          pos_3 = pos_3;
          pwm.setPWM(3,0,pos_3);
        }
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
    if (Xbox.getButtonClick(L3)){
      Serial.println(F("L3"));
      for (uint16_t pulselen = SERVOMIN_5; pulselen < SERVOMAX_5; pulselen++) {
        pwm.setPWM(5, 0, pulselen);
      }
      for (uint16_t pulselen = SERVOMAX_6; pulselen > SERVOMIN_6; pulselen--) {
        pwm.setPWM(6, 0, pulselen);
      }
    }
    if (Xbox.getButtonClick(R3)){
      Serial.println(F("R3"));
      for (uint16_t pulselen = SERVOMAX_5; pulselen > SERVOMIN_5; pulselen--) {
        pwm.setPWM(5, 0, pulselen);
      }
      for (uint16_t pulselen = SERVOMIN_6; pulselen < SERVOMAX_6; pulselen++) {
        pwm.setPWM(6, 0, pulselen);
      }
    }
    if (Xbox.getButtonClick(L1)){
      Serial.println(F("L1"));
      Motor3->run(BACKWARD);
      Motor3->setSpeed(100);
      Motor1->run(BACKWARD);
      Motor1->setSpeed(100);
      delay(1000);
    }
    if (Xbox.getButtonClick(R1)){
      Serial.println(F("R1"));
      Motor1->run(RELEASE);
      Motor3->run(RELEASE);
    }
    if (Xbox.getButtonClick(XBOX)) {
      Xbox.setLedMode(ROTATING);
      Serial.println(F("Xbox"));
    }

    if (Xbox.getButtonClick(A)){
      Serial.println(F("A"));
      //FACTORY RESET//
      pwm.setPWM(0, 0, 400);
      pwm.setPWM(1, 0, 400);
      pwm.setPWM(2, 0, 400);
      pwm.setPWM(3, 0, 400);
      pwm.setPWM(4, 0, 400);
      pwm.setPWM(5, 0, SERVOMAX_5);
      pwm.setPWM(6, 0, SERVOMIN_6);
    }
    if (Xbox.getButtonClick(B)){
      Serial.println(F("B"));
      /*motion = motion - 1;
       if (motion < 0) motion = 1;
       Serial.print(motion);*/
    }
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y)){
      Serial.println(F("Y"));
      /*motion = motion + 1;
       if (motion > 10) motion = 10;
       Serial.print(motion);*/
    }
  }
  delay(1);
}











