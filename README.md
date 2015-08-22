# BeerCounter
A beer cork counting system, built around an Arduino, with the love of beer.

## Contents
* Eagle schematic and PCB
* Arduino firmware

## Eagle

### Bill of materials
| Part      | Value         | Device          | Package         |
|-----------|---------------|-----------------|-----------------|
| C1        | 22pF          | C-EU025-024X044 | C025-024X044    |
| C2        | 22pF          | C-EU025-024X044 | C025-024X044    |
| C3        | 100nF         | C-EU025-024X044 | C025-024X044    |
| C4        |               | CPOL-EUTT2D5    | TT2D5           |
| C5        | 100nF         | C-EU025-024X044 | C025-024X044    |
| IC1       | TL072P        | TL072P          | DIL08           |
| IC2       | MEGA8-P       | MEGA8-P         | DIL28-3         |
| ISP       | AVR_SPI_PROG  | AVR_SPI_PROG    | AVR_ICSP        |
| J1        | POWER_JACKSLT | POWER_JACKSLT   | POWER_JACK_SLOT |
| LDR1      |               | M02LOCK         | 1X02_LOCK       |
| LDR2      |               | M02LOCK         | 1X02_LOCK       |
| Q1        | 16MHz         | XTAL/S          | QS              |
| R1        | 4k7           | R-EU_0204/7     | 0204/7          |
| R2        | 1M            | R-EU_0204/7     | 0204/7          |
| R3        | 1M            | R-EU_0204/7     | 0204/7          |
| R4        | 4k7           | TRIM_EU-LI10    | LI10            |
| R5        | 4k7           | TRIM_EU-LI10    | LI10            |
| R6        | 68k           | R-EU_0204/7     | 0204/7          |
| R7        | 68k           | R-EU_0204/7     | 0204/7          |
| R8        | 220           | R-EU_0204/7     | 0204/7          |
| R9        | 220           | R-EU_0204/7     | 0204/7          |
| R10       | 4k7           | R-EU_0204/7     | 0204/7          |
| S1        |               | TAC_SWITCHPTH   | TACTILE-PTH     |
| S2        |               | TAC_SWITCHPTH   | TACTILE-PTH     |

### Board render
Render of the controller board. It measures 2.05x2.30 inch

![board_controller](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/board_controller.png)

Render of the display board. It measures 3.70x1.60 inch.

![board_display](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/board_display.png)

## Arduino
The source can be found under `firmware`. It's build with Arduino 1.0.5, but newer versions should work too.

See the `firmware/UsedLibraries.md` for the libraries to install.

### Firmware
To be done.

### Hardware configuration
For best performance, the light gate should be fitted in a tube, where environment light has little effect.

To calibrate the device, check the [calibration](https://github.com/basilfx/Arduino-BeerCounter/blob/master/docs/Calibration.md) guide.

### Firmware configuration
Once you have the device ready, connect it to the computer via a serial adapter. Hold the setup button, and power cycle the device. The settings for terminal are 115200 baud, 8 bits, no parity and 1 start/stop bit. The terminal has the following commands:

* reset &mdash; Reset the device
* count <value> &mdash; Get/set the initial count
* zeroes <1|0> &mdash; Enable/disable leading zeroes
* restarts &mdash; Get the number of power cycles

## License
See the `LICENSE` file (MIT license).
