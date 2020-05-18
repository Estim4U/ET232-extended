# ET232-extended

    IMPORTANT: THIS PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTY OF ANY KIND.
    USE IT AT YOUR OWN RISKS.

![](/src/resources/logo.png)

The ET232 power unit is a nice and powerful box with a dozen of very well crafted routines.
But this is just the tip of the iceberg; you can get much more from your ET232.
The output signal of each channel depends on 19 parameters.
But for each routine the Multi Adjust (MA) knob let you control only one, two or three parameters.

This program give you access to all adjustable parameters
and visualize the variations in real time.

![](/doc/screen01.png)

![](/doc/screen02.png)

# Requirements

* ET232 Power unit.
* USB/Serial adapter.
* Qt 5.12 LTS release.
* Your ...

# Read the `doc` first

The `doc` directory contains flat Open Office documents (.fodt) explaining how to connect
the ET232 to your computer, how the output signal is controled, the serial protocol etc...

![](/src/resources/signal.png)

When the program starts read **carefully** the information displayed about how to
synchronize with ET232.

*Note: Depending on the buffer status this can fail, retry.*

# Qt 5

This C++ code requires Qt 5.12 LTS release.
It hasn't been tested with Qt 5.13, 5.14.
The serial communication doesn't work with Qt 5.15.0 (beta).

# Recommendation

Start with the *Intense* routine. The number of parameters you can
control is limited and easy to understand.
