#include <M5StickC.h>
#include "toiletlever.h"

namespace {

const int kSensorPort = 26;
int RemainCount = 0;

ToiletLever lever(kSensorPort);

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

}  // namespace

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(8);
  lever.begin();

  draw();
}

void loop() {
  M5.update();
  lever.update();

  if (M5.BtnA.wasPressed()) {
    // new stamp
    RemainCount = 120;
  } else if (lever.wasChanged() && lever.isFlushing() && RemainCount > 0) {
    // flushing
    RemainCount--;
  } else {
    return;
  }
  draw();
}
