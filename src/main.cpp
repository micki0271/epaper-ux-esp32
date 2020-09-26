#include <Arduino.h>
#include <Wire.h>
#include <FT6X36.h>


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
  //Wire.begin(21,22,50000); // 21 SDA  22 SDL
  Wire.begin();
  
  ts.begin();
  ts.registerTouchHandler(touch);
}

void loop()
{
  ts.loop();
}
