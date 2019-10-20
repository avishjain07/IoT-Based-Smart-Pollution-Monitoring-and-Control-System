#include <hpma115S0.h>
#include <TinyGPS.h>

/**************************************************************
* Global variables for GPS Sensor                             *
***************************************************************/
long Lat,Long;                  // create variable for latitude and longitude object
float Lat1,Long1;
TinyGPS gps;                    // create gps object

/**************************************************************
* Global variables for PM Sensor                              *
***************************************************************/
HPMA115S0 hpma115S0(Serial3);
unsigned int PM25=0,PM10=0;       //variables to store pm data
float PM25_1,PM10_1;              //pm data to be sent to the server

void setup()
{
  Serial.begin(9600);     // Start serial communication
  Start_GSM();            // Start GSM Module
  Start_PM();             // Start PM  Sensor
  Serial2.begin(9600);  // Start GPS sensor
}

void loop()
{
  while(Serial2.available())
  {
    GPSRead();
  }
}

void toSerial(void)
{
  return;
} 

void GPSRead(void)
{
  if(gps.encode(Serial2.read())) // encode gps data
   { 
    gps.get_position(&Lat,&Long); // get latitude and longitude
    // display position
    Serial.print("Position: ");
    Serial.print("lat: ");Serial.print(Lat1);Serial.print(" ");// print latitude
    Serial.print("lon: ");Serial.println(Long1); // print longitude
    
    Long1=float(Long)/1000000;
    Lat1=float(Lat)/1000000;

    PM_Read();
    
    Send_data_to_server();
    delay(1000);
  }
}

void Start_GSM(void)
{
  //Setup Code (AT Commands) for GPRS

  Serial1.begin(9600);  //Start GSM/GPRS Module
  Serial.println("Config SIM900...");
  delay(100);
  Serial.println("Done!...");
  Serial1.flush();
  Serial.flush();

  // Attach or Detach from GPRS Service 
  Serial1.println("AT+CGATTb  ?");
  delay(100);
  toSerial();

  // Bearer settings
  Serial1.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(100);
  toSerial();

  // Bearer settings
  Serial1.println("AT+SAPBR=3,1,\"APN\",\"taif\"");
  delay(100);
  toSerial();

  // Bearer settings
  Serial1.println("AT+SAPBR=1,1");
  delay(100);
  toSerial();
}

void Send_data_to_server(void)
{
  // set HTTP Parameter Value
    Serial1.print("AT+HTTPPARA=\"URL\",\"www.indeedelectronics.com/write_data.php?data1="); 
    delay(50);
    Serial1.print(Lat1);  // Add Latitude to the URL
    delay(50);
    Serial1.print("&data2=");  
    delay(50);
    Serial1.print(Long1);  // Add Longitude to URL
    delay(50);
    Serial1.print("&data3=");  
    delay(50);
    Serial1.print(10);   // Add PM25 Value to URL
    delay(50);
    /*gprsSerial.print("&data4=");  
    delay(50);
    gprsSerial.print(PM10);  // Add PM10 Value to URL
    delay(50);*/
    Serial1.println("\"");   // Close URL
    delay(100);
    toSerial();

    // set http action type 0 = GET, 1 = POST, 2 = HEAD
    Serial1.println("AT+HTTPACTION=0");
    delay(100);
    toSerial();

    // read server response
    Serial1.println("AT+HTTPREAD"); 
    delay(100);
    toSerial();

    Serial1.println("");
    Serial1.println("AT+HTTPTERM");
    toSerial();
    delay(100);

    Serial1.println("");
    delay(100); //gsm code ends here
    Serial.println("data sent");
}

void Start_PM(void)
{
  Serial3.begin(9600);
  delay(5000);
  Serial.println("Starting...");
  hpma115S0.Init();
  hpma115S0.StartParticleMeasurement();
}
void PM_Read(void)
{
  if (hpma115S0.ReadParticleMeasurement(&PM25, &PM10))
  {
    Serial.println("PM 2.5: " + String(PM25) + " ug/m3" );
    Serial.println("PM 10 : " + String(PM10) + " ug/m3" );
  }
  PM25_1=PM25;
  PM10_1=PM10;
  delay(8000);
}
