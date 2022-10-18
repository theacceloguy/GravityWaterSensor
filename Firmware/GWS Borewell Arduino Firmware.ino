
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>


#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define Level1 A0
#define Level2 A1
#define Level3 A2
#define Level4 A3

float latitude= 115.1501;
float longitude=8.7931;
int Level1_val=0;
int Level2_val=0;
int Level3_val=0;
int Level4_val=0;


void setup()   {                
  Serial.begin(9600);

  pinMode(Level1,INPUT); //Level 1 Input eclaration
  pinMode(Level2,INPUT); //Level 1 Input eclaration
  pinMode(Level3,INPUT); //Level 1 Input eclaration
  pinMode(Level4,INPUT); //Level 1 Input eclaration

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
  display.println("Booting Up The System");
  delay(2000);
  display.display(); 

}


void loop() 

{

  display.clearDisplay();
  Level1_val =analogRead(Level1); // reads the value of the water level Sensor (value between 0 and 1)
  // Serial.print("Level1 : ");
  // Serial.println(Level1_val);
  Level2_val =analogRead(Level2); // reads the value of the water level Sensor (value between 0 and 1)
//   Serial.print("Level2 : ");
//  Serial.println(Level2_val);
 
  Level3_val =analogRead(Level3); // reads the value of the water level Sensor (value between 0 and 1)
//   Serial.print("Level3 : ");
//  Serial.println(Level3_val);
 
  Level4_val =analogRead(Level4); // reads the value of the water level Sensor (value between 0 and 1)
  // Serial.print("Level4 : ");
  // Serial.println(Level4_val);
 
if (Level1_val>1000)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Latitude: ");
  display.print(latitude);
  display.println(" E");
  display.print("Longitude: ");
  display.print(longitude);
  display.println(" S");
  display.println("Water Level: 7cms"); //Water Level On Actual Calculations
  display.println("Volume of Water: 0.005 cu meter "); // Water Volume V=pi*r*h  
  delay(10000);
  display.display(); 

}

if (Level1_val>1000 and Level2_val>1000)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);  // Draw white text
  display.print(("Latitude: "));
  display.print(latitude);
  display.println((" E"));
  display.print(("Longitude: "));
  display.print(longitude);
  display.println((" S"));
  display.println(("Water Level: 14cms")); //Water Level On Actual Calculations
  display.println(("Volume of Water: 0.001 cu meter ")); // Water Volume V=pi*r*h
  display.display();  
  delay(10000);

}

if (Level1_val>1000 && Level2_val>1000 && Level3_val>1000 )
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);  // Draw white text
  display.print(("Latitude: "));
  display.print(latitude);
  display.println((" E"));
  display.print(("Longitude: "));
  display.print(longitude);
  display.println((" S"));
  display.println(("Water Level: 21cms")); //Water Level On Actual Calculations
  display.println(("Volume of Water: 0.0011 cu meter  ")); // Water Volume V=pi*r*h
  display.display();  
  delay(10000);

}
if (Level1_val>1000 && Level2_val>1000 && Level3_val>1000 && Level4_val>1000)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);  // Draw white text
  display.print(("Latitude: "));
  display.print(latitude);
  display.println((" E"));
  display.print(("Longitude: "));
  display.print(longitude);
  display.println((" S"));
  display.println(("Water Level: 28cms")); //Water Level On Actual Calculations
  display.println(("Volume of Water: 0.002 cu meter  ")); // Water Volume V=pi*r*h
  display.display();  
  delay(10000);
}  

if (Level1_val<1000 && Level2_val<1000 && Level3_val<1000 && Level4_val<1000)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);  // Draw white text
  display.print(("Low Water Or Hardware ERROR Contact Support AKA Marsha"));
  display.display();  
  delay(5000);


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

