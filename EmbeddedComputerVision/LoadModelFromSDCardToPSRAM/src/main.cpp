#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

uint8_t *modelBuffer, tensorPool;

static tflite::MicroErrorReporter micro_error_reporter;
static tflite::ErrorReporter *errorReporter = &micro_error_reporter;

/* Load model from the buffer
   Allocate estimated memory in PSRAM
   Allocate and return the actual memory used by the model
*/
size_t getUsedMemorySize(size_t arenaSize)
{
  const tflite::Model *tfModel = tflite::GetModel(modelBuffer);
  if (tfModel->version() != TFLITE_SCHEMA_VERSION)
    return -1;

  uint8_t *tensorArena = (uint8_t *)ps_malloc(arenaSize);
  if (!tensorArena)
    return -2;

  Serial.println((String) "Memory available in PSRAM : " + ESP.getFreePsram());

  tflite::AllOpsResolver resolver;
  tflite::MicroInterpreter interpreter(tfModel, resolver, tensorArena, arenaSize, errorReporter);

  TfLiteStatus allocate_status = interpreter.AllocateTensors();
  if (allocate_status != kTfLiteOk)
  {
    free(tensorArena);
    return 0;
  } 
  else 
    return interpreter.arena_used_bytes();
}

/* Load .tflite model byte by byte from SD card */
bool loadModel()
{
  File file = SD.open("/singlepose.tflite", FILE_READ);
  size_t modelSize = file.size();

  Serial.print("Found model on filesystem of size ");
  Serial.println(modelSize);

  modelBuffer = (uint8_t *)ps_malloc(modelSize);

  for (size_t i = 0; i < modelSize; i++)
    modelBuffer[i] = file.read();

  file.close();

  return modelSize > 0;
}

SPIClass spi = SPIClass(VSPI);
bool initSDCard()
{
  short SCK = 19, MISO = 23, MOSI = 18, CS = 5; 
  spi.begin(SCK, MISO, MOSI, CS);

  return SD.begin(CS, spi, 80000000);
}

void setup()
{
  Serial.begin(115200);

  if (!psramInit())
  {
    Serial.println("Could not find PSRAM (check your platformio.ini)");
    return;
  }

  if (!initSDCard())
  {
    Serial.println("Cannot init SD");
    return;
  }

  if (!loadModel())
  {
    Serial.println("Could not load model from SD card");
    return;
  }

  Serial.println(getUsedMemorySize(1000000));
}

void loop()
{
}