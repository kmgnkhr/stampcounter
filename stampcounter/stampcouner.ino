#include <M5StickC.h>

namespace {

int RemainCount = 0;

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

class ToiletLever {
 public:
  explicit ToiletLever(int pin) : pin_(pin) {
  }

  void begin() {
    ::pinMode(pin_, INPUT);
    changed_ = false;
    last_ = readPort();
  }

  void update() {
    if (last_ != readPort()) {
      changed_ = true;
      last_ = !last_;
    }
    // waiting for stability
    ::delay(5);
  }

  bool isFlushing() const {
    return last_;
  }

  bool wasChanged() {
    if (!changed_) return false;
    changed_ = false;
    return true;
  }

 private:
  const int pin_;
  bool changed_;
  bool last_;

  bool readPort() const {
    return ::digitalRead(pin_) == LOW;
  }
};

const int kSensorPort = 26;
ToiletLever lever(kSensorPort);

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
