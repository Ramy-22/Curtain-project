#include <Servo.h>

// Pins
#define SWITCH 4
#define BUTTON 7
#define SERVO 9
#define GREEN 3 // Green LED (when turned on, means the curtain is closed)
#define RED 5   // Red LED (when turned on, means the curtain is opened)
#define LDR_PIN A0

// Curtain state

#define CURTAIN_STATE_OPENED 2
#define CURTAIN_STATE_CLOSED 3


#define AUTO 1
#define MANUAL 10
#define STATE_UNKNOWN 88

// Servo variables
Servo servo;
int angle = 40;


// Button variables
int buttonState = STATE_UNKNOWN;
int oldButtonState = STATE_UNKNOWN;

// Switch variables
int oldSwitchState = STATE_UNKNOWN;
int switchState = STATE_UNKNOWN;

int ldr_val = STATE_UNKNOWN;

int curtainState = STATE_UNKNOWN;

int operationMode = STATE_UNKNOWN;

// Functions
void open_curtain(void);
void close_curtain(void);

void setup() {
  
  Serial.begin(9600);
  servo.attach(9);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode (SWITCH, INPUT);

  // LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);

  close_curtain();
}

void loop()
{
  switchState = digitalRead(SWITCH);
  if(switchState != oldSwitchState)
  {
    //Serial.println(switchState);
    oldSwitchState = switchState;
    switch(switchState)
    {
      case HIGH:
        operationMode = AUTO; // Auto
        break;
      case LOW:
        operationMode = MANUAL; // Manual
        break;
    }
  }
  if(operationMode == MANUAL)
  {
    buttonState = digitalRead(BUTTON);
    Serial.println("Button state is\n");
    Serial.println(buttonState);
    delay(100);
    if (buttonState == HIGH)
    {
      if(curtainState == CURTAIN_STATE_OPENED || curtainState = STATE_UNKNOWN)
      {
        close_curtain();
      }
      else if(curtainState == CURTAIN_STATE_CLOSED || curtainState = STATE_UNKNOWN)
      {
        open_curtain();
      }
    }
    
  }
  else if(operationMode == AUTO)
  {
    ldr_val = analogRead(LDR_PIN);
    //Serial.println(ldr_val); //TEST
    if(ldr_val > 480)
    {
      close_curtain();
    }
    else if(ldr_val < 400)
    {
      open_curtain();
    }
  }
}



void close_curtain(void)
{
  if(curtainState != CURTAIN_STATE_CLOSED)
  {
        for(angle = 120; angle > 40; angle--)
        {
          servo.write(angle);
          delay(15);
        }
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, HIGH);
        curtainState = CURTAIN_STATE_CLOSED;
  }
}

void open_curtain(void)
{
  if(curtainState != CURTAIN_STATE_OPENED)
  {
        for(angle = 40; angle < 120; angle++)
        {
          servo.write(angle);
          delay(15);
        }
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        curtainState = CURTAIN_STATE_OPENED;
  }
}
