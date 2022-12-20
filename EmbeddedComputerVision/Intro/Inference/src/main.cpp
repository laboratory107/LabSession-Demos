#include <Arduino.h>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model_quantized.h"

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

const int tensorArenaSize = 4096;
uint8_t tensorArena[tensorArenaSize];

int inferenceCount = 0;
const int inferencePerCycle = 30;
const float xRange = 2 * M_PI; // We'll go from 0 to 2 PI

void setup() {
  Serial.begin(115200);

  static tflite::MicroErrorReporter reporter; 
  tflite::ErrorReporter* errorReporter = &reporter;

  model = tflite::GetModel(model_quantized_tflite);

  static tflite::MicroMutableOpResolver<4> resolver;
  resolver.AddDequantize();
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddQuantize();

  static tflite::MicroInterpreter staticInterperter(model, resolver, tensorArena, tensorArenaSize, errorReporter);
  interpreter = &staticInterperter;

  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  float position = (float)inferenceCount / inferencePerCycle;
  float x = position * xRange;

  int8_t xQuantized = x / input->params.scale + input->params.zero_point; // We are using a quantized model, so let's rely on smaller datatypes
  input->data.int8[0] = xQuantized;

  interpreter->Invoke();

  int8_t yQuantized = output->data.int8[0];
  float y = (yQuantized - output->params.zero_point) * output->params.scale;   // Dequantize the output from integer to floating-point

  Serial.println(y, 10); // A very uninspired way to handle the output

  inferenceCount = inferenceCount <= inferencePerCycle ? inferenceCount + 1 : 0;
  delay(100);
}