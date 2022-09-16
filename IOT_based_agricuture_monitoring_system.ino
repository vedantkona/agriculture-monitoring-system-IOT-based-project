 
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
dht DHT;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial gsm(10, 11); // RX, TX
#define DHT11_PIN A3
int chk;
int humi = 0;
int temp = 0;
int soil = 0;
int light = 0;
int BMP = 0;
int gas = 0;
boolean HT;

void setup()
{
  
  gsm.begin(9600);
  bmp.begin();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Please wait for");
  lcd.setCursor(0, 1);
  lcd.print("30 seconds.");
  delay(5000);
  modem_init();
  data_init();
  internet_init();
  lcd.clear();
}

void loop()
{
  chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humi = DHT.humidity;
  soil = analogRead(A0);
  light = analogRead(A1);
  gas = analogRead(A2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  soil = map(soil, 0, 1023, 100, 0);
  lcd.print(soil);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  light = map(light, 0, 1023, 0, 100);
  lcd.print(light);
  lcd.print("%");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  
 
  
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print(" *C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humi);
    lcd.print("%");
  
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Qlt: ");
  gas = map(gas, 0, 1023, 0, 100);
  lcd.print(gas);
  lcd.print("%");
  BMP = bmp.readPressure();
  
  lcd.setCursor(0, 1);
  lcd.print("Pressure:");
  if (!bmp.begin())
  {
    lcd.print("No Data");
    BMP = 0;
  }
  else
  {
    lcd.print(BMP);
  }
  lcd.print("Pa");
  delay(3000);
  Send_data();
}

void modem_init()
{
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}

void data_init()
{
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000); delay(1000);
  gsm.println("AT+CPIN?");
  delay(1000); 
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("contype");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("GPRS");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000); ;
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("APN");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  
  gsm.print("airtelgprs.com"); 

  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("USER");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("PWD");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(2000);
  gsm.print("AT+SAPBR=1,1");
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(3000);
}

void internet_init()
{
  Serial.println("Please wait.....");
  delay(1000);
  gsm.println("AT+HTTPINIT");
  delay(1000); delay(1000);
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("CID");
  gsm.print('"');
  gsm.print(',');
  gsm.println('1');
  delay(1000);
}

void Send_data()
{
  lcd.clear();
  lcd.print("Sending the data");
  lcd.setCursor(0, 1);
  lcd.print("to Thingspeak...");
  delay(1500);
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("URL");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("http:");
  gsm.print('/');
  gsm.print('/');
 
 
  gsm.print("api.thingspeak.com/update?api_key==");  
  gsm.print(soil);// >>>>>>  variable 1
  delay(200);
  gsm.print("api.thingspeak.com/update?api_key==");
  gsm.print(light); // >>>>>> variable 2
  delay(200);
  gsm.print("api.thingspeak.com/update?api_key==");
  gsm.print(gas); // >>>>>> variable 3
  delay(200);
  gsm.print("api.thingspeak.com/update?api_key==");
  gsm.print(temp); // >>>>>> variable 4
  delay(200);
  gsm.print("api.thingspeak.com/update?api_key==");
  gsm.print(humi); // >>>>>> variable 5
  delay(200);
  gsm.print("api.thingspeak.com/update?api_key==");
  gsm.print(BMP); // >>>>>> variable 6
  delay(200);
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.println("AT+HTTPACTION=0");
  delay(1000);
}
