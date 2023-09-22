#include <Wire.h>
#include <PWMServo.h>

//PWMServo myservo;
PWMServo servo1;
PWMServo servo2;
PWMServo servo3;
float RateRoll, RatePitch, RateYaw;
//float RateYaw, RatePitch;
//float RateYaw1;
float RateCalibrationRoll, RateCalibrationPitch,
RateCalibrationYaw;
//float RateCalibrationYaw, RateCalibrationPitch;
int RateCalibrationNumber; 


int sensorValue1 = 0;  // value read from the pot, Yaw
int outputValue1 = 0;  // value output to the PWM (analog out)
int sensorValue2 = 0;  // value read from the pot, Pitch
int outputValue2 = 0;  // value output to the PWM (analog out)
int sensorValue3 = 0;  // value read from the pot, Roll
int outputValue3 = 0;  // value output to the PWM (analog out)
void gyro_signals(void) {
Wire.beginTransmission(0x68);//Roll
Wire.write(0x1A);
Wire.write(0x05);
Wire.endTransmission();

Wire.beginTransmission(0x68);//Pitch
Wire.write(0x1B);
Wire.write(0x08);
Wire.endTransmission();

Wire.beginTransmission(0x68);//Yaw
Wire.write(0x43);
Wire.endTransmission(); 
Wire.requestFrom(0x68,6);
int16_t GyroX=Wire.read()<<8 | Wire.read();
int16_t GyroY=Wire.read()<<8 | Wire.read();
int16_t GyroZ=Wire.read()<<8 | Wire.read();
RateRoll=(float)GyroX/65.5;
RatePitch=(float)GyroY/65.5;
RateYaw=(float)GyroZ/65.5;


}
void setup() {
//myservo.attach(SERVO_PIN_A); 
  servo1.attach(6); // Replace pin_number_1 with the actual pin number
  servo2.attach(7); // Replace pin_number_2 with the actual pin number
  servo3.attach(8); // Replace pin_number_3 with the actual pin number
  //servo3.attach(12);
Serial.begin(57600);
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);
Wire.setClock(400000);
Wire.begin();
delay(250);
Wire.beginTransmission(0x68);
Wire.write(0x6B);
Wire.write(0x00);
Wire.endTransmission();
for (RateCalibrationNumber=0;
RateCalibrationNumber<2000; 
RateCalibrationNumber ++) {
gyro_signals();
RateCalibrationRoll+=RateRoll;
RateCalibrationPitch+=RatePitch;
RateCalibrationYaw+=RateYaw;
delay(1);
}
RateCalibrationRoll/=2000;
RateCalibrationPitch/=2000;
RateCalibrationYaw/=2000; 
//myservo.attach(SERVO_PIN_A2);         // attaches the servo on pin 9 to the servo object
}
void loop() {
gyro_signals();

RateRoll-=RateCalibrationRoll;
RatePitch-=RateCalibrationPitch;
RateYaw-=RateCalibrationYaw;

sensorValue1 = RateRoll;
outputValue1 = map(sensorValue1, -10, 10, 0, 255);
servo1.write(outputValue1); 

sensorValue2 = RatePitch;
outputValue2 = map(sensorValue2, -10, 10, 0, 255);
servo2.write(outputValue2); 

sensorValue3 = RateYaw;
outputValue3 = map(sensorValue3, -10, 10, 0, 255);
servo3.write(outputValue3); 

Serial.print("Roll rate [°/s]= ");
Serial.print(RateRoll);

Serial.print(" Pitch Rate [°/s]= ");
Serial.print(RatePitch);

Serial.print(" Yaw Rate [°/s]= ");
Serial.println(RateYaw);

Serial.print(" Motor Step Roll = ");
Serial.print(outputValue1);

Serial.print(" Motor Step Pitch = ");
Serial.print(outputValue2);

Serial.print(" Motor Step Yaw = ");
Serial.print(outputValue3);


delay(500);
}