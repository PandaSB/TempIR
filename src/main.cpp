#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <HardwareSerial.h>
#include <XPT2046_Touchscreen.h>
#include <Adafruit_MLX90614.h>


#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>


#define TFT_CS D0
#define TFT_DC D8
#define TS_CS  D3

#define BOXSIZE 40
#define SCREEN_H 320
#define SCREEN_W 240

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(TS_CS);

boolean Celcius = true ;
unsigned long starttime ;  

void setup() {
  Serial.begin(115200);
  tft.begin();
  ts.begin();
  ts.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
  
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);

  tft.fillRect(0, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*2, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*3, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE*4, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE*5, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
  
  tft.drawRect(0, SCREEN_H-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);

  tft.setFont(&FreeMonoBoldOblique12pt7b);

  tft.setTextSize (2);
  tft.setCursor(20,SCREEN_H/2);
  tft.setTextColor (ILI9341_YELLOW) ; 
  tft.print("IR Temp");

  mlx.begin();
  starttime = millis();

}

void loop() {
  double AmbientTempC ; 
  double ObjectTempC ; 
  double AmbientTempF ; 
  double ObjectTempF ; 
  unsigned long currenttime ; 
  uint16_t BackgroundColor ; 

  currenttime = millis(); 
  if (currenttime >= (starttime + 500))
  {
    AmbientTempC  = mlx.readAmbientTempC(); 
    ObjectTempC  = mlx.readObjectTempC(); 
    AmbientTempF = mlx.readAmbientTempF();
    ObjectTempF  = mlx.readObjectTempF();     
   
    Serial.print("Ambient = "); Serial.print((Celcius) ? AmbientTempC : AmbientTempF);Serial.print((Celcius)? "*C" : " F"); 
    Serial.print("\tObject = "); Serial.print((Celcius) ? ObjectTempC : ObjectTempF);Serial.println((Celcius)? "*C" : " F");

    Serial.println();
    starttime = currenttime ;

    if ( ObjectTempC < 37.5) BackgroundColor = ILI9341_GREEN ;
    else if ( ObjectTempC > 39) BackgroundColor = ILI9341_RED ; 
    else BackgroundColor = ILI9341_YELLOW ; 

  tft.fillRect(0, 0, SCREEN_W, 10,ILI9341_WHITE );
  tft.fillRect(0, 0, 10, SCREEN_H,ILI9341_WHITE );
  tft.fillRect(SCREEN_W -10 , 0, 10, SCREEN_H,ILI9341_WHITE );
  tft.fillRect(0, SCREEN_H -10 , SCREEN_W, 10,ILI9341_WHITE );

  tft.fillRect(10, 10, SCREEN_W-20, SCREEN_H-20,BackgroundColor );

  tft.setFont(&FreeSerifBold18pt7b);

  tft.setTextSize (2);
  tft.setCursor(20,SCREEN_H/2);
  if       ( BackgroundColor == ILI9341_GREEN ) tft.setTextColor (ILI9341_NAVY) ;
  else if  ( BackgroundColor == ILI9341_RED   ) tft.setTextColor (ILI9341_WHITE) ;
  else if  ( BackgroundColor == ILI9341_YELLOW) tft.setTextColor (ILI9341_BLUE) ;

  tft.printf("%02.1f%s",(Celcius) ? ObjectTempC : ObjectTempF ,(Celcius)? " C" : " F"  );

  tft.setFont();
  tft.setCursor(10,SCREEN_H-50);
  tft.printf("T Ambiante :%02.1f%s",(Celcius) ? AmbientTempC : AmbientTempF ,(Celcius)? " C" : " F"  );




  }

  if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
    delay(30);
    Serial.println();
  }

  delay(500);

}