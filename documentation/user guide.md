Hackscribble_MCP9804 Library User Guide
=======================================

#### *Comprehensive Arduino library for Microchip Technology's MCP9804 I2C digital temperature sensor.*

<br>

Created on 13 November 2014 by Ray Benitez  
Last modified on --- by ---  
Change history in **README.md**  

This software is licensed by Ray Benitez under the MIT License.  
  
git@hackscribble.com | http://www.hackscribble.com | http://www.twitter.com/hackscribble

<br>


1. About the MCP9804
--------------------

The MCP9804 from Microchip Technology is a compact, I2C-connected digital temperature sensor. It converts temperatures between -40°C and +125°C to a digital word with better than ±1°C accuracy. The MCP9804 allows user-selectable settings such as Shutdown or low-power modes and the specification of upper, lower and critical temperature thresholds. When the temperature changes beyond the specified threshold, the MCP9804 outputs an Event signal.

You can find more information about the MCP9804 here: http://www.microchip.com/wwwproducts/Devices.aspx?product=MCP9804.


2. What the Hackscribble_MCP9804 library does
---------------------------------------------

The Hackscribble_MCP9804 library makes it simple to use the MCP9804 in your Arduino sketch, handling all communication and value conversion between your sketch and the sensor.

The library provides functions to:

- read the ambient temperature
- set the precision with which the MCP9804 measures temperatures
- set upper, lower and critical temperature thresholds for the MCP9804 to monitor
- check the alert status being indicated by the MCP9804
- set the hysteresis which the MCP9804 uses to decide if a temperature threshold has been crossed


3. Getting started
------------------

1.  Download and install the Hackscribble_MCP9804 library.

2.  Connect an MCP9804 to your Arduino.  

3.  Try out the example sketches.

4.  Read the **Reference manual** section later in this guide to understand how to use the Hackscribble_MCP9804 library in your sketches.


4. How to install Hackscribble_MCP9804
--------------------------------------

Follow the standard Arduino instructions at http://arduino.cc/en/Guide/Libraries.


5. Connecting the MCP9804 to your Arduino
--------------------------------------

The MCP9804 uses the I2C bus to communicate with the Arduino.  Connect MCP9804 SCL and SDA pins to the equivalent Arduino pins (for a Uno, these are A5 and A4 respectively).  Connect pullup resistors from SDA and SCL to +5V (typically 4.7k).

If you want to use the MCP9804 Alert output, connect it to an unused Arduino input pin. For convenience, you can connect it to an unused analog input pin close the I2C inputs, but any pin capable of digital input can be used. The Alert output is active low and needs a pullup resistor to +5V.  Either use an external resistor or configure the internal Arduino pullup.  For example:
```
pinMode(A0, INPUT_PULLUP);
```

Connect MCP9804 GND to Arduino GND.  You can power the MCP9804 from Arduino +5V if required.


6. Example sketches
-------------------

The Hackscribble_MCP9804 library comes with two example sketches:

1. **minimal** sets the measurement resolution of the MCP9804 and then repeatedly reads and prints the ambient temperature.

2. **test** sets the measurement resolution and threshold resolution, and upper and lower threshold temperatures. It then repeatedly reads and prints the ambient temperature and the status of the MCP9804 threshold alert flags.  When either threshold temperature is passed, it prints details of the event.

The sketches include comments and debug `Serial.println()` statements to explain what they are doing.


7. Reference manual
-------------------

#### 7.1 Prerequisites

Include the following in your sketch to load the Wire and Hackscribble_MCP9804 libraries:

```
#include <Wire.h>
#include <Hackscribble_MCP9804.h>
```


#### 7.2 Hackscribble_MCP9804

##### Creating an instance of Hackscribble_MCP9804

```
Hackscribble_MCP9804(uint8_t address);
```

The constructor takes one argument, the 7-bit I2C address of the MCP9804.  For example:

```
Hackscribble_MCP9804 mySensor(0x18);
```

You can have more than one MCP9804 connected to your Arduino at the same time.  You should create each one with a different I2C address.  For example ...

```
Hackscribble_MCP9804 firstSensor(0x18);
Hackscribble_MCP9804 secondSensor(0x19);
```

After creating each instance, you call its `begin()` method. This initialises the I2C bus, so you don't have to in your sketch. For example ...

```
firstSensor.begin();
secondSensor.begin();
```

##### Reading the ambient temperature

