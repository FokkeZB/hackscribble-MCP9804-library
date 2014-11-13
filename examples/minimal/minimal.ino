/*

	minimal
	=======
	
	Example sketch for Hackscribble_MCP9804 library.
	

	Created on 13 November 2014
	By Ray Benitez
	Last modified on ---
	By ---
	Change history in "README.md"
		
	This software is licensed by Ray Benitez under the MIT License.
	
	git@hackscribble.com | http://www.hackscribble.com | http://www.twitter.com/hackscribble

*/


//
// Set up sensor
//

#include <Wire.h>
#include <Hackscribble_MCP9804.h>

Hackscribble_MCP9804 sensor(0x1B);  // I2C address (7 bit).


//
// Test program parameters
//

#define TEST_RESOLUTION					R_DEGREES_0_1250

#define HEARTBEAT_DURATION_MS			1000

uint32_t lastHeartbeat;


void setup()
{

  Serial.begin(115200);
  Serial.println("Hackscribble_MCP9804: minimal.ino\r\n\n");

  sensor.begin();

  Serial.print("Resolution requested: ");
  Serial.println(TEST_RESOLUTION);
  sensor.setResolution(TEST_RESOLUTION);
  Serial.print("Resolution set to: ");
  Serial.println(sensor.getResolution());
  Serial.println();
  
  lastHeartbeat = millis();

}


void loop()
{

  if (millis() - lastHeartbeat > HEARTBEAT_DURATION_MS)
  {
    lastHeartbeat += HEARTBEAT_DURATION_MS;
    Serial.print("Ta = ");
    Serial.println(sensor.getTAFloat(), 2);
  }

}


