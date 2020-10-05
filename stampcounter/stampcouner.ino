#include <M5StickC.h>
#include "toiletlever.h"
#include "timercounter.h"

namespace {

const int kSensorPort = 26;
RTC_DATA_ATTR int RemainCount = 0;

ToiletLever lever(kSensorPort);
TimerCounter sleep_counter(30 * 1000);

void draw() {
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(2, 0);
  M5.Lcd.print("Last");

  M5.Lcd.setTextColor((RemainCount < 10) ? RED : WHITE);

  M5.Lcd.setTextSize(6);
  M5.Lcd.setCursor(26, 20);

  auto prefix = String("  ") + String(RemainCount);
  M5.Lcd.print(prefix.substring(prefix.length() - 3));

  M5.Lcd.setCursor(2, 70);
  M5.Lcd.setTextSize(1);
  if (RemainCount <= 0) {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("Time for new stamp !!!");
  } else if (RemainCount < 10) {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("Time for new stamp soon!");
  }
}

void enterSleep() {
  ::esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW);
  ::esp_sleep_enable_ext1_wakeup(BIT64(kSensorPort),ESP_EXT1_WAKEUP_ALL_LOW);

  M5.Axp.SetSleep();
  ::esp_deep_sleep_start();

  for(;;);  // never reach
}

}  // namespace

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(8);
  lever.begin();
  sleep_counter.begin();

  draw();
}

void loop() {
  M5.update();
  lever.update();

  if (M5.BtnA.wasPressed()) {
    // new stamp
    RemainCount = 120;
  } else if (lever.wasChanged()) {
    if (lever.isFlushing() && RemainCount > 0) {
      // flushing
      RemainCount--;
    }
  } else {
    if (lever.isFlushing()) {
      sleep_counter.reset();
    } else if (sleep_counter.isTimeOut()) {
      enterSleep();
    }
    return;
  }
  draw();
  sleep_counter.reset();
}
