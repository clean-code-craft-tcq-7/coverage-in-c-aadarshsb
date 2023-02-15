#include "typewise-alert.h"
#include <stdio.h>
//-------------------static function prototypes------------------------------
static inline void sendEmailForLowBreach(const char* );
static inline void SendEmailForHighBreach(const char* );
static inline void SendEmailForNormalDummy(const char* );
static inline void PrintInvaildEmail(void);

//--------------------------constants-------------------------------------
//Mapping array to avoid state machine 
const stCoolingLimits_t* const arrtyCoolingSystemToLimitMap[NO_OF_COOLING_TYPE] = 
{
 [PASSIVE_COOLING] = {.LowerLimit = 0U, .UpperLimit = 35U},
 [HI_ACTIVE_COOLING] = {.LowerLimit = 0U, .UpperLimit = 45U},
 [MED_ACTIVE_COOLING] = {.LowerLimit = 0U, .UpperLimit = 40U}
};

const void (*EmailMapFnPointer[NO_OF_BREACH_TYPES])(char*)= 
{
  sendEmailForLowBreach,
  SendEmailForHighBreach,
  SendEmailForNormalDummy
};

const void (*AlertMessageTypeFnPointer[NO_OF_ALERT_TYPES])(enumBreachType_t)= 
{
  sendToEmail,
  sendToEmail
};

//------------------------function def--------------------------------------------
enumBreachType inferBreach(double f_dTempInC, stCoolingLimits_t f_stCoolingLimits) {
  if(f_dTempInC < f_stCoolingLimits.LowerLimit) {
    return TOO_LOW;
  }
  if(f_dTempInC > f_stCoolingLimits.UpperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

enumBreachType classifyTemperatureBreach(enumCoolingType_t coolingType, double temperatureInC) 
{
  return inferBreach(temperatureInC, arrtyCoolingSystemToLimitMap[coolingType]);
}

void checkAndAlert(enumAlertTarget_t alertTarget, stBatteryCharacter_t batteryChar, double temperatureInC) 
{
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  if(alertTarget < NO_OF_ALERT_TYPES)
    (*AlertMessageTypeFnPointer)[alertTarget](breachType);
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
    (*EmailMapFnPointer)[breachType](recepient);
}

//------------------------static function defintion----------------
static inline void sendEmailForLowBreach(const char* f_recepient)
{
  if(f_recepient != NULL) //avoid null pointer deferencing
  {
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too low\n");
  }
  else
    PrintInvaildEmail();
}

static inline void SendEmailForHighBreach(const char* f_recepient)
{
  if(f_recepient != NULL)
  {
    printf("To: %s\n", recepient);
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

//-=---------------------------backups-----------------
#if 0 
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

  switch(alertTarget) 
  {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }

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
