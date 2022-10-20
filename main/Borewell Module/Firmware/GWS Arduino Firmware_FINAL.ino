#include <Servo.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <SoftwareSerial.h>

#define Hall_Sensor_D 3
#define Dc_Motor_Trigger 4 // might throw error !!1
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
SoftwareSerial espSerial(0, 1);
String str;
Servo myservo;  // create servo object to control a servo


float latitude= 115.1501;
float longitude=8.7931;
int potpin = 0;  // Servo pot connector 
int val1,val2;    // Servo variable 
int magsensor_count=0;
int magsensor_read=0;
int pushsensor_val=0;
int timer =0;
int pushsensorval=0;


int depth = 0;
int water_level = 0; // calculate water level in 1 m pipe  
int volume = 0; //2*pi*rsq


void setup()   
{                
  Serial.begin(9600);
  espSerial.begin(115200);
  pinMode(Hall_Sensor_D,INPUT); //MagSensor Declaration
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
  pinMode(Dc_Motor_Trigger, OUTPUT);
  digitalWrite(Dc_Motor_Trigger, HIGH);                  //Motor Turn OFF at start 



  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  delay(500);

  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  delay(500);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.clearDisplay();
  display.println("Booting Up The System and Calibrating thre Motor");
  display.display();
  delay(500);

  pushsensor_val =analogRead(A1);
  Serial.println(pushsensor_val);
  delay(200);
  if (pushsensor_val !=1023)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.clearDisplay();
  display.println("Motor Working");
  display.display();
  digitalWrite(Dc_Motor_Trigger, LOW);                  //Motor Turn ON Code 

  delay(4000);
  digitalWrite(Dc_Motor_Trigger, HIGH);                  //Motor Turn ON Code 
}

  magsensor_count=0;
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.clearDisplay();
  display.println("Motor Callibrated Succesfully");
  display.display();
 delay(1000);

}

void loop() 

{

  display.clearDisplay();
  display.setCursor(0,10);

// Freefall Starts

magsensor_read =analogRead(A6); // reads the value of the potentiometer (value between 0 and 1023)
Serial.println(magsensor_read);
 
if (magsensor_read>1020)
{
  magsensor_count=magsensor_count+1;
}
timer=timer+0.3;
display.clearDisplay();
display.setCursor(0,10);

display.print(("Magnet Count: ")); // can be replaced with an DONEEE
display.println(magsensor_count);
display.print(("Timer: "));
display.println(timer);
display.print(("Latitude: "));
display.print(latitude);
display.println((" E"));

display.print(("Longitude: "));
display.print(longitude);
display.println((" S"));

if (depth==0)
{
display.print(("Water Level: "));
display.println("Calculating");

display.print(("Water Volume: "));
display.println("Calculating");

}

display.print(("Water Level: "));
display.print(water_level);
display.println((" cms "));

display.print(("Water Volume: "));
display.print(volume);
display.println((" cu cm"));

display.display();  

delay(400);


if (timer>10)
{

  DynamicJsonDocument doc(1024);
  depth = magsensor_count*7;
  if (depth > 100)
  {
    depth=80;
  }
  water_level = 100-depth; // calculate water level in 1 m pipe  
  volume = water_level*162.06; //2*pi*rsq


  doc["Rotation"] = magsensor_count;// no of rotation
  doc["Depth"] = depth;// Multiplication factor is 7cms per rotation
  doc["volume"] = volume;// Multiplication factor is 7cms per rotation
  doc["Lat"] =latitude ;
  doc["Long"] = longitude;
  display.println((" Data Sent to the Backend"));
  display.display();  
  timer=0;
  delay(500);

   serializeJson(doc, Serial);
   serializeJson(doc, espSerial);
 
}

}

void testfillrect(void) 
{
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    delay (250);
    display.display();
    color++;
  }
}

