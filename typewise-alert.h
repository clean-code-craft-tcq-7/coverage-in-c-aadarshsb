#pragma once

//---------------------------Typedefs-----------------------------------
typedef enum {
  PASSIVE_COOLING = 0,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  NO_OF_COOLING_TYPE
} enumCoolingType_t;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH,
  NO_OF_BREACH_TYPES
} enumBreachType_t;

typedef struct
{
  double LowerLimit;
  double UpperLimit;
}stCoolingLimits_t;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL,
  NO_OF_ALERT_TYPES
} enumAlertTarget_t;

typedef struct {
  enumCoolingType_t coolingType;
  char brand[48];
} stBatteryCharacter_t;

//----------------------Function prototypes---------------------------------------
enumBreachType_t inferBreach(const double value, const stCoolingLimits_t f_CoolingLimit);
enumBreachType_t classifyTemperatureBreach(const enumCoolingType_t coolingType,const double temperatureInC);
void checkAndAlert(const enumAlertTarget_t alertTarget,const stBatteryCharacter_t batteryChar,const double temperatureInC);
void sendToController(enumCoolingType_t breachType);
void sendToEmail(enumCoolingType_t breachType);
