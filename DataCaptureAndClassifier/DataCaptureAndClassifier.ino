#define PROFILE_WINDOWS

#include <TensorFlowLite_ESP32.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
#include "model.h"

const int numSamples = 119;
int ind = 3;
float const damp = 0.9;

int samplesRead = numSamples;

tflite::MicroErrorReporter tflErrorReporter;
tflite::AllOpsResolver tflOpsResolver;
const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

const char* GESTURES[] = {
  "maximizev3",
  "minimizev3",
  "backv3"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  Serial.begin(38400);
  Wire.begin();
  byte status = mpu.begin();
  delay(100);
  mpu.calcOffsets(true, true);
  delay(100);

  tflModel = tflite::GetModel(model);
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);
  tflInterpreter->AllocateTensors();
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() {
  mpu.update();
  float aX, aY, aZ, gX, gY, gZ, p1, p2, p3;
  samplesRead = 3;
  ind = 3; 
  if(touchRead(27) < 20){
 
    while (samplesRead < numSamples) {

      mpu.update();
      delay(10);

      gX=mpu.getGyroX();
      gZ=mpu.getGyroZ();
      gY=mpu.getGyroY();
      aX=mpu.getAccX();
      aZ=mpu.getAccZ();
      aY=mpu.getAccY();
      p1=analogRead(36);
      p2=analogRead(39);
      p3=analogRead(34);     
      
      tflInputTensor->data.f[samplesRead * 9 + 0] = (gX + 600.0) / 1000.0;
      tflInputTensor->data.f[samplesRead * 9 + 1] = (gZ + 600.0) / 1000.0;
      tflInputTensor->data.f[samplesRead * 9 + 2] = (gY + 600.0) / 1000.0;
      tflInputTensor->data.f[samplesRead * 9 + 3] = (aX + 2.5) / 5.0;
      tflInputTensor->data.f[samplesRead * 9 + 4] = (aZ + 2.5) / 5.0;
      tflInputTensor->data.f[samplesRead * 9 + 5] = (aY + 2.5) / 5.0;
      tflInputTensor->data.f[samplesRead * 9 + 6] = p1 / 2500.0;
      tflInputTensor->data.f[samplesRead * 9 + 7] = p2 / 2500.0;
      tflInputTensor->data.f[samplesRead * 9 + 8] = p3 / 2500.0;
      
      samplesRead++;

      if (samplesRead == numSamples) {
        
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();

        if (invokeStatus != kTfLiteOk) {

          Serial.println("Invoke failed!");
          while (1);
          return;

        }

        if(tflOutputTensor->data.f[0] > tflOutputTensor->data.f[1] && tflOutputTensor->data.f[0] > tflOutputTensor->data.f[2]){
          ind=0;
        }
        else if(tflOutputTensor->data.f[1] > tflOutputTensor->data.f[0] && tflOutputTensor->data.f[1] > tflOutputTensor->data.f[2]){
          ind=1;
        }
        else{
          ind=2;
        }
      } 
    }
  }
  
  int yaw = (mpu.getAngleX()*10000);
  int roll = (mpu.getAngleZ()*10000);
  yaw = constrain(yaw, -250000, 250000);
  roll = constrain(roll, -300000, 300000);
  int posx = map(yaw, -250000, 250000, 1200, 0);
  int posy = map(roll, -300000, 300000, 1920, 0);

  Serial.print(posx);
  Serial.print(",");
  Serial.print(posy);
  Serial.print(",");
  
  if(touchRead(4)<20){
    Serial.print(1);
  }
  else{
    Serial.print(0);
  }
  Serial.print(",");
  Serial.println(ind);

  
  


}