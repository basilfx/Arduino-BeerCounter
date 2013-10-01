#include <avr/eeprom.h>
#include <TimerOne.h>
#include <CommandLine.h>
#include "Config.h"

//#define DEBUG

/**
 * CommandLine
 */
CommandLine commandLine;
configuration_t configuration;

timer_t timerSave;
timer_t timerDisplay;
timer_t timerMinute;
timer_t timerBlink;
timer_t timerValidate;
timer_t timerBackoff;
timer_t* timers[] = { &timerSave, &timerDisplay, &timerMinute, &timerBlink, &timerValidate, &timerBackoff};

/**
 * Mask for the seven segment displays
 */
const byte numbers[17] = {
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11100110,
  0b11101110,
  0b00111110,
  0b10011100,
  0b01111010,
  0b10011110,
  0b10001110,
  0b00000000
};

uint16_t perMinute[60];
uint8_t currentMinute;
uint16_t totalPerHour;

int missedInterrupts;
byte error;
byte displays;
byte currentDisplay;

void setup() {
  // Setup pins
  setupPins();

  // Load settings
  loadSettings();
  
  // Determine program mode
  if (digitalRead(PIN_MODE) == LOW) {
    configMode(); 
  } else {
    #ifdef DEBUG
    Serial.begin(38400);
    Serial.println("Run mode");
    #endif
    
    // Increase restart count
    configuration.restarts++;
    saveSettings();
    
    // Setup normal program
    setupNormalMode();
  }
}

/**
 * Setup commandline
 */
void configMode() {
  // Setup serial
  Serial.begin(38400);
  Serial.println("Config mode");

  // Add commands
  commandLine.add((char*) "help", commandHelp);
  commandLine.add((char*) "count", commandCount); 
  commandLine.add((char*) "zeroes", commandZeroes); 
  commandLine.add((char*) "restarts", commandRestarts);
  commandLine.add((char*) "ram", commandRam);   
  commandLine.add((char*) "reset", commandReset);

  // Start command line
  commandLine.begin((char *) "> ");

  // Loop config mode until reset.
  for (;;) commandLine.loop();
}

void configTimers() {
  timerSave.enabled = false;
  timerSave.count = 0;
  timerSave.value = 60000;
  timerSave.callback = &timerSaveCallback;
  
  timerDisplay.enabled = true;
  timerDisplay.count = 0;
  timerDisplay.value = 2000;
  timerDisplay.callback = &timerDisplayCallback;
  
  timerMinute.enabled = true;
  timerMinute.count = 0;
  timerMinute.value = 60000;
  timerMinute.callback = &timerMinuteCallback;

  timerBlink.enabled = true;
  timerBlink.count = 0;
  timerBlink.data = 0;
  timerBlink.value = 500;
  timerBlink.callback = &timerBlinkCallback;
  
  timerValidate.enabled = false;
  timerBlink.data = 0;
  timerValidate.count = 0;
  timerValidate.value = 5;
  timerValidate.callback = &timerValidateCallback;
  
  timerBackoff.enabled = false;
  timerBackoff.count = 0;
  timerBackoff.value = 35;
  timerBackoff.callback = &timerBackoffCallback;
}

/**
 * Setup the pins
 */
void setupPins() {
  pinMode(PIN_MODE, INPUT);
 
  pinMode(PIN_LDR1, INPUT);
  pinMode(PIN_LDR2, INPUT);

  pinMode(PIN_SEG1, OUTPUT);     
  pinMode(PIN_SEG2, OUTPUT);     
  pinMode(PIN_SEG3, OUTPUT);     
  pinMode(PIN_SEG4, OUTPUT);       

  pinMode(PIN_SEGA, OUTPUT);     
  pinMode(PIN_SEGB, OUTPUT);     
  pinMode(PIN_SEGC, OUTPUT);     
  pinMode(PIN_SEGD, OUTPUT); 
  pinMode(PIN_SEGE, OUTPUT);     
  pinMode(PIN_SEGF, OUTPUT);     
  pinMode(PIN_SEGG, OUTPUT);     
  pinMode(PIN_SEGDOT, OUTPUT);
}

