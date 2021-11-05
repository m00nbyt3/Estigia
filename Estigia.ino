#include "config.h"

TTGOClass *ttgo;

char buf[128];
bool irq = false;

void low_energy(int oway)
{
  ttgo->power->readIRQ();
  if (ttgo->power->isPEKShortPressIRQ() || oway)
  {
    ttgo->power->clearIRQ();
    irq = false;
    if (ttgo->bl->isOn())
    {
      ttgo->closeBL();
      ttgo->bma->enableStepCountInterrupt(false);
      ttgo->displaySleep();
      setCpuFrequencyMhz(10);
    }
    else
    {
      setCpuFrequencyMhz(80);
      ttgo->power->clearIRQ();
      ttgo->displayWakeup();
      ttgo->rtc->syncToSystem();
      ttgo->openBL();
      ttgo->bma->enableStepCountInterrupt();
    }
  }
}

void setup()
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->tft->fillScreen(TFT_BLACK);
  
  //Set date and time
  //ttgo->rtc->setDateTime(2021, 11, 6, 18, 54, 40);

  //Button config
  //--------------------------------------
  
  //Enable button input
  pinMode(AXP202_INT, INPUT_PULLUP);
  attachInterrupt(AXP202_INT, [] {
    irq = true;
  }, FALLING);
  
  //!Clear IRQ unprocessed  first
  ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
  ttgo->power->clearIRQ();
  ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
  //--------------------------------------
}

void loop()
{
  RTC_Date tnow = ttgo->rtc->getDateTime();
  int16_t x, y;
  int i = 0;

  low_energy(0);
  //Text size and color
  ttgo->tft->setTextSize(1);
  //ttgo->tft->setTextColor(TFT_GREEN);
  
  //Show battery left
  snprintf(buf, sizeof(buf), "Battery: %u", ttgo->power->getBattPercentage());
  ttgo->tft->drawString(buf, 22, 210, 2);

  //Show time
  snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
  ttgo->tft->drawString(buf, 1, 10, 7);

  //Show date
  snprintf(buf, sizeof(buf), "%s - %s - %s", (String)tnow.day, (String)tnow.month, (String)tnow.year);
  ttgo->tft->drawString(buf, 90, 80, 4);

  //Show name
  ttgo->tft->drawString("m00nbyt3", 9, 150, 4);

  //Load menu
  if (ttgo->getTouch(x, y))
  {
    while (ttgo->getTouch(x, y))
    {
      i += 1;
      delay(100);
      if (i == 12)
      {
        low_energy(1);
        break;
      }
    }
    switch (launch())
    {
      case 0:
        break;
      case 1:
        ttgo->tft->fillScreen(TFT_BLUE);
        ttgo->tft->drawString("Im here...", 9, 150, 4);
        while (!(ttgo->getTouch(x, y))){}
        break;
      case 2:
        ttgo->tft->fillScreen(TFT_RED);
        ttgo->tft->drawString("Kill haha", 9, 150, 4);
        while (!(ttgo->getTouch(x, y))){}
        break;
      case 3:
        ttgo->tft->fillScreen(TFT_YELLOW);
        while (!(ttgo->getTouch(x, y))){}
        break;
    }
  }
}
