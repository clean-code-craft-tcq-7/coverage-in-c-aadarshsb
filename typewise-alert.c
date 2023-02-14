#include "typewise-alert.h"
#include <stdio.h>

//static function prototype
static const stCoolingLimits_t GetLimitsFromCoolingType(CoolingType f_enumCoolingType);


static const stCoolingLimits_t GetLimitsFromCoolingType(const CoolingType f_enumCoolingType)
{
  return arrtyMap[f_enumCoolingType].CoolingLimits;
}

const MapCoolerTypeToLimits_t* const arrtyCoolingSystemToLimitMap[NO_OF_COOLING_TYPE] = 
{
 {PASSIVE_COOLING, {0U, 35U}},
 {HI_ACTIVE_COOLING, {0U, 45U}},
 {MED_ACTIVE_COOLING, {0U, 40U}}
};

BreachType inferBreach(double value, stCoolingLimits_t f_stCoolingLimits) {
  if(value < f_stCoolingLimits.lowerLimit) {
    return TOO_LOW;
  }
  if(value > f_stCoolingLimits.upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach( \
    CoolingType coolingType, double temperatureInC) 
{
  stCoolingLimits_t stCoolLimits = arrtyCoolingSystemToLimitMap(coolingType);
#if 0
  int lowerLimit = 0;
  int upperLimit = 0;
  switch(coolingType) {
    case PASSIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 35;
      break;
    case HI_ACTIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 45;
      break;
    case MED_ACTIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 40;
      break;
  }
#endif
  return inferBreach(temperatureInC, stCoolLimits);
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  switch(breachType) {
    case TOO_LOW:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too low\n");
      break;
    case TOO_HIGH:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too high\n");
      break;
    case NORMAL:
      break;
  }
}
