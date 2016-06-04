/**********************************************************************************************************************
File: user_app2.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app2 as a template:
 1. Copy both user_app2.c and user_app2.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app2" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp2" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP2" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];                     /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                     /* From debug.c  */

extern volatile bool G_bUserAppCharacterCorrectFlag;  /*From user_app.c*/

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */


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
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp2SM_Idle(void)
{
  static u16 u16CounterFor5s = 0;   
  static u32  u32CorrectTimesIndex = 0;
  static u8  u8MessageForDetectedTimes[] = "'Xavier' has been detected: ";
  static u8 u8TuneTable[] = {2,1,0,1,2,2,2,1,1,1,2,3,3,2,1,0,1,2,2,2,2,1,1,2,1,0};
  static u8 u8CurrentTuneIndex = 0;
  
  if(G_bUserAppCharacterCorrectFlag)
  {
    /*when all characters are correct , set the counter*/
    u16CounterFor5s = 5000;
    G_bUserAppCharacterCorrectFlag = FALSE;
    u32CorrectTimesIndex++;
    
   /*show many times my name has been detected*/
    DebugLineFeed();
    DebugPrintf(u8MessageForDetectedTimes);
    DebugPrintNumber(u32CorrectTimesIndex);
    DebugLineFeed();
    
    /*initialize leds and the buzzer*/
    LedOn(WHITE);
    LedOff(PURPLE);
    LedOn(BLUE);
    LedOff(CYAN);
    LedOn(GREEN);
    LedOff(YELLOW);
    LedOn(ORANGE);
    LedOff(RED); 
    PWMAudioOn(BUZZER1);
    u8CurrentTuneIndex = 0; 
  }
  
  /*The sound and LEDs should run for 5 seconds*/
  if(u16CounterFor5s)
  {
    u16CounterFor5s--;
    
    /*leds blink*/
    if(u16CounterFor5s % 250 == 0)
    {
      LedToggle(WHITE);
      LedToggle(PURPLE);
      LedToggle(BLUE);
      LedToggle(CYAN);
      LedToggle(GREEN);
      LedToggle(YELLOW);
      LedToggle(ORANGE);
      LedToggle(RED);
      
      /*choose the frequency according to the TuneTable*/
      switch(u8TuneTable[u8CurrentTuneIndex]) 
      { 
        case 0:PWMAudioSetFrequency(BUZZER1, 262); 
              break; 
        case 1:PWMAudioSetFrequency(BUZZER1, 294); 
              break; 
        case 2:PWMAudioSetFrequency(BUZZER1, 330); 
              break; 
        case 3:PWMAudioSetFrequency(BUZZER1, 392); 
              break;  
        default:PWMAudioOff(BUZZER1);  
              break; 
     } 
         
     if(u8CurrentTuneIndex == 25) 
     { 
       u8CurrentTuneIndex = 0; 
     } 
     else 
     {     
       u8CurrentTuneIndex++; 
     } 

    }    
  }
  else
  {
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    PWMAudioOff(BUZZER1);
  }
 
} /* end UserApp2SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
