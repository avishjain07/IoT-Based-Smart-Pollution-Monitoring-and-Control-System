#include<SoftwareSerial.h>
#include "pm_library.h"


pm_sensor pm(10,11);
data* pm_data;
bool  pm_status;
void setup()
{

  Start_PM();
}

void loop()
{
  Read_PM();
}


/****************************************************************************************
* Start_PM() function is used to start the PM Sensor.                 *
* It also prints the status of PM Sensor to the Serial monitor:             *
*   If status is 1 => sensor can be used to read PM data.               *
* If status is 0 => sensor isn't ready yet.                     *
****************************************************************************************/
void Start_PM(void)
{
  Serial.begin(9600);
  while(!Serial);
  // Stop autosend
  pm_status = pm.stop_autosend(); 
  Serial.print("Stop autosend status is ");
  Serial.println(pm_status, BIN);
  Serial.println(" ");
  //delay(500);
  
  pm_status = pm.start_measurement(); 
  Serial.print("Start measurement status is ");
  Serial.println(pm_status, BIN);
  Serial.println(" ");
  //delay(5000);
}


/****************************************************************************************
* Read_PM() function is used to get pm10 and pm10 levels from the sensor.       *
* It uses read_measurement() function of the pm_sensor_library to get the data.     *
* ->read_measurement() function returns a pointer to an array of 3 integers which   *
*            contains status,pm2.5 data, & pm10 data.           *
* It also displays all 3 values on the serial monitor.                  *
****************************************************************************************/
void Read_PM(void)
{
  pm_data = pm.read_measurement();

  Serial.print("Read measurement status is ");
  Serial.println( (*pm_data).my_status);

  Serial.print("PM2.5 value is ");
  Serial.println( (*pm_data).pm25);
  
  Serial.print("PM10 value is ");
  Serial.println( (*pm_data).pm10);
  Serial.println(" ");
  
  //delay(8000);
}
