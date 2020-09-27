#include <Arduino.h>
#include <Wire.h>

#include <FT6X36.h>                  // Touch class
#include <GxEPD.h>
#include <GxGDEW027W3/GxGDEW027W3.h> // Should match your epaper model
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// FONT used for title / message body
// Converting fonts with ümlauts: ./fontconvert *.ttf 18 32 252
#include <Fonts/FreeMono9pt7b.h>

// SPI interface GPIOs defined in platformio.ini
GxIO_Class io(SPI, EINK_CS, EINK_DC, EINK_RST);
// (GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
GxEPD_Class display(io, EINK_RST, EINK_BUSY );
bool printClearScreenButton = true;

// I2C Touch pins
int i2c_sda = I2C_SDA;
int i2c_sdl = I2C_CLK;
uint32_t i2c_freq = I2C_FREQ;
// INTGPIO is touch interrupt, goes low when it detects a touch, which coordinates are read by I2C
FT6X36 ts(&Wire, INTGPIO);

void drawClearButton(){
    display.fillRoundRect(1,display.height()-40,40,40,4,GxEPD_BLACK);
    display.setCursor(5,display.height()-10);
    display.setTextColor(GxEPD_WHITE);
    display.print("CLEAR");
    display.setTextColor(GxEPD_BLACK);
}

void printTouchInfo(TPoint p, TEvent e)
{
  if (e != TEvent::Tap && e != TEvent::DragStart && e != TEvent::DragMove && e != TEvent::DragEnd)
    return;

  std::string eventName = "";
  switch (e)
  {
  case TEvent::Tap:
    eventName = "tap";
    break;
  case TEvent::DragStart:
    eventName = "DragStart";
    break;
  case TEvent::DragMove:
    eventName = "DragMove";
    break;
  case TEvent::DragEnd:
    eventName = "DragEnd";
    break;
  default:
    eventName = "UNKNOWN";
    break;
  }

  Serial.printf("X: %d Y: %d E: %s\n", p.x, p.y, eventName.c_str());

  if (p.x<25 && p.y>200) { // Clear screen
    display.fillScreen(GxEPD_WHITE);
    drawClearButton();
    display.update();
  } else {
    // Print coordinates
    display.setCursor(p.x, p.y+10);
    display.printf("X:%d Y:%d E:%s", p.x, p.y, eventName.c_str());
    display.updateWindow(p.x, p.y, display.width(), 20);
  }
}

void touch(TPoint p, TEvent e)
{
  //Serial.printf("touchI() callback\n");
  printTouchInfo(p, e);
}

void setup()
{
  pinMode(INTGPIO, INPUT_PULLUP);


  Serial.begin(115200);
  Serial.printf("Starting. %d as interrupt PIN",INTGPIO);
  Wire.begin(i2c_sda,i2c_sdl,i2c_freq); // Defined in platformio.ini
  //Wire.begin(); // Will start I2C on default pins default speed
  display.init();
  //display.setRotation(1); // This won't work because it should be Touch rotation aware
  
  // Optional font
  //display.setFont(&FreeMono9pt7b);
  if (printClearScreenButton) {
    drawClearButton();
  }
  display.update();
  
  
  

  ts.begin();
  ts.registerTouchHandler(touch);
}

void loop()
{
  ts.loop();
}
