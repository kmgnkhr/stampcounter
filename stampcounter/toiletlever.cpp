#include "toiletlever.h"
#include <Arduino.h>

namespace {

bool readPort(int pin) {
  return ::digitalRead(pin) == LOW;
}

}  // namespace

ToiletLever::ToiletLever(int pin) : pin_(pin) {
}

void ToiletLever::begin() {
  ::pinMode(pin_, INPUT);
  changed_ = false;
  last_ = readPort(pin_);
}

void ToiletLever::update() {
  if (last_ != readPort(pin_)) {
    changed_ = true;
    last_ = !last_;
  }
  // waiting for stability
  ::delay(5);
}

bool ToiletLever::isFlushing() {
  return last_;
}

bool ToiletLever::wasChanged() {
  if (!changed_) return false;
  changed_ = false;
  return true;
}
