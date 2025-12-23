#include "HiWonderMotorController.h"

void setup()
{
  Wire.begin();
  setMotorSpeed(0, 0);  // Ensure both motors are stopped
}

void loop()
{
  SetMotorSpeed(50,50);
  delay(1000);
  SetMotorSpeed(0,0);
  delay(1000);
  SetMotorSpeed(-50,-50);
  delay(1000);
}
