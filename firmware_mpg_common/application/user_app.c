/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  LedPWM(RED, LED_PWM_5); 
  LedPWM(WHITE, LED_PWM_5);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE); 
  LedOn(LCD_RED);
  LedOff(LCD_GREEN);
  LedOff(LCD_BLUE);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static LedNumberType u8LedControl[] = {WHITE, PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED};
  static bool boolflag = TRUE;
  static u8 u8CurrentLedIndex  = 1;
  static u16 u16Counter = 0;
  static u8 u8LedCurrentLevel  = 10;  
  u16Counter++;
  if(u16Counter == 500)
  {
    u16Counter = 0;
    if(boolflag)
    {
      if(u8CurrentLedIndex != 0)
      {
        LedOff(u8LedControl[u8CurrentLedIndex-1]);
        LedOff(u8LedControl[8-u8CurrentLedIndex]);
      }
      LedPWM( (LedNumberType)u8LedControl[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
      LedPWM( (LedNumberType)u8LedControl[7-u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel); 
    }
    else
    {
      if(u8CurrentLedIndex != 3)
      {
        LedOff(u8LedControl[u8CurrentLedIndex+1]);
        LedOff(u8LedControl[6-u8CurrentLedIndex]);
      }
      LedPWM( (LedNumberType)u8LedControl[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
      LedPWM( (LedNumberType)u8LedControl[7-u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
    }
    
    if(u8CurrentLedIndex % 2 == 0)
    {
      LedOn(LCD_RED);
      LedOff(LCD_BLUE);
    }
    else
    {
      LedOn(LCD_BLUE);
      LedOff(LCD_RED);
    }
    
    if(u8CurrentLedIndex == 3 || u8CurrentLedIndex == 0)
    {
      boolflag = !boolflag;  
    }
    if(boolflag)
    {
      u8CurrentLedIndex++;
      u8LedCurrentLevel += 5;
    }
    else
    {
      u8CurrentLedIndex--;
      u8LedCurrentLevel -= 5;      
    }
  }
} /* end UserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
