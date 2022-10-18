#include <Servo.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>
#define Hall_Sensor_D 3
//#define Push_Sensor_D 7
#define Dc_Motor_Trigger 4
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

SoftwareSerial espSerial(0, 1);
String str;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Servo myservo;  // create servo object to control a servo
int potpin = 0;  // Servo pot connector 
int val1,val2;    // Servo variable 
int magsensor_count=0;
int magsensor_read=0;
int pushsensor_val=0;
float latitude= 115.1501;
float longitude=8.7931;
int timer =0;
int pushsensorval=0;



void setup()
{
////////////////////////////////////////////////////Intiation///////////////////////////////////////////  
  Serial.begin(115200);
  espSerial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3 C);  
  display.clearDisplay();

  pinMode(Hall_Sensor_D,INPUT); //MagSensor Declaration
//  pinMode(Push_Sensor_D,INPUT); //MagSensor Declaration

  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
  pinMode(Dc_Motor_Trigger, OUTPUT);
  digitalWrite(Dc_Motor_Trigger, HIGH);                  //Motor Turn OFF at start 
/////////////////////////////////////////////////////////////////////////////////////////////////////
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  // Draw white text
//  Wire.begin();
  display.print(("Calibrating the Motor")); // can be replaced with an initiation process kinda text
  pushsensor_val =analogRead(A1);
  Serial.println(pushsensor_val);

  //pushsensor_val =digitalRead(Push_Sensor_D);         //Read Push Button  
  delay(1000);
  while (pushsensor_val !=1023)
  {
    display.clearDisplay();
    display.setCursor(0,10);

    display.print(("MOTOR WORKING")); // can be replaced with an initiation process kinda text
    digitalWrite(Dc_Motor_Trigger, LOW);                  //Motor Turn ON Code 
    pushsensor_val =analogRead(A1);     //Read Push Button
    display.display();
    Serial.println(pushsensor_val);

    delay(500)  ; 
  }
  digitalWrite(Dc_Motor_Trigger,HIGH);                  //Motor Turn ON Code 

  magsensor_count=0;
  display.clearDisplay();
  display.setCursor(0,10);

  display.print(("Calibratated Succesfully")); // can be replaced with an DONEEE
  display.display();  
  delay(1000);

}

void loop()
{
//  display.clearDisplay();
  display.setCursor(0,10);

// Freefall Starts

magsensor_read =digitalRead(Hall_Sensor_D); // reads the value of the potentiometer (value between 0 and 1023)
if (magsensor_read==1)
{
  magsensor_count=magsensor_count+1;
}
timer=timer+1;
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

display.display();  

delay(1000);

if (timer>10)
{
  DynamicJsonDocument doc(1024);
  doc["Rotation"] = magsensor_count;
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