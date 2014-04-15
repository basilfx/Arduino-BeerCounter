# BeerCounter
A beer cork counting system, built around an Arduino, with the love of beer.

## Contents
* Eagle schematic and PCB
* Arduino firmware

## Eagle

### Bill of materials
To be done.

### Board render
Display board:

![alt tag](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/display.png)

Schema board:

![alt tag](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/schema.png)

## Arduino
The source can be found under `firmware`. It's build with Arduino 1.0.5, but newer versions should work too.

See the `firmware/UsedLibraries.md` for the libraries to install.

### Firmware
To be done.

### Hardware configuration
For best performance, the light gate should be fitted in a tube, where environment light has little effect. 

Configuring the hardware is something that has to be done with extra care. Since the detection is built around a light gate and a comparator, the potentiometers have to be tuned to the right sensitivity. The right sensitivity can be estimated by measuring the voltage across the LDR, and then tune the wiper position so the voltage across the potentiometer is a bit higher. 

If you have a osciloscope, you can hook it up to the comparator output, and measure the time of flight if you throw one cork through the sensor tube. If the time of flight is too short, or not detected, try to decrease sensitivity. Else, increase it.

### Firmware configuration
Once you have the device ready, connect it to the computer via a serial adapter. Hold the setup button, and power cycle the device. The settings for terminal are 115200 baud, 8 bits, no parity and 1 start/stop bit. The terminal has the following commands:

* reset &mdash; Reset the device
* count <value> &mdash; Get/set the initial count
* zeroes <1|0> &mdash; Enable/disable leading zeroes
* restarts &mdash; Get the number of power cycles

## License
See the `LICENSE` file (MIT license).