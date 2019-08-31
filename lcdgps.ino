#include <SoftwareSerial.h> 

#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

TinyGPS gps;
SoftwareSerial ss(5,4);   //Rx,Tx;  Rx(5)-> Tx(GPS), Tx(4)->Rx(GPS) 


const int rs = 12, en = 11, d4 =10 , d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int i;






float lat, lng;
static char lat1[8];
static char lng1[8];




void setup()
{

  Serial.begin(9600);
  ss.begin(9600);
    lcd.begin(16,2);
  lcd.setCursor(0,0);
   lcd.print("Boat No - 1127");
  lcd.setCursor(0,1);
  lcd.print(" GPS Location");
   delay(5000); 
  lcd.noDisplay();
  
 }



void loop()
{
  B: 
   lcd.display();
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
   
    lcd.setCursor (0,0);
    lcd.print("LAT = ");
    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    lcd.print(" ");

    lcd.setCursor(0, 1);
    lcd.print("LON = ");
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    lcd.print(" ");
    
    lat = flat;
    lng = flon;
  }

  dtostrf(lat,7, 6, lat1);
  dtostrf(lng,7, 6, lng1);
  
  
 

}



