#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
// For stats that happen every 5 seconds
unsigned long last = 0UL;
int ledPin = 13;
void setup()
{
 Serial.begin(9600);
 ss.begin(GPSBaud);
pinMode(ledPin, OUTPUT);
pinMode(2, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
}
bool away = false;
bool transto = false;
bool leaving = false;
bool ihome = false;
double distanceToDest;
void setState()
{
  static double lastDist = 0;
  away = false;
  transto = false;
  leaving = false;
  ihome = false;
  if(distanceToDest > 0.2)
  away = true;
  else if(distanceToDest < 0.000000001)
  ihome = true;
  else if(lastDist <= 0.07 && distanceToDest > 0.07)
  leaving = true;
  else if(lastDist > 0.07 && distanceToDest <= 0.07)
  transto = true;
 
Serial.print(leaving);
Serial.print(transto);
Serial.print(away);
Serial.print(ihome);

lastDist = distanceToDest;
  
}
void loop()
{
 // Dispatch incoming characters
 while (ss.available() > 0)
   gps.encode(ss.read());
 if (gps.location.isUpdated())
 {
   Serial.print(F("  Lat="));
   Serial.print(gps.location.lat(), 6);
   Serial.print(F(" Long="));
   Serial.println(gps.location.lng(), 6);
 }
 else if (millis() - last > 5000)
 {
   Serial.println();
   if (gps.location.isValid())
   {
     // replace 'Dest_LAT' and 'Dest_LON' values basedon your location  
     // you can find Latitude and Longitude from Read_Lat_Lng.ino  
     static const double Dest_LAT = 33.048071, Dest_LON = -117.203849;
     distanceToDest =
       TinyGPSPlus::distanceBetween(
         gps.location.lat(),
         gps.location.lng(),
         Dest_LAT, 
         Dest_LON)/100;
         setState ();
     Serial.print(F("Distance to Destination ="));
     Serial.print(distanceToDest, 6);       // *Prints distance to destination 
     if(ihome || away)   //Here when distanceToDest/1000 is less than  0.050000  LED turns on . So change *distance to destination as per your requirement. 
     {
      digitalWrite(ledPin, LOW);
      }
     else if(transto)
     {
       digitalWrite(2, LOW);
       delay(1000);
       digitalWrite(2, HIGH);
       delay(250);
       digitalWrite(7, LOW);
       delay(1000);
       digitalWrite(7, HIGH);
       delay(250);
       digitalWrite(8, LOW);
       delay(1000);
       digitalWrite(8, HIGH);
       delay(250);
     }
     else if(leaving)
     {
       digitalWrite(2, LOW);
       delay(1000);
       digitalWrite(2, HIGH);
       delay(250);
       digitalWrite(7, LOW);
       delay(1000);
       digitalWrite(7, HIGH);
       delay(250);
       digitalWrite(8, LOW);
       delay(1000);
       digitalWrite(8, HIGH);
       delay(250);
     }
   } 
   if (gps.charsProcessed() < 10) 
     Serial.println(F("WARNING: No GPS data.  Check wiring."));
   last = millis();
   Serial.println();
 }
}
