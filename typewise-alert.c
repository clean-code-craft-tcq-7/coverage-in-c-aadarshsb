#include "typewise-alert.h"
#include <stdio.h>
//-------------------static function prototypes------------------------------
static inline void sendEmailForLowBreach(const char* );
static inline void SendEmailForHighBreach(const char* );
static inline void SendEmailForNormalDummy(const char* );
static inline void PrintInvaildEmail(void);

//--------------------------constants-------------------------------------
//Mapping array to avoid state machine 
const stCoolingLimits_t arrtyCoolingSystemToLimitMap[NO_OF_COOLING_TYPE] = 
{
 [PASSIVE_COOLING] = {.LowerLimit = 0.0, .UpperLimit = 35.0},
 [HI_ACTIVE_COOLING] = {.LowerLimit = 0.0, .UpperLimit = 45.0},
 [MED_ACTIVE_COOLING] = {.LowerLimit = 0.0, .UpperLimit = 40.0}
};

void (*EmailMapFnPointer[NO_OF_BREACH_TYPES])(const char*) = 
{
 sendEmailForLowBreach,
 SendEmailForHighBreach,
 SendEmailForNormalDummy
};

void (*AlertMessageTypeFnPointer[NO_OF_ALERT_TYPES])(enumBreachType_t) = 
{
 sendToController,
 sendToEmail
 };

//------------------------function def--------------------------------------------
enumBreachType_t inferBreach(double f_dTempInC, stCoolingLimits_t f_stCoolingLimits) {
  if(f_dTempInC < f_stCoolingLimits.LowerLimit) {
    return TOO_LOW;
  }
  if(f_dTempInC > f_stCoolingLimits.UpperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

enumBreachType_t classifyTemperatureBreach(enumCoolingType_t coolingType, double temperatureInC) 
{
  return inferBreach(temperatureInC, arrtyCoolingSystemToLimitMap[coolingType]);
}

void checkAndAlert(enumAlertTarget_t alertTarget, stBatteryCharacter_t batteryChar, double temperatureInC) 
{
  enumBreachType_t breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  if(alertTarget < NO_OF_ALERT_TYPES)
    (*AlertMessageTypeFnPointer[alertTarget])(breachType);
}

void sendToController(enumBreachType_t breachType) 
{
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(enumBreachType_t breachType) 
{
  const char* recepient = "a.b@c.com";
  if(breachType < NO_OF_BREACH_TYPES)
    (*EmailMapFnPointer[breachType])(recepient);
}

//------------------------static function defintion----------------
static inline void sendEmailForLowBreach(const char* f_recepient)
{
  if(f_recepient != NULL) //avoid null pointer deferencing
  {
    printf("To: %s\n", f_recepient);
    printf("Hi, the temperature is too low\n");
  }
  else
    PrintInvaildEmail();
}

static inline void SendEmailForHighBreach(const char* f_recepient)
{
  if(f_recepient != NULL)
  {
    printf("To: %s\n", f_recepient);
    printf("Hi, the temperature is too high\n");
  }
  else
    PrintInvaildEmail();
}

static inline void SendEmailForNormalDummy(const char* f_recepient)
{
}

static inline void PrintInvaildEmail(void)
{
  printf("Provide a proper email ID \n");
}
