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

extern u8 G_au8DebugScanfBuffer[];                     /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                     /* From debug.c  */

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
  static u8 u8MyName[] = "A3. Xavier YU";
  
  /*show my name at lcd line1*/
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,u8MyName); 
  
  /*change the lcd backlight to green*/
  LedOn(LCD_GREEN);
  LedOff(LCD_RED);
  LedOff(LCD_BLUE);
  
  /*test comment for github*/
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
  static u8 u8CounterFor5ms = 0;
  static u32 u32TotalNumberOfCharacter = 0;
  static u8 u8CurrentLcdAddress = LINE2_START_ADDR;
  static u8 u8TermInputBuffer[8] = {0};
  static u8 u8CorrectInputBuffer[8] = {0};
  
  static u8 u8CorrectInputIndex = 0;
  
  u8 u8MessageTotalCharacters[] = "Total characters received: ";
  u8 u8MessageCharacterCountClear[] = "Character count cleared!";
  u8 u8MessageCurrentBuffer[] = "Current letter buffer: ";
  u8 u8MyName[] = "Xavier";
    
  u8CounterFor5ms++;
  
  /*BUTTON0 clears the line of text so the next character starts from the beginning*/
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LCDClearChars(LINE2_START_ADDR, 20);
    u8CurrentLcdAddress = LINE2_START_ADDR;     
  }
  
  /*BUTTON1 prints the total number of characters received on the debug port */
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    DebugLineFeed();
    DebugPrintf(u8MessageTotalCharacters);
    DebugPrintNumber(u32TotalNumberOfCharacter);
    DebugLineFeed();
  }
  
  /*BUTTON2 clears the total character count and reports a message*/
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    u32TotalNumberOfCharacter = 0;
    DebugLineFeed();
    DebugPrintf(u8MessageCharacterCountClear);
    DebugLineFeed();
  }
 
  /*BUTTON3 prints the current letter buffer that is storing my name*/
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    DebugLineFeed();
    DebugPrintf(u8MessageCurrentBuffer);   
    DebugPrintf(u8CorrectInputBuffer);
    DebugLineFeed();
  }
  
  if(u8CounterFor5ms == 5)
  {
    u8CounterFor5ms = 0;
    
    /*check debug input every 5 ms*/
    if(DebugScanf(u8TermInputBuffer))
    {
      /*Keep track of the total number of characters that have been received*/
      u32TotalNumberOfCharacter++;
      
      /*when the cursor returns to the beginning,clear the whole line*/
      if(u8CurrentLcdAddress == LINE2_START_ADDR)
      {
        LCDClearChars(LINE2_START_ADDR, 20);   
      }
      
      /*display the character inputed on the lcd*/
      LCDMessage(u8CurrentLcdAddress,u8TermInputBuffer);
      
      if(u8CurrentLcdAddress >= LINE2_END_ADDR)
      {
        /*Once the screen is full, clear Line 2 and start again from the left*/
        u8CurrentLcdAddress = LINE2_START_ADDR;     
      }
      else
      {
        u8CurrentLcdAddress++;
      }
      
      /*Compare the character and ignore uppercase OR lowercase*/
      if(u8TermInputBuffer[0] == u8MyName[u8CorrectInputIndex] || u8TermInputBuffer[0] == u8MyName[u8CorrectInputIndex] - 32 ||u8TermInputBuffer[0] == u8MyName[u8CorrectInputIndex] + 32)
      {
        u8CorrectInputBuffer[u8CorrectInputIndex] = u8TermInputBuffer[0];
        if(u8CorrectInputIndex == 5)
        {
          /*if all characters are correct clear the buffer and set the flag*/
          u8CorrectInputIndex = 0;
          for(u8 i = 0;i < 8;i++)
          {
            u8CorrectInputBuffer[i] = 0;
          }
          LedOn(RED);
        }
        else
        {
          u8CorrectInputIndex++;
        }
      }
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
