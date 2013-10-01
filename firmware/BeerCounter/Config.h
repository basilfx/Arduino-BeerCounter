#ifndef CONFIG_H
#define CONFIG_H

// Pin settings
#define PIN_MODE 4
#define PIN_LDR1 2
#define PIN_LDR2 3

#define PIN_SEG1 10
#define PIN_SEG2 7
#define PIN_SEG3 8
#define PIN_SEG4 9

#define PIN_SEGDOT A0
#define PIN_SEGG A1
#define PIN_SEGF A2
#define PIN_SEGE A3
#define PIN_SEGD A4
#define PIN_SEGC A5
#define PIN_SEGB 5
#define PIN_SEGA 6 

// Interrupts
#define INTERRUPT1 0
#define INTERRUPT2 1

// Display states
#define DISPLAY_NORMAL 0x01
#define DISPLAY_PER_HOUR 0x02
#define DISPLAY_ERROR 0x04

#define ERROR_NONE 0x00
#define ERROR_TOO_MANY 0x01
#define ERROR_MISSED_INTERRUPTS 0x02

// Definitions
struct configuration_t {
  unsigned int magic;
  unsigned long count;
  unsigned long restarts;
  bool zeroes;
};

struct timer_t {
  bool enabled;
  unsigned long count;
  unsigned long value;
  unsigned long data;
  void (*callback)();
};

#endif