void setupNormalMode() {
  // Stabilize
  delay(1000);
  
  // Attach interrupts
  attachInterrupt(INTERRUPT1, interruptDetect, RISING);
  attachInterrupt(INTERRUPT2, interruptDetect, RISING);
  
  // Set default state
  displays = DISPLAY_NORMAL;
  currentDisplay = DISPLAY_NORMAL;
  error = ERROR_NONE;
  missedInterrupts = 0;
  
  // Setup program timers
  configTimers();
  
  // Setup timer interrupt
  Timer1.initialize(1000);
  Timer1.attachInterrupt(interruptTimer);
}

/**
 * Main-loop van het programma. Handel alle tijdsintensiefe taken
 * af, afhankelijk van de variabelen
 */
void loop() {
  boolean skipDigit = true;
  boolean raw = false;
  boolean dot[4] = {false, false, false, false};
  byte digit[4];
  long power = 1000;
  long value = 0;
  
  // Determine what to display
  switch (currentDisplay) {
    case DISPLAY_NORMAL:
      value = configuration.count;
      
      dot[0] = configuration.count >= 40000;
      dot[1] = configuration.count >= 30000;
      dot[2] = configuration.count >= 20000;
      dot[3] = configuration.count >= 10000;
      
      break;
    case DISPLAY_PER_HOUR:
      value = totalPerHour;
  
      dot[0] = false;
      dot[1] = false;
      dot[2] = false;
      dot[3] = timerBlink.data;
      
      break;
    case DISPLAY_ERROR:
      raw = true;

      dot[0] = false;
      dot[1] = false;
      dot[2] = false;
      dot[3] = false;
      
      digit[3] = error;
      digit[2] = 0;
      digit[1] = 0;
      digit[0] = 0x0E;
      break;
  }
    
  // Convert value into value per digit if not raw
  if (!raw) {
    for (int i = 0; i < 4; i++) {
       digit[i] = (value / power) % 10;
       power = power / 10;
       
       if (!configuration.zeroes && digit[i] == 0 && skipDigit && i != 3) {
         digit[i] = 16; // numbers[16] = 0b00000000
       } else {
         skipDigit = false;
       }
    }
  }

  // Display the digits
  updateSegment(4, digit[0], dot[0]);
  updateSegment(3, digit[1], dot[1]);
  updateSegment(2, digit[2], dot[2]);
  updateSegment(1, digit[3], dot[3]);
}

void updateSegment(byte segment, byte number, boolean dot) {
    digitalWrite(PIN_SEG1, LOW);
    digitalWrite(PIN_SEG2, LOW);
    digitalWrite(PIN_SEG3, LOW);
    digitalWrite(PIN_SEG4, LOW);      
  
    digitalWrite(PIN_SEGA, (numbers[number] & 0b10000000));  
    digitalWrite(PIN_SEGB, (numbers[number] & 0b01000000)); 
    digitalWrite(PIN_SEGC, (numbers[number] & 0b00100000)); 
    digitalWrite(PIN_SEGD, (numbers[number] & 0b00010000)); 
    digitalWrite(PIN_SEGE, (numbers[number] & 0b00001000)); 
    digitalWrite(PIN_SEGF, (numbers[number] & 0b00000100)); 
    digitalWrite(PIN_SEGG, (numbers[number] & 0b00000010)); 
    digitalWrite(PIN_SEGDOT, dot); 
      
    if (segment == 1)
      digitalWrite(PIN_SEG1, HIGH);
    if (segment == 2)
      digitalWrite(PIN_SEG2, HIGH);
    if (segment == 3)
      digitalWrite(PIN_SEG3, HIGH);
    if (segment == 4)
      digitalWrite(PIN_SEG4, HIGH); 
     
    delay(5);
}

/**
 * Wordt aangeroepen zodra een dopje detected wordt
 *
 * Omdat het kan zijn dat een dopje meerdere signalen afgeeft, kan er 
 * slechts een volgende interrupt plaatsvinden zodra de eerste is afgehandeld
 * door de timer (na 200-300ms)
 */
