
 #define PROFILE_WINDOWS
 #include "Wire.h"
 #include <MPU6050_light.h>
 float roll, pitch, yaw;
 MPU6050 mpu(Wire);
 unsigned long timer = 0;
 int pot1, pot2, pot3;
 float const damp = 0.6;
 float outx = 0;
 float outy = 0;
 float accx, accy, accz;
 const int numSamples = 119;
 int samplesRead = numSamples;
 int threshold = 20;
 
 

 void setup() {
   Serial.begin(115200);
   Wire.begin();
   byte status = mpu.begin();
   //Serial.print(F("MPU6050 status: "));
   //Serial.println(status);
   //while (status != 0) { } // stop everything if could not connect to MPU6050
   //Serial.println(F("Calculating offsets, do not move MPU6050"));
   delay(100);
   mpu.calcOffsets(true,true); // gyro and accelero
   delay(100);
   Serial.println("gX,gZ,gY,aX,aZ,aY,p1,p2,p3");

      //Serial.println("Done!\n");
 }

 void loop() {
    samplesRead = 0;

    if(touchRead(27) < threshold){
    delay(100);
    while(samplesRead < numSamples){


    mpu.update();
    yaw = mpu.getGyroX();
    roll = mpu.getGyroZ();
    pitch = mpu.getGyroY();
    accx = mpu.getAccX();
    accy = mpu.getAccY();
    accz = mpu.getAccZ();

     pot1 = analogRead(36);
     pot2 = analogRead(39);
     pot3 = analogRead(34);

        Serial.print(yaw);
        Serial.print(",");
        Serial.print(roll);
        Serial.print(",");
        Serial.print(pitch); 

        Serial.print(",");
        Serial.print(accx);
        Serial.print(",");
        Serial.print(accz);
        Serial.print(",");
        Serial.print(accy);

        Serial.print(",");
        Serial.print(pot1);
        Serial.print(",");
        Serial.print(pot2);
        Serial.print(",");
        Serial.println(pot3);

      samplesRead++;
      if(samplesRead == numSamples){
        Serial.println();
      }
    } 
    }
  }