```
int8_t getTAInteger();
float getTAFloat();
```
You can read the ambient temperature (Ta) from the MCP9804 as either an integer or a float.  The value returned by `getTAInteger()` will be rounded to the nearest integer; it is not a truncated version of the float value.

For example ...

```
// Assume actual Ta is 20.75 degrees
Serial.println(firstSensor.getTAFloat());   // Prints 20.75
Serial.println(firstSensor.getTAInteger()); // Prints 21
```

For many applications, these will be the only functions you need to use, since the default value for resolution (0.0625 degrees) will be satisfactory.

##### Setting measurement resolution

The MCP9804 can be set to measure Ta with different resolutions: half, quarter, eighth and sixteenth of a degree.  The trade off is that the greater the resolution, the longer each sample takes to read, as described in section 5.1.6 of the MCP9804 datasheet.

The library provides functions to set and read back the resolution, using this `enum` type and methods below:

```
enum MCP9804_Resolution
{
	R_DEGREES_0_5000 = 0x00,	// 0.5 degrees
	R_DEGREES_0_2500 = 0x01,	// 0.25 degrees
	R_DEGREES_0_1250 = 0x02,	// 0.125 degrees
	R_DEGREES_0_0625 = 0x03,	// 0.0625 degrees
	numberOfResolutionOptions
};
```
```
MCP9804_Resolution getResolution();
void setResolution(MCP9804_Resolution res);
```

For example ...

```
// Assume actual Ta is 20.92 degrees
firstSensor.setResolution(R_DEGREES_0_5000);  // 0.5 degree resolution
Serial.println(firstSensor.getTAFloat(), 4);  // Prints 21.0000
firstSensor.setResolution(R_DEGREES_0_0625);  // 0.0625 degree resolution  
Serial.println(firstSensor.getTAFloat(), 4);  // Prints 20.9375
```

##### Threshold alerts

The MCP9804 can be configured to set a status flag and generate an Alert output when Ta exceeds any one of three thresholds: Upper, Lower and Critical.  Their operation is described in section 5.1.2 of the MCP9804 datasheet.

The library provides functions to set and read back the value of each temperature threshold in degrees.  They support integer values only. 

```
void setTUPPER(int8_t val);
void setTLOWER(int8_t val);
void setTCRIT(int8_t val);
```
```
int8_t getTUPPER();
int8_t getTLOWER();
int8_t getTCRIT();
```

The MCP9804 can be set to apply different amounts of hysteresis when deciding if a threshold has been passed (either by a rising or falling Ta): 0, 1.5, 3 and 6 degrees.  Section 5.1.1 of the MCP9804 datasheet describes how hysteresis works.

The library provides functions to set and read back the hysteresis, using this `enum` type and methods below:

```
enum MCP9804_Hysteresis
{
	H_DEGREES_0_0 = 0x00,		// 0.0 degrees
	H_DEGREES_1_5 = 0x01,		// 1.5 degrees
	H_DEGREES_3_0 = 0x02,		// 3.0 degrees
	H_DEGREES_6_0 = 0x03,		// 6.0 degrees
	numberOfHysteresisOptions
};
```
```
MCP9804_Hysteresis getHysteresis();
void setHysteresis(MCP9804_Hysteresis val);
```

For example ...

```
firstSensor.setHysteresis(H_DEGREE_1_5);
```

When the MCP9804 detects that Ta has risen or fallen through a defined threshold (allowing for hysteresis), it sets or clears an alert flag for the threshold.

The library provides three functions to check the status of each threshold.  A value of `true` means that the threshold has been exceeded.
```
boolean alertTCRIT();
boolean alertTUPPER();
boolean alertTLOWER();
```

You can also set the MCP9804 to activate its Alert output when any of the thresholds is exceeded. If you want to use this feature, call this method at the start of your sketch:

```
void configureAlert();
```

After your sketch detects the Alert output and reads the status flags and temperature, it must clear the flag so that further alerts can be detected by calling this method:
 
```
void clearAlert();

```

From testing, it appears that if Ta is already out of range of the defined thresholds at startup, the Alert output is not activated.  In our example sketch, we use the following code to handle this special case:

```
if ((sensor.getTAInteger() < sensor.getTLOWER()) || (sensor.getTAInteger() > sensor.getTUPPER()))
{
  handleAlert();  // Call user function that is called when Alert is detected
}
```

##### Other functions

The library provides three functions to read the device details as described in sections 5.1.4 and 5.1.5 of the MCP9804 datasheet.
 
```
uint16_t getManufacturerID();
uint8_t getDeviceID();
uint8_t getDeviceRevision();
```
