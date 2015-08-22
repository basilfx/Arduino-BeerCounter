# Calibration

## Introduction
Configuring the hardware is something that has to be done with extra care. Since the detection is built around a light gate and a comparator, the potentiometers have to be tuned to the right sensitivity. The right sensitivity can be estimated by measuring the voltage across the LDR, and then tune the wiper position so the voltage across the potentiometer is a bit higher.

If you have a oscilloscope, you can hook it up to the comparator output, and measure the time of flight if you throw one cork through the sensor tube. If the time of flight is too short, or not detected, try to decrease sensitivity. Otherwise, increase it. The output of a channel should be in the range of 15-30 ms when a cork is registered. The examples below show a calibrated output.

## Examples

### One channel
This is an example for just one channel, when a cork is registered.

![scope_channel](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/scope_channel.png)

* Yellow: LDR output
* Blue: voltage reference (output of trimpot)
* Purple: channel output (feeds into Arduino)

### Both channels
This is an example reading of both channels, when a cork is registered.

![scope_both](https://raw.github.com/basilfx/Arduino-BeerCounter/master/docs/scope_both.png)

* Yellow: channel one (feeds into Arduino)
* Blue: channel two (feeds into Arduino)
