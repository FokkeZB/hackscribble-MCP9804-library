/*

	test
	====
	
	Example sketch for Hackscribble_MCP9804 library.
	

	Created on 13 November 2014
	By Ray Benitez
	Last modified on 17 October 2016
	By Ray Benitez
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

#define ALERT_PIN	A0


//
// Test program parameters
//

#define TEST_THRESHOLD_CRITICAL_DEGREES 50
#define TEST_THRESHOLD_UPPER_DEGREES    28
#define TEST_THRESHOLD_LOWER_DEGREES     -2

#define TEST_RESOLUTION					R_DEGREES_0_1250
#define TEST_HYSTERESIS					H_DEGREES_3_0

#define HEARTBEAT_DURATION_MS			1000

uint32_t lastHeartbeat;


void setup()
{

  Serial.begin(115200);
  Serial.println("Hackscribble_MCP9804: test.ino\r\n\n");

  pinMode(ALERT_PIN, INPUT);

  sensor.begin();

  Serial.println("Manufacturer ID, device ID, device revision:");
  Serial.print("0x");
  Serial.print(sensor.getManufacturerID(), HEX);
  Serial.print("   0x");
  Serial.print(sensor.getDeviceID(), HEX);
  Serial.print("   0x");
  Serial.println(sensor.getDeviceRevision(), HEX);

  Serial.print("Resolution requested: ");
  Serial.println(TEST_RESOLUTION);
  sensor.setResolution(TEST_RESOLUTION);
  Serial.print("Resolution set to: ");
  Serial.println(sensor.getResolution());
  Serial.println();

  Serial.print("Hysteresis requested: ");
  Serial.println(TEST_HYSTERESIS);
  sensor.setHysteresis(TEST_HYSTERESIS);
  Serial.print("Hysteresis set to: ");
  Serial.println(sensor.getHysteresis());
  Serial.println();

  Serial.print("Tlower requested: ");
  Serial.println(TEST_THRESHOLD_LOWER_DEGREES);
  sensor.setTLOWER(TEST_THRESHOLD_LOWER_DEGREES);
  Serial.print("Tlower set to: ");
  Serial.println(sensor.getTLOWER());
  Serial.println();

  Serial.print("Tupper requested: ");
  Serial.println(TEST_THRESHOLD_UPPER_DEGREES);
  sensor.setTUPPER(TEST_THRESHOLD_UPPER_DEGREES);
  Serial.print("Tupper set to: ");
  Serial.println(sensor.getTUPPER());
  Serial.println();

  Serial.print("Tcrit requested: ");
  Serial.println(TEST_THRESHOLD_CRITICAL_DEGREES);
  sensor.setTCRIT(TEST_THRESHOLD_CRITICAL_DEGREES);
  Serial.print("Tcrit set to: ");
  Serial.println(sensor.getTCRIT());
  Serial.println();

  sensor.configureAlert(ENABLE);

  // At startup, alert pin is not asserted if Ta is already out of range, so handle this special case
  if ((sensor.getTAInteger() < sensor.getTLOWER()) || (sensor.getTAInteger() > sensor.getTUPPER()))
  {
    handleAlert();
  }  
  
  lastHeartbeat = millis();

}


void loop()
{

  if (millis() - lastHeartbeat > HEARTBEAT_DURATION_MS)
  {
    lastHeartbeat += HEARTBEAT_DURATION_MS;
    Serial.print("Ta = ");
    Serial.println(sensor.getTAFloat(), 2);
	Serial.println("CUL threshold alert flags");
    Serial.print(sensor.alertTCRIT());
    Serial.print(sensor.alertTUPPER());
    Serial.println(sensor.alertTLOWER());
  }

  if (digitalRead(ALERT_PIN) == LOW) // MCP9804 alert output is active low.
  {
    handleAlert();
  }

}


void handleAlert()
{
	float time = millis() / 1000.0;
	
	boolean atc = sensor.alertTCRIT();
	boolean atu = sensor.alertTUPPER();
	boolean atl = sensor.alertTLOWER();
	float ta = sensor.getTAFloat();
	
	sensor.clearAlert();

	Serial.print("\r\nALERT AT ");
	Serial.print(time, 3);
	Serial.println(" seconds");
	Serial.println("CUL threshold alert flags");
	Serial.print(atc);
	Serial.print(atu);
	Serial.println(atl);
	Serial.println("Ta");
	Serial.println(ta, 2);
	Serial.println();
}
