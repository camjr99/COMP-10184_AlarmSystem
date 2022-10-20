// COMP-10184 – Mohawk College 
// This program simulatens an alarm application using a PIR sensor
// @author Cameron Randall
// @id 000820614
 
 //arduino library
#include <Arduino.h> 
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6
// alarm state definitions
#define ALARM_DISABLED 0
#define ALARM_ENABLED 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3

// alarm start variable
int alarmState;
// switch state variables
int switchState = HIGH;
int buttonSignal;
// PIR state variable
bool bPIR;
// timing/counter variables
unsigned long blinkStartMillis = 0;
unsigned long countStartMillis = 0;
unsigned long debugStartMillis = 0;
unsigned long currentMillis = 0;
// intervals for performing certain actions, used with timers
const unsigned long countPeriod = 10000;
const unsigned long blinkPeriod = 250;
const unsigned long debugPeriod = 1000;


// ************************************************************* 
// Timing function - sets alarm to ACTIVE state after timer elapses
void blinkTimer() {
    if (currentMillis - countStartMillis >= countPeriod) {
      alarmState = ALARM_ACTIVE;
      countStartMillis = currentMillis;
    }
}

// ************************************************************* 
// Timing function - used to print alarm state to console, for debugging 
void debug() {
  if (currentMillis - debugStartMillis >= debugPeriod) {
    Serial.println(alarmState);
    debugStartMillis = currentMillis;
  }
}

// ************************************************************* 
// Timing function - Turns LED on/off after timer elapses
void blinkTest() {
    if (alarmState == ALARM_COUNTDOWN && currentMillis - blinkStartMillis >= blinkPeriod) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      blinkStartMillis = currentMillis;
    }
}

// ************************************************************* 
// Button debounce logic

void checkButton() {
  int buttonRead = digitalRead(PIN_BUTTON);

  if (buttonRead != buttonSignal) {
    buttonSignal = buttonRead;
    if (buttonSignal == HIGH && alarmState != ALARM_ACTIVE) {
      //countStartMillis = currentMillis;
      alarmState = ALARM_ENABLED;
      
    }
  }
  delay(1);
}


// ************************************************************* 
// Checks for PIR input, sets alarm state if sensor is active
void checkInputs() {
  if (alarmState != ALARM_ACTIVE) {
    bPIR = digitalRead(PIN_PIR);
  }
  
  if (bPIR) {
      alarmState = ALARM_COUNTDOWN;
  }
  //checkButton();

}

// ************************************************************* 
// Checks current alarm state and reacts according to state
void checkAlarmState() {
  switch (alarmState) {
    case ALARM_DISABLED:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case ALARM_ENABLED:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case ALARM_COUNTDOWN:
      blinkTimer();
      blinkTest();
      break;
    case ALARM_ACTIVE:
      digitalWrite(LED_BUILTIN, LOW);
      break;

  }
}

// ************************************************************* 
void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  // Initial alarm state
  alarmState = ALARM_ENABLED;
  
} 
 
// ************************************************************* 
void loop() { 
  // Master timer
  currentMillis = millis();
  checkButton();
  checkInputs();
  checkAlarmState();
  debug();
}