// http://donalmorrissey.blogspot.com/2010/04/putting-arduino-diecimila-to-sleep.html

#include <avr/sleep.h>
#include <avr/power.h>

int pin2 = 2;

/***************************************************
 *  Name:        pin2Interrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt  
 *
 ***************************************************/
void pin2Interrupt(void)
{
  /* This will bring us back from sleep. */
}

/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();
  
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the Arduino.           
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  
  /* Setup the pin direction. */
  pinMode(pin2, INPUT);

  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(digitalPinToInterrupt(pin2), pin2Interrupt, CHANGE);
  
  Serial.println("Initialisation complete.");
}

/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
int seconds=0;
void loop()
{
  delay(1000);
  seconds++;
  
  Serial.print("Awake for ");
  Serial.print(seconds, DEC);
  Serial.println(" second");
  
  if(seconds == 3)
  {
    Serial.println("Entering sleep");
    delay(200);
    seconds = 0;
    enterSleep();
  }
}
