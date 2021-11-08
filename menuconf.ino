#include "config.h"

const int appnum = 4;
String appname[appnum] = {"Clock", "Set Time", "KillAll", "Honey"};

int launch()
{
  int selection = 0;
  int16_t x, y, tx, ty;
  bool exit = false;

  options(0);
  while (!exit)
  {
    if (ttgo->getTouch(x, y))
    {
      while (ttgo->getTouch(x, y)){}
      //Move up
      if (y >= 160)
      {
        selection += 1;
        if (selection == appnum)
          selection = 0;
        options(selection);
      }
      //Move down
      if (y <= 80)
      {
        selection -= 1;
        if (selection < appnum)
          selection = 0;
        options(selection);
      }
      //Select
      if (y > 80 && y < 160)
        exit = true;
    }
  }
  ttgo->tft->fillScreen(TFT_BLACK);
  return selection;
}

void options(int selection)
{
   int current = 0;
  // Display mode header
  ttgo->tft->fillScreen(TFT_GREEN);
  ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);

  // Display apps
  if (selection == 0)
    current = appnum - 1;
  else
    current = selection - 1;

  ttgo->tft->setTextSize(2);
  ttgo->tft->setCursor(50, 30);
  ttgo->tft->println(appname[current]);

  ttgo->tft->setTextSize(3);
  ttgo->tft->setCursor(40, 110);
  ttgo->tft->println(appname[selection]);

  if (selection == appnum - 1)
    current = 0;
  else
    current = selection + 1;

  ttgo->tft->setTextSize(2);
  ttgo->tft->setCursor(50, 190);
  ttgo->tft->print(appname[current]);
}
