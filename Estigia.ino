#include "config.h"

TTGOClass *ttgo;

char buf[128];
bool irq = false;

void low_energy()
{
  ttgo->power->readIRQ();
  if (ttgo->power->isPEKShortPressIRQ())
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
  //ttgo->rtc->setDateTime(2021, 11, 5, 01, 45, 00);

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

  low_energy();
  
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
}
