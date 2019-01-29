/*************************************************************
   Name: Test_KB_Motor_Module
   Device: KidBrigth32 V1.3
   Chip: ESP32
   Compile: NodeMCU-32s
   Create: 25-5-18
*************************************************************/

#include <Wire.h>
#include "KB_Motor.h"

KB_MOTOR i2c_motor; //Create a new KB_MOTOR instance

uint8_t motor1_addr = 0x66; //i2c Address Motor Control Motor 1 
uint8_t motor2_addr = 0x68; //i2c Address Motor Control Motor 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  i2c_motor.begin(motor1_addr, motor2_addr); //init Motor Module 2CH
}

void loop() {
  Serial.println("Move forward");
  i2c_motor.faultCheck1(motor1_addr); // Check Error Parameter Sent to Module
  i2c_motor.i2c_motor_write(1, 255, 1); //i2c_motor_write(int ch, int mspeed, int stat) speed min-max = 0-255 / channel 1,2 / { stat (0 = stop) ,(1 = Forward) ,(2 = Backward) }
  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.faultCheck2(motor2_addr); 
  i2c_motor.i2c_motor_write(2, 255, 1);
  i2c_motor.faultCheck2(motor2_addr);

  delay(5000);
  Serial.println("Stop");
  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.i2c_motor_write(1, 0, 0);
  i2c_motor.faultCheck1(motor1_addr);
   i2c_motor.faultCheck2(motor2_addr);
  i2c_motor.i2c_motor_write(2, 0, 0);
  i2c_motor.faultCheck2(motor2_addr);
  
  delay(1000);
  Serial.println("Move backward");
  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.i2c_motor_write(1, 255, 2);
  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.faultCheck2(motor2_addr);
  i2c_motor.i2c_motor_write(2, 255, 2);
  i2c_motor.faultCheck2(motor2_addr);

  delay(5000);

  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.i2c_motor_write(1, 0, 0);
  i2c_motor.faultCheck1(motor1_addr);
  i2c_motor.faultCheck2(motor2_addr);
  i2c_motor.i2c_motor_write(2, 0, 0);
  i2c_motor.faultCheck2(motor2_addr);
  delay(1000);
}
