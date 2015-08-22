
void commandHelp(char * arguments) {
  Serial.println("Available commands: count [#], help, reset, zeroes (0|1)");
}

void commandCount(char * arguments) {
  char * value = strtok(NULL, " ");

  if (value == NULL) {
    Serial.print("Count: ");
    Serial.print(configuration.count);
    Serial.println(""); 
  } 
  else {
    long count = (long) atol(value);

    if (count < 0 ) {
      Serial.println("Count must be positive"); 
      return;
    }

    configuration.count = count;
    saveSettings();
  }
}

void commandZeroes(char * arguments) {
  char * value = strtok(NULL, " ");

  if (value == NULL) {
    Serial.print("Zeroes: ");
    Serial.print(configuration.zeroes);
    Serial.println(""); 
    return;
  } 

  byte state = atoi(value);

  if (state < 0 || state > 1) {
    Serial.println("Parameter must be 1 or 0");
    return; 
  }

  configuration.zeroes = state == 1;
  saveSettings();
}

void commandRestarts(char * arguments) {
    Serial.print("Restarts: ");
    Serial.print(configuration.restarts);
    Serial.println("");
}

void commandReset(char * arguments) {
  // Warning: soft reset only!
  asm volatile ("jmp 0");
}

void commandRam(char * arguments) {
  extern int __heap_start, *__brkval; 
  int v;
  int free = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  
  Serial.print("Memory: ");
  Serial.print(free);
  Serial.println("");
}