void interruptDetect() {  
  // Disable interrupts
  noInterrupts();
  
  // Enable validation timer
  if (timerValidate.data == 0) {
    timerValidate.count = 0;
    timerValidate.data = 1;
    timerValidate.enabled = true;
  }
  
  // Re-enable interrupts
  interrupts();
}

void interruptTimer() {
  for (int i = 0; i < 6; i++) {
    if (timers[i]->enabled == true) {
      timers[i]->count++;
     
      if (timers[i]->count > timers[i]->value) {
        noInterrupts();
        timers[i]->count = 0;
        timers[i]->callback();
        interrupts();
      }
    }
  }
}

void timerValidateCallback() {
  // Disable this timer
  timerValidate.enabled = false;
  
  // Increment count IF one of the pins is still high
  if (digitalRead(PIN_LDR1) == HIGH || digitalRead(PIN_LDR2) == HIGH) {
    // Increment total count
    configuration.count++;
    
    // Update per hour stats (including current)
    displays |= DISPLAY_PER_HOUR;
    perMinute[currentMinute]++;
    totalPerHour++;

    // Check for errors
    if (totalPerHour > 15) {
      error |= ERROR_TOO_MANY;
      displays |= DISPLAY_ERROR;
      
      #ifdef DEBUG
      Serial.println("Too many");
      #endif
    }
    
    // Schedule a save event
    timerSave.count = 0;
    timerSave.enabled = true;
    
    // Ensure current count is displayed
    timerDisplay.count = 0;
    currentDisplay = DISPLAY_NORMAL;
    
    #ifdef DEBUG
    Serial.println("Int ACK");
    #endif
    
    // Enable timer for backoff period
    timerBackoff.count = 0;
    timerBackoff.enabled = true;
  } else {
    missedInterrupts++;
    
    if (missedInterrupts > 5) {
      error |= ERROR_MISSED_INTERRUPTS;
      displays |= DISPLAY_ERROR;
    }
    
    // Reset validation timer
    timerValidate.data = 0;
    
    #ifdef DEBUG
    Serial.println("Int NAK");
    #endif
  }
}

void timerBackoffCallback() {
  #ifdef DEBUG
  Serial.println("Backoff");
  #endif
    
  // Disable this timer
  timerBackoff.enabled = false;
  
  // Reset validation timer
  timerValidate.data = 0;
}

void timerSaveCallback() {
  #ifdef DEBUG
  Serial.println("Save");
  #endif
  
  // Save to EEPROM
  saveSettings();
   
  // Clear timer
  timerSave.enabled = false;
}

void timerDisplayCallback() {
  static byte index = 0;
  
  // Handle current display
  switch (currentDisplay) {
    case DISPLAY_PER_HOUR:
      displays &= ~DISPLAY_PER_HOUR;
      break;
  }
  
  // Determine next display
  if (index == 0) {
    index = 8;
  }
  
  while (index--) {
    if (displays & (1 << index)) {
      break;
    }
  }
  
  // Set display
  currentDisplay = (1 << index);
}

void timerMinuteCallback() {
  #ifdef DEBUG
  Serial.println("Minute");
  #endif

  // Increment current minute
  currentMinute = (currentMinute + 1) % 60;
  perMinute[currentMinute] = 0;

  // Update the total per hour
  totalPerHour = 0;
  
  for (int i = 0; i < 60; i++) {
     totalPerHour += perMinute[i];
  }
}

void timerBlinkCallback() {
  timerBlink.data = !timerBlink.data;
}

void saveSettings() {
  // Insert validation token
  configuration.magic = 0xAA;
  
  eeprom_write_block((const void *) &configuration, (void *) 0, sizeof(configuration)); 
}

void loadSettings() {
  eeprom_read_block((void *) &configuration, (void *) 0, sizeof(configuration));
  
  // Load default values if magic token is wrong
  if (configuration.magic != 0xAA) {
    configuration.count = 0;
    configuration.zeroes = false;
    configuration.restarts = 0;
  }
}
