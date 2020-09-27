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

// I2C Touch pins
int i2c_sda = I2C_SDA;
int i2c_sdl = I2C_CLK;
uint32_t i2c_freq = I2C_FREQ;
// INTGPIO is touch interrupt, goes low when it detects a touch, which coordinates are read by I2C
FT6X36 ts(&Wire, INTGPIO);


void printTouchInfo(TPoint p, TEvent e)
{
  Serial.printf("printTouchInfo callback\n");
  if (e != TEvent::Tap && e != TEvent::DragStart && e != TEvent::DragMove && e != TEvent::DragEnd)
    return;

  Serial.print("X: ");
  Serial.print(p.x);
  Serial.print(", Y: ");
  Serial.print(p.y);
  Serial.print(", E: ");

  switch (e)
  {
  case TEvent::Tap:
    Serial.println("Tap");
    break;
  case TEvent::DragStart:
    Serial.println("DragStart");
    break;
  case TEvent::DragMove:
    Serial.println("DragMove");
    break;
  case TEvent::DragEnd:
    Serial.println("DragEnd");
    break;
  default:
    Serial.println("UNKNOWN");
    break;
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
  display.fillRoundRect(20,20,100,50,4,GxEPD_BLACK);
  display.update();

  ts.begin();
  ts.registerTouchHandler(touch);
}

void loop()
{
  ts.loop();
}
