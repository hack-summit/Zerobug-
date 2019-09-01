#include <SoftwareSerial.h> 

#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
const int buzzer = 6;  
TinyGPS gps;
SoftwareSerial ss(5,4);  

#define enB 3     
#define in3 2
#define in4 13

long int  caution_lng, caution_lat;

long int lngmult;
long int latmult;


long int   borderlat = 12824424;
long int   borderlng = 80046791;

const int rs = 12, en = 11, d4 =10 , d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int i;



static char message[100];


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
  
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set initial rotation direction      // LOW, HIGH  - Forward,    HIGH,LOW   - Reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
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
      
lngmult = lng*1000000;       
latmult = lat*1000000;        


caution_lat = abs(borderlat-latmult);
caution_lng = abs(borderlng-lngmult);


if ((caution_lng< 200 )  )    
{
  analogWrite(enB, 0);                          // motor off
 // Serial.print("motor off ");

  tone(buzzer, 500);                           // High Sound - Border Reached
  delay(100);        
  
}

else if ((caution_lng >= 200 ))                  
{
  analogWrite(enB, 200);                       // motor medium speed
  // Serial.print("motor medium speed");
  tone(buzzer, 500);                           // medium speed - adjacent to border
  delay(500);        
  noTone(buzzer);     
  delay(500); 
    
  //SendMessage();
}

/*else if ((caution_lat > 800 ))     
{
  analogWrite(enB, 255);                      // motor high speed 
  // Serial.print("motor High speed");
  SendMessage();

tone(buzzer, 500);                           //  Low Speed - ALert
  delay(500);        
  noTone(buzzer);     
  delay(2000);
  
}*/


}

void SendMessage()
{
  Serial.println("AT+CMGF=1");
  delay(1000);
  
  Serial.println("AT+CMGS=\"+918525862472\"\r");        // COASTAL GUARD NUMBER 
  delay(1000);
  
  Serial.println("MESSAGE FROM BOAT");
  Serial.println(lat);
  Serial.println(" LNG = ");
  Serial.println(lng);
  delay(100);
  
  Serial.println((char)26);
  delay(1000);

}